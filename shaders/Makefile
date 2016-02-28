CXX=clang++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic
LDFLAGS=-lm -lGLEW

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	LDFLAGS += -lglfw3 -framework OpenGL
else
	LDFLAGS += -lglfw -lGL
endif

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
