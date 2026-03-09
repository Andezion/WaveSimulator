CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I.
LDFLAGS  = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

TARGET  = WaveSimulator
SRCDIR  = .

# Collect all .cpp files, excluding the build directory
SOURCES := $(shell find $(SRCDIR) -name "*.cpp" -not -path "*/build/*" -not -path "*/.git/*")
OBJECTS := $(SOURCES:.cpp=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	find . -name "*.o" -not -path "*/.git/*" -delete
	rm -f $(TARGET)
