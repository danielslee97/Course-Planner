
all: graph.o
	./a.out reqfile.txt courses.txt sample.txt

graph.o: graph
	g++ Graph.o CA5.o -o a.out

graph: Graph.h Graph.cpp
	g++ -c Graph.cpp
	g++ -c CA5.cpp

clean:
	rm *.o a.out



