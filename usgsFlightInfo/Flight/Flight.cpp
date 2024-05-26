#include "Flight.h"

string Flight::addPadding(int w, char fill, int number) {
    stringstream ss;
    ss << setw(w) << setfill(fill) << number;
    return ss.str();
}

Flight::Flight(string airlineCode, int arrivalNumber, string arrivalCity, int departureNumber, string departureCity) {
    this->airlineCode = airlineCode;

    this->arrivalNumber = arrivalNumber;
    this->arrivalCity = arrivalCity;

    this->departureNumber = departureNumber;
    this->departureCity = departureCity;

    this->gate = 4;
}

void Flight::setArrivalTime(int arrivalTime) {
    this->arrivalTime = arrivalTime;
}

void Flight::setDepartureTime(int departureTime) {
    this->departureTime = departureTime;
}

void Flight::setOutboundPax(int outboundPax) {
    this->outboundPax = outboundPax;
}

void Flight::setInboundWchr(int inboundWchr) {
    this->inboundWchr = inboundWchr;
}

void Flight::setOutboundWchr(int outboundWchr) {
    this->outboundWchr = outboundWchr;
}

void Flight::setGate(int gate) {
    this->gate = gate;
}

string Flight::getArrivalNumber() {
    return airlineCode + to_string(arrivalNumber);
}

int Flight::getArrivalTime() {
    return arrivalTime;
}

string Flight::getDepartureNumber() {
    return airlineCode + to_string(departureNumber);
}

int Flight::getDepartureTime() {
    return departureTime;
}

string Flight::printFlight() {
    string paxPadded = addPadding(3, ' ', outboundPax);
    string arrivalPadded = addPadding(4, '0', arrivalTime);
    string departurePadded = addPadding(4, '0', departureTime);

    return getArrivalNumber() + " / " + getDepartureNumber() + " -- " +
            arrivalCity + "-ILM-" + departureCity + " -- " +
            "arrives " + arrivalPadded + " / " + "departs " + departurePadded + " -- " +
            paxPadded + " passengers -- " +
            to_string(inboundWchr) + " wchr in / " + to_string(outboundWchr) + " wchr out -- " +
            "gate " + to_string(gate) + "\n";
}