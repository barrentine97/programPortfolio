#include "Flight.h"

string Flight::addPadding(int w, char fill, int number) {
    stringstream ss;
    ss << setw(w) << setfill(fill) << number;
    return ss.str();
}

string Flight::getCityCode(int flightNumber) {
    if (flightNumber == 455 || flightNumber == 456) {
        return "HVN";
    } else if (flightNumber == 387 || flightNumber == 388) {
        return "TPA";
    } else if (flightNumber == 771 || flightNumber == 772) {
        return "FLL";
    } else if (flightNumber == 780 || flightNumber == 781) {
        return "MCO";
    } else if (flightNumber == 471 || flightNumber == 472) {
        return "BWI";
    } else if (flightNumber == 575 || flightNumber == 576) {
        return "ILG";
    } else if (flightNumber == 909 || flightNumber == 910) {
        return "MSP";
    } else {
        return "Invalid";
    }
}

Flight::Flight(string airlineCode, int arrivalNumber, int departureNumber) {
    this->airlineCode = airlineCode;

    this->arrivalNumber = arrivalNumber;
    this->arrivalCity = getCityCode(arrivalNumber);

    this->departureNumber = departureNumber;
    this->departureCity = getCityCode(departureNumber);

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

string Flight::getDepartureNumber() {
    return airlineCode + to_string(departureNumber);
}

bool Flight::isValid() {
    return (arrivalCity != "Invalid" && departureCity != "Invalid");
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