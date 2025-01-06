#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <map>
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

map<int, string> arrivals;
map<int, string> departures;
map<int, string> airlineCodes;

/**
 * @brief reads the flight city codes from the file path and stores them in the cityCodes map
 * 
 * @param filePath the filepath to read from
 */
void loadCityCodes(const string &filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open flight code file: " << filePath << endl;
        exit(-1);
    }

    string line, currentAirline;
    bool isArrivals = false;

    while (getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));  // Trim leading whitespace
        line.erase(line.find_last_not_of(" \t\r\n") + 1);  // Trim trailing whitespace

        if (line.empty()) continue;

        // Check for airline code (e.g., XP)
        if (isalpha(line[0]) && line.size() == 2) {
            currentAirline = line;  // Set the current airline when we find it
            continue;
        }

        // check if we are reading arrivals or departures
        if (line == "Arrivals:" || line == "Departures:") {
            isArrivals = (line == "Arrivals:");
            continue;
        }

        // Now read the flight number and city code from the line
        int flightNumber;
        string cityCode;
        istringstream buffer(line);
        buffer >> flightNumber >> cityCode;

        if (buffer.fail() || cityCode.empty()) {
            cerr << "Error: Malformed line in file: " << line << endl;
            continue;
        }

        // Store city code and airline code
        airlineCodes[flightNumber] = currentAirline;
        if (isArrivals) {
            arrivals[flightNumber] = cityCode;
        } else {
            departures[flightNumber] = cityCode;
        }
    }

    file.close();
}

/**
 * @brief Get the City Code associated with the flight number
 * 
 * @param flightNumber the flight number to consider
 * @return string the airport code associated with the given flight number
 */
string getCityCode(int flightNumber, bool isArrival) {
    // Choose the correct map based on whether it's an arrival or departure
    const map<int, string>& cityCodes = isArrival ? arrivals : departures;

    auto code = cityCodes.find(flightNumber);
    if (code != cityCodes.end()) {
        return code->second;
    } else {
        return "Invalid";
    }
}

string getSchedulePath(const string &day, bool testing) {
    map<string, string> scheduleMap = {
        {"S", "sun-thurs schedule.txt"},
        {"TH", "sun-thurs schedule.txt"},
        {"M", "mon-fri schedule.txt"},
        {"F", "mon-fri schedule.txt"},
        {"T", "tue schedule.txt"},
        {"SA", "sat schedule.txt"}
    };

    auto it = scheduleMap.find(day);
    if (it == scheduleMap.end()) {
        return "";
    }

    string relativePath = "schedules/" + it->second;
    return testing ? relativePath : getFullPath("Desktop/" + relativePath);
}

/**
 * @brief runs the program.
 * 
 * @return int system exit status
 */
int main(int argc, char** argv) {
    bool testing = (argc == 2 && strcmp(argv[1], "t") == 0);

    string fullPath = testing ? "schedules/flightCodes.txt" : getFullPath("Desktop/schedules/flightCodes.txt");
    if (fullPath.empty()) {
        return -1;
    }

    loadCityCodes(fullPath);

    cout << "Please enter the day (S/M/T/Th/F/Sa): ";
    string day;
    cin >> day;
    transform(day.begin(), day.end(), day.begin(), ::toupper);

    fullPath = getSchedulePath(day, testing);
    if (fullPath.empty()) {
        cerr << "Invalid day or schedule not found." << endl;
        return -1;
    }

    ifstream file(fullPath);
    if (!file.is_open()) {
        cerr << "Unable to open schedule file: " << fullPath << endl;
        return -1;
    }

    vector<Flight> flights;
    string input;

    while (getline(file, input)) {
        input.erase(input.find_last_not_of(" \n\r\t") + 1);
        input.erase(0, input.find_first_not_of(" \n\r\t"));

        if (input.empty()) continue;

        istringstream buffer(input);
        int arrivalNumber, departureNumber;
        if (!(buffer >> arrivalNumber >> departureNumber)) {
            cerr << "Invalid schedule entry: " << input << endl;
            continue;
        }

        string arrivalCity = getCityCode(arrivalNumber, true);
        string departureCity = getCityCode(departureNumber, false);
        if (arrivalCity == "Invalid" || departureCity == "Invalid") {
            cerr << "Invalid flight numbers: " << input << endl;
            continue;
        }

        string airlineCode = airlineCodes[arrivalNumber];
        Flight flight(airlineCode, arrivalNumber, arrivalCity, departureNumber, departureCity);

        int arrivalTime;
        int departureTime;
        if (buffer.rdbuf()->in_avail() == 0) {
            cout << flight.getArrivalInfo() << " Scheduled Arrival: ";
            cin >> arrivalTime;
            cout << flight.getDepartureInfo() << " Scheduled Departure: ";
            cin >> departureTime;
            cout << endl;
        } else {
            buffer >> arrivalTime >> departureTime;
        }

        flight.setArrivalTime(arrivalTime);
        flight.setDepartureTime(departureTime);

        flights.push_back(flight);
    }

    file.close();
    sort(flights.begin(), flights.end(), firstDeparture);

    // ask user for the rest of the information
    int inboundWchr;
    int outboundPax;
    int outboundWchr;
    int gate;
    for (Flight &flight : flights) {
        string arrivalInfo = flight.getArrivalInfo();
        string departureInfo = flight.getDepartureInfo();

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