#pragma once

#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

class Flight {
    string airlineCode;
    int arrivalNumber;
    string arrivalCity;
    int arrivalTime;
    int departureNumber;
    string departureCity;
    int departureTime;
    int outboundPax;
    int inboundWchr;
    int outboundWchr;
    int gate;
    
    string addPadding(int w, char fill, int number);
    string getCityCode(int flightNumber);

public:
    Flight(string airlineCode, int arrivalNumber, int departureNumber);
    void setArrivalTime(int arrivalTime);
    void setDepartureTime(int departureTime);
    void setOutboundPax(int outboundPax);
    void setInboundWchr(int inboundWchr);
    void setOutboundWchr(int outboundWchr);
    void setGate(int gate);
    string getArrivalNumber();
    string getDepartureNumber();
    bool isValid();
    string printFlight();
};