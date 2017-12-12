# NAMES: Seung Woo Lee, Brandon Donato, Ri Hang Tony Chen
# Specifiy the target
all:    Graph.o

# Specify the object files that the target depends on
# Also specify the object files needed to create the executable

# Specify how the object files should be created from source files
Graph.o:    Graph
	g++ Graph.o CA5.o -o schedule.exe

Graph:  Graph.h Graph.cpp CA5.cpp
	g++ -c CA5.cpp
	g++ -c Graph.cpp

# Specify the object files and executables that are generated
# and need to be removed to re-compile the whole thing
clean:
	rm *.o schedule.exe

