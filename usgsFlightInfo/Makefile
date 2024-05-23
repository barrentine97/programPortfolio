all: flightInfo.o Flight.o
	clang++ flightInfo.o flight.o -o flightInfo

flightInfo.o: flightInfo.cpp
	clang++ -std=c++11 flightInfo.cpp -c -o flightInfo.o

Flight.o: Flight/Flight.cpp Flight/Flight.h
	clang++ -std=c++11 Flight/Flight.cpp -c -o Flight.o

clean:
	rm -f flightInfo
	rm -f *.o
	rm -f output.txt