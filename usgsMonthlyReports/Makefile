CXX := clang++
EXE := monthlyReport
CXXFLAGS := -std=c++14

XLNT_INCLUDE := /Users/brandonbarrentine/vcpkg/installed/arm64-osx/include
XLNT_LIB := /Users/brandonbarrentine/vcpkg/installed/arm64-osx/lib

CXXFLAGS += -I$(XLNT_INCLUDE)

SRC := monthlyReport.cpp
OBJS := $(SRC:.cpp=.o)

$(EXE): $(OBJS)
	$(CXX) $(OBJS) -o $(EXE) -L$(XLNT_LIB) -lxlnt

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXE)
	rm -f output.txt