#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Employee {
    string name;
    int numHours = 0;
    int mileage = 0;
};

int getHours(const string &city) {
    if (city == "FAY") {
        return 2;
    } else if (city == "PGV") {
        return 3;
    } else if (city == "ILM" || city == "GSO" || city == "OAJ") {
        return 4;
    }
    return 0;
}

int getMileage(const string &city) {
    if (city == "FAY") {
        return 64;
    } else if (city == "PGV") {
        return 70;
    } else if (city == "GSO") {
        return 113;
    } else if (city == "ILM") {
        return 115;
    } else if (city == "OAJ") {
        return 93;
    }
    return 0;
}

vector<string> splitStringByDelimiters(string &input, string &delimiters) {
    vector<string> result;
    int start = 0;
    int end = 0;

    while ((end = input.find_first_of(delimiters, start)) != string::npos) {
        if (end != start) {
            result.push_back(input.substr(start, end - start));
        }
        start = end + 1;
    }

    if (start != input.length()) {
        result.push_back(input.substr(start));
    }

    return result;
}

void trim(string &s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    s = (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

int main() {
    ifstream file;
    string input;
    string delimiters = ":,";
    file.open("travelTimes.txt");
    if (file.is_open()) {
        vector<Employee> employees;
        while (getline(file, input)) {
            vector<string> tokens = splitStringByDelimiters(input, delimiters);
            if (!tokens.empty()) {
                Employee emp;
                emp.name = tokens[0];
                trim(emp.name);

                for (int i = 1; i < tokens.size(); i++) {
                    string city = tokens[i];
                    trim(city);

                    if (!city.empty() && city.back() == 'd') {
                        city.pop_back();
                        trim(city);
                        emp.mileage += getMileage(city);
                    }
                    
                    emp.numHours += getHours(city);
                }

                employees.push_back(emp);
            }
        }

        for (const Employee &emp : employees) {
            cout << "Name: " << emp.name << endl
                 << "Hours: " << emp.numHours << endl
                 << "Mileage: " << emp.mileage << endl << endl;
        }

    } else {
        cerr << "unable to open file" << endl;
        return -1;
    }

    file.close();
    return 0;
}