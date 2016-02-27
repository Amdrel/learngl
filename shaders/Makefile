CXX=clang++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic
LDFLAGS=-lm -lGL -lglfw -lGLEW

SOURCES=learngl.cpp shader.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
TARGET=learngl

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY clean:

clean:
	rm -rf $(TARGET) $(OBJECTS)
