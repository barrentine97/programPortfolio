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
    const double EXCEL_EPOCH_OFFSET = 25569;  // Excel's epoch (1/1/1900) vs Unix epoch (1/1/1970)

    // Skip invalid dates, those should correspond to serial number < EXCEL_EPOCH_OFFSET
    if (serialDate < EXCEL_EPOCH_OFFSET) {
        return "Invalid Date";  // Return a placeholder for invalid dates
    }

    time_t rawTime = (serialDate - EXCEL_EPOCH_OFFSET) * 86400;  // 86400 seconds in a day

    tm timeStruct;
    gmtime_r(&rawTime, &timeStruct);  // Convert to GMT time

    // Format the date in MM/DD/YY format (e.g., 11/01/24)
    stringstream formattedDate;
    formattedDate << put_time(&timeStruct, "%m/%d/%y");

    return formattedDate.str();
}

string generateMonthlyReport(const string &filename) {
    xlnt::workbook wb;
    wb.load(filename);

    // get the sheet for the current month
    string currentMonth = getCurrentMonthSheet();
    if (!wb.contains(currentMonth)) {
        cerr << "Sheet for " << currentMonth << " not found in the workbook" << endl;
        exit(-1);
    }

    xlnt::worksheet ws = wb.sheet_by_title(currentMonth);
    string report;
    int rowIndex = 0;
    string lastDate = "";

    for (auto rows : ws.rows(false)) {
        if (rowIndex < 2) {  // Skip header rows
            rowIndex++;
            continue;
        }

        // Get the date and convert from Excel serial date
        double serialDate = rows[0].value<double>();
        string date = formatExcelDate(serialDate);
        if (date == "Invalid Date") {
            continue;  // Skip invalid dates
        }

        // Only print the date once
        if (date != lastDate) {
            report += date + ":\n\n";
            lastDate = date;  // Update the last printed date
        }

        // Extract flight information and Pax data
        string flightInfo = rows[2].to_string();
        string paxIn = rows[7].to_string();
        string paxOut = rows[8].to_string();

        // Add flight info under the current date
        report += flightInfo + "\n";
        report += "Pax In: " + paxIn + "\n";
        report += "Pax Out: " + paxOut + "\n\n";
    }

    return report;
}


int main(int argc, char** argv) {
    string outputPath = "output.txt";

    remove(outputPath.c_str());
    ofstream output(outputPath.c_str());
    if (output.is_open()) {
        output << generateMonthlyReport(WORKBOOK_NAME);
        output.close();
        cout << "Output has been saved to " << outputPath << endl;
    } else {
        cerr << "unable to open output file at path: " << outputPath << endl;
        return -1;
    }

    return 0;
}