#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>

#include "Flight/Flight.h"

using namespace std;

int main() {
    char day;
    string input;
    ifstream file;

    cout << "Please enter the day(S/M/T/F): ";
    cin >> day;
    day = toupper(day);
    if (day == 'S' || day == 'T') {
        cout << "\nOpening Sunday/Thursday schedule...\n";
        file.open("schedules/sun-thurs schedule.txt");
    } else if (day == 'M' || day == 'F') {
        cout << "\nOpening Monday/Friday schedule...\n";
        file.open("schedules/mon-fri schedule.txt");
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
            

            cout << departureInfo << " outbound pax: ";
            cin >> outboundPax;
            flight.setOutboundPax(outboundPax);
            cout << arrivalInfo << " inbound wchr: ";
            cin >> inboundWchr;
            flight.setInboundWchr(inboundWchr);
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

    remove("output.txt");
    ofstream output;
    output.open("output.txt");
    if (output.is_open()) {
        for (Flight flight : flights) {
            output << flight.printFlight();
        }
        output.close();
        cout << "Output has been saved to output.txt" << endl;

    } else {
        cerr << "unable to open output file" << endl;
        return -1;
    }

    return 0;
}
