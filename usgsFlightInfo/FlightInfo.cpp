#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <stdlib.h>
#include <sys/stat.h>

#include "Flight/Flight.h"

using namespace std;

/**
 * @brief Get the Full Path to a file
 * 
 * @param relativePath relative path to the file
 * @return the full path to the file as a string
 */
string getFullPath(string relativePath) {
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

/**
 * @brief comparator function to sort a list of flights in ascending order based on departure times
 * 
 * @param f1 the first flight to consider
 * @param f2 the second flight to consider
 * @return true if f1 departs before f2
 * @return false if f2 departs before f1
 */
bool firstDeparture(Flight f1, Flight f2) {
    return (f1.getDepartureTime() < f2.getDepartureTime());
}

/**
 * @brief Get the City Code associated with the flight number
 * 
 * @param flightNumber the flight number to consider
 * @return string the airport code associated with the given flight number
 */
string getCityCode(int flightNumber) {
    if (flightNumber == 455 || flightNumber == 456) {
        return "HVN";
    } else if (flightNumber == 387 || flightNumber == 388) {
        return "TPA";
    } else if (flightNumber == 771 || flightNumber == 772 ||
               flightNumber == 481 || flightNumber == 482) {
        return "FLL";
    } else if (flightNumber == 780 || flightNumber == 781) {
        return "MCO";
    } else if (flightNumber == 471 || flightNumber == 472 ||
               flightNumber == 773 || flightNumber == 774) {
        return "BWI";
    } else if (flightNumber == 575 || flightNumber == 576) {
        return "ILG";
    } else if (flightNumber == 909 || flightNumber == 910) {
        return "MSP";
    } else {
        return "Invalid";
    }
}

/**
 * @brief runs the program.
 * 
 * @return int system exit status
 */
int main(int argc, char** argv) {

    //determine if running in testing mode
    bool testing = false;
    if (argc == 2) {
        if (strcmp(argv[1], "t") == 0) {
            cout << "Entering testing mode..." << endl;
            testing = true;
        } else {
            cout << "Invalid Arguments. Please try again." << endl;
            return -1;
        }
    } else if (argc > 2) {
        cout << "Invalid Arguments. Please try again." << endl;
        return -1;
    }
    
    // ask user which schedule to use
    string day;
    string input;
    ifstream file;
    cout << "Please enter the day(S/M/T/Th/F/Sa): ";
    cin >> day;
    transform(day.begin(), day.end(), day.begin(), ::toupper);
    string fullPath;
    if (day == "S" || day == "TH") {
        cout << "\nOpening Sunday/Thursday schedule...\n";
        if (testing) {
            fullPath = "schedules/sun-thurs schedule.txt";
        } else {
            fullPath = getFullPath("Desktop/schedules/sun-thurs schedule.txt");
            if (fullPath.empty()) {
                cerr << "Unable to resolve full path for the schedule file." << endl;
                return -1;
            }
        }

    } else if (day == "M" || day == "F") {
        cout << "\nOpening Monday/Friday schedule...\n";
        if (testing) {
            fullPath = "schedules/mon-fri schedule.txt";
        } else {
            fullPath = getFullPath("Desktop/schedules/mon-fri schedule.txt");
            if (fullPath.empty()) {
                cerr << "Unable to resolve full path for the schedule file." << endl;
                return -1;
            }
        }

    } else if (day == "T") {
        cout << "\nOpening Tuesday schedule...\n";
        if (testing) {
            fullPath = "schedules/tue schedule.txt";
        } else {
            fullPath = getFullPath("Desktop/schedules/tue schedule.txt");
            if (fullPath.empty()) {
                cerr << "Unable to resolve full path for the schedule file." << endl;
                return -1;
            }
        }

    } else if (day == "SA") {
        cout << "\nOpening Saturday schedule...\n";
        if (testing) {
            fullPath = "schedules/sat schedule.txt";
        } else {
            fullPath = getFullPath("Desktop/schedules/sat schedule.txt");
            if (fullPath.empty()) {
                cerr << "Unable to resolve full Path for the schedule file." << endl;
                return -1;
            }
        }

    } else {
        cerr << "Invalid day. Please try again." << endl;
        return -1;
    }

    file.open(fullPath);

    vector<Flight> flights;

    string arrivalInfo;
    string departureInfo;

    // read each flight and add to list of flights
    if (file.is_open()) {
        while (getline(file, input)) {

            string airlineCode;
            int arrivalNumber, departureNumber;
            int arrivalTime, departureTime;

            istringstream buffer(input);
            buffer >> arrivalNumber >> departureNumber;

            string arrivalCity = getCityCode(arrivalNumber);
            if (arrivalCity == "Invalid") {
                cerr << "Invalid Flight Number: " << arrivalNumber << ". Please check schedule" << endl;
                return -1;
            }

            string departureCity = getCityCode(departureNumber);
            if (departureCity == "Invalid") {
                cerr << "Invalid Flight Number: " << departureNumber << ". Please check schedule" << endl;
                return -1;
            }

            if (arrivalNumber == 909) {
                airlineCode = "SY";
            } else {
                airlineCode = "XP";
            }
            
            Flight flight(airlineCode, arrivalNumber, arrivalCity, departureNumber, departureCity);

            departureInfo = flight.getDepartureNumber();
            arrivalInfo = flight.getArrivalNumber();

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

            flights.push_back(flight);
        }

        file.close();
        sort(flights.begin(), flights.end(), firstDeparture);
        cout << endl;
    } else {
        cerr << "unable to open schedule file" << endl;
        return -1;
    }

    int inboundWchr;
    int outboundPax;
    int outboundWchr;
    int gate;

    // ask user for the rest of the information
    for (Flight &flight : flights) {
        string arrivalInfo = flight.getArrivalNumber();
        string departureInfo = flight.getDepartureNumber();

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

        cout << endl;
    }

    // write output to the file
    string outputPath;
    if (testing) {
        outputPath = "output.txt";
    } else {
        outputPath = getFullPath("Desktop/output.txt");
        if (outputPath.empty()) {
            cerr << "Unable to resolve full path for output file." << endl;
            return -1;
        }
    }

    remove(outputPath.c_str());
    ofstream output(outputPath.c_str());
    if (output.is_open()) {
        for (Flight &flight : flights) {
            output << flight.printFlight();
        }
        output.close();
        cout << "Output has been saved to " << outputPath << endl;
    } else {
        cerr << "unable to open output file at path: " << outputPath << endl;
        return -1;
    }

    return 0;
}