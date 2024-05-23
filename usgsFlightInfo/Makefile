all: flightInfo.o flight.o
	clang++ flightInfo.o flight.o -o flightInfo

flightInfo.o: flightInfo.cpp
	clang++ -std=c++11 flightInfo.cpp -c -o flightInfo.o

flight.o: Flight/Flight.cpp Flight/Flight.h
	clang++ -std=c++11 Flight/Flight.cpp -c -o flight.o

clean:
	rm -f flightInfo
	rm -f *.o
	rm -f output.txt