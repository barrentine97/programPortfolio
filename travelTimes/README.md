# Travel Times
## Using the Makefile
to use the Makefile, navigate to the travelTimes folder.
Use the following command to compile:
```
make
```
Use the following command to remove the executable.
```
make clean
```

## To Run:
Use the following command in the travelTimes folder after compilation:
```
./travelTimes
```

## Using travelTimes.txt
This program reads the information given in the travelTimes.txt file.
Each line in this file represents an employee and the information given after the colon represents the places traveled to by that employee.

Below is an example of how the travelTimes.txt document should be formatted:
```
Name1: City1(d), City2(d), City3(d),...
Name2: City1(d), City2(d), City3(d),...
```
where the cities are given as a comma-separated list and the d is an optional tag at the end of each city to represent mileage for that city.
