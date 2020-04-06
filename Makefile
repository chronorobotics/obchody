SOURCES = $(wildcard *.cpp) $(wildcard neural/*.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
DEPS = $(SOURCES:%.cpp=%.d)
TARGET = neural_network

CFLAGS = -std=gnu++14 -O2 -Wall
LFLAGS = -lm -O2

all: $(TARGET)

clean:
	rm $(DEPS) $(OBJECTS) $(TARGET)

.cpp.o:
	g++ -ggdb -MMD -MP $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) -o $(TARGET) $(LFLAGS)

-include $(DEPS)
