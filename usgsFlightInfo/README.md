# USGS Flight Info

# Using The Makefile
to use the Makefile, navigate to the usgsFlightInfo folder.
Use the following command to compile:
```
make
```
Use the following command to clean up object files, output files, and executables:
```
make clean
```
# To Run:
Use the following command in the usgsFlightInfo folder after compilation:
```
./flightInfo
```

# To Change The Schedule:
In the schedule folder, open the desired schedule and change it in the text file and save. the format is:
arrivalCityCode departureCityCode arrivalTime departureTime

Note: you can leave the departure/arrival times blank for a flight and it will ask you for those values during runtime.
