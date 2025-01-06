#pragma once

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

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

public:
    Flight(string airlineCode, int arrivalNumber, string arrivalCode, int departureNumber, string departureCode);
    void setArrivalTime(int arrivalTime);
    void setDepartureTime(int departureTime);
    void setOutboundPax(int outboundPax);
    void setInboundWchr(int inboundWchr);
    void setOutboundWchr(int outboundWchr);
    void setGate(int gate);
    string getArrivalInfo();
    int getArrivalTime();
    string getDepartureInfo();
    int getDepartureTime();
    string printFlight();
};