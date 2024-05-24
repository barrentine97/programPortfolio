CXX := clang++
EXE := FlightInfo
CXXFLAGS := -std=c++11

VPATH := Flight
BUILDDIR := Flight

OBJS := FlightInfo.o $(BUILDDIR)/Flight.o

$(EXE): $(OBJS)
	$(CXX) $^ -o $@

$(BUILDDIR)/%.o: $(BUILDDIR)/%.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

clean:
	rm -f $(OBJS) $(EXE)
	rm -f output.txt