#include <iostream>
#include <xlnt/xlnt.hpp>
#include <ctime>
#include <sstream>
#include <fstream>

using namespace std;

string WORKBOOK_NAME = "Avelo Flight Stats 2024.xlsx";

string getCurrentMonthSheet() {
    // get the current date
    time_t now = time(0);
    tm *ltm = localtime(&now);

    // format month as a string
    string month;
    switch (ltm->tm_mon) {
        case 0: month = "January"; break;
        case 1: month = "February"; break;
        case 2: month = "March"; break;
        case 3: month = "April"; break;
        case 4: month = "May"; break;
        case 5: month = "June"; break;
        case 6: month = "July"; break;
        case 7: month = "August"; break;
        case 8: month = "September"; break;
        case 9: month = "October"; break;
        case 10: month = "November"; break;
        case 11: month = "December"; break;
    }

    return month;
}

string formatExcelDate(double serialDate) {
    const double EXCEL_EPOCH_OFFSET = 25569;

    if (serialDate < EXCEL_EPOCH_OFFSET) {
        return "Invalid Date";
    }

    time_t rawTime = (serialDate - EXCEL_EPOCH_OFFSET) * 86400;

    tm timeStruct;
    gmtime_r(&rawTime, &timeStruct);

    stringstream formattedDate;
    formattedDate << put_time(&timeStruct, "%m/%d/%y");

    return formattedDate.str();
}

time_t convertToTimeT(const string &dateStr) {
    tm timeStruct = {};
    stringstream ss(dateStr);
    ss >> get_time(&timeStruct, "%m/%d/%y");

    return mktime(&timeStruct);
}

string generateReport(const string &filename, const string &startDate, const string &endDate) {
    xlnt::workbook wb;
    wb.load(filename);

    string currentMonth = getCurrentMonthSheet();
    if (!wb.contains(currentMonth)) {
        cerr << "Sheet for " << currentMonth << " not found in the workbook" << endl;
        exit(-1);
    }

    xlnt::worksheet ws = wb.sheet_by_title(currentMonth);
    string report;
    int rowIndex = 0;
    string lastDate = "";

    time_t start = convertToTimeT(startDate);
    time_t end = convertToTimeT(endDate);

    for (auto rows : ws.rows(false)) {
        if (rowIndex < 2) {
            rowIndex++;
            continue;
        }

        double serialDate = rows[0].value<double>();
        string date = formatExcelDate(serialDate);

        if (date == "Invalid Date") {
            continue;
        }

        time_t flightTime = convertToTimeT(date);
        if (flightTime < start || flightTime > end) {
            continue;
        }

        if (date != lastDate) {
            report += date + ":\n\n";
            lastDate = date;
        }

        string flightInfo = rows[1].to_string();
        string paxIn = rows[7].to_string();
        string paxOut = rows[8].to_string();

        report += flightInfo + "\n";
        report += "Pax In: " + paxIn + "\n";
        report += "Pax Out: " + paxOut + "\n\n";
    }

    return report;
}

int main(int argc, char** argv) {
    string startDate, endDate;

    cout << "Enter start date (MM/DD/YY): ";
    cin >> startDate;
    cout << "Enter end date (MM/DD/YY): ";
    cin >> endDate;

    string outputPath = "output.txt";

    remove(outputPath.c_str());
    ofstream output(outputPath.c_str());
    if (output.is_open()) {
        output << generateReport(WORKBOOK_NAME, startDate, endDate);
        output.close();
        cout << "Output has been saved to " << outputPath << endl;
    } else {
        cerr << "Unable to open output file at path: " << outputPath << endl;
        return -1;
    }

    return 0;
}
