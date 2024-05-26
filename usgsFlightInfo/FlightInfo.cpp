#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <stdlib.h>
#include <sys/stat.h>

#include "Flight/Flight.h"

using namespace std;

string getFullPath(const string &relativePath) {
    const char* homeDir = getenv("HOME");
    if (homeDir == nullptr) {
        cerr << "Error: HOME environment variable not set." << endl;
        return "";
    }

    string fullPath = string(homeDir) + "/" + relativePath;

    // Check if the directory exists
    size_t pos = fullPath.find_last_of('/');
    if (pos != string::npos) {
        string dir = fullPath.substr(0, pos);
        struct stat info;
        if (stat(dir.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR)) {
            cerr << "Error: Directory " << dir << " does not exist." << endl;
            return "";
        }
    }
    return fullPath;
}

int main() {
    char day;
    string input;
    ifstream file;

    cout << "Please enter the day(S/M/T/F): ";
    cin >> day;
    day = toupper(day);
    string fullPath;
    if (day == 'S' || day == 'T') {
        cout << "\nOpening Sunday/Thursday schedule...\n";
        fullPath = getFullPath("Desktop/schedules/sun-thurs schedule.txt");
        if (fullPath.empty()) {
            cerr << "Unable to resolve full path for the schedule file." << endl;
            return -1;
        }
        file.open(fullPath);
    } else if (day == 'M' || day == 'F') {
        cout << "\nOpening Monday/Friday schedule...\n";
        fullPath = getFullPath("Desktop/schedules/mon-fri schedule.txt");
        if (fullPath.empty()) {
            cerr << "Unable to resolve full path for the schedule file." << endl;
            return -1;
        }
        file.open(fullPath);
    } else {
        cerr << "Invalid day. Please try again." << endl;
        return -1;
    }

    vector<Flight> flights;

    if (file.is_open()) {
        while (getline(file, input)) {

            string airlineCode;
            int arrivalNumber;
            int arrivalTime;
            int departureNumber;
            int departureTime;
            int outboundPax;
            int inboundWchr;
            int outboundWchr;
            int gate;

            istringstream buffer(input);
            buffer >> arrivalNumber >> departureNumber;
            if (arrivalNumber == 909) {
                airlineCode = "SY";
            } else {
                airlineCode = "XP";
            }
            
            Flight flight(airlineCode, arrivalNumber, departureNumber);

            if (!flight.isValid()) {
                cerr << "Invalid Flight Number. Please check schedule" << endl;
                return -1;
            }

            string departureInfo = flight.getDepartureNumber();
            string arrivalInfo = flight.getArrivalNumber();

            if (buffer.rdbuf()->in_avail() == 0) {
                cout << arrivalInfo << " Scheduled Arrival: ";
                cin >> arrivalTime;
                cout << departureInfo << " Scheduled Departure: ";
                cin >> departureTime;
            } else {
                buffer >> arrivalTime >> departureTime;
            }

            flight.setArrivalTime(arrivalTime);
            flight.setDepartureTime(departureTime);
            

            cout << arrivalInfo << " inbound wchr: ";
            cin >> inboundWchr;
            flight.setInboundWchr(inboundWchr);
            cout << departureInfo << " outbound pax: ";
            cin >> outboundPax;
            flight.setOutboundPax(outboundPax);
            cout << departureInfo << " outbound wchr: ";
            cin >> outboundWchr;
            flight.setOutboundWchr(outboundWchr);
            cout << departureInfo << " gate: ";
            cin >> gate;
            flight.setGate(gate);

            flights.push_back(flight);
            cout << endl;
        }

        file.close();

    } else {
        cerr << "unable to open schedule file" << endl;
        return -1;
    }

    string outputPath = getFullPath("Desktop/output.txt");
    if (outputPath.empty()) {
        cerr << "Unable to resolve full path for output file." << endl;
        return -1;
    }

    remove(outputPath.c_str());
    ofstream output(outputPath.c_str());
    if (output.is_open()) {
        for (Flight flight : flights) {
            output << flight.printFlight();
        }
        output.close();
        cout << "Output has been saved to output.txt at path: " << outputPath << endl;
    } else {
        cerr << "unable to open output file at path: " << outputPath << endl;
        return -1;
    }

    return 0;
}
