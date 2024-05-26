CXX := clang++
EXE := usgsFlightInfo
CXXFLAGS := -std=c++11

VPATH := Flight
BUILDDIR := Flight

OBJS := FlightInfo.o $(BUILDDIR)/Flight.o

$(EXE): $(OBJS)
	$(CXX) $^ -o $@

$(BUILDDIR)/%.o: $(BUILDDIR)/%.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $< -c -o $@

clean:
	rm -f $(OBJS) $(EXE)
	rm -f /Users/brandonbarrentine/Desktop/usgsFlightInfo
	rm -f /Users/brandonbarrentine/Desktop/output.txt