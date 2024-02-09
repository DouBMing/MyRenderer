cpp = g++
sources := $(wildcard *.cpp)
objects := $(sources:.cpp=.o)

Renderer: $(objects)
	$(cpp) -o Renderer $(objects)

%.o: %.cpp
	$(cpp) -c $<

clean:
	del /Q *.o $(exe)

.PHONY: all clean