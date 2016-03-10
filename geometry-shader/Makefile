CXX=clang++
CXXFLAGS=-std=c++11 -Wall -Wextra -pedantic
LDFLAGS=-lm -lGLEW -lSOIL

# Tack on platform specific linker flags.
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	# Headers are assumed to be installed through homebrew.
	# NOTE: GLFW is named differently when distributed through homebrew.
	LDFLAGS += -lglfw3 -framework OpenGL
else
	# Assume on an OS that is sane (not windows).
	LDFLAGS += -lglfw -lGL
endif

SOURCES=learngl.cpp shader.cpp perspectivecamera.cpp
OBJECTS=$(SOURCES:%.cpp=%.o)
TARGET=learngl

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY clean:

clean:
	rm -rf $(TARGET) $(OBJECTS)
