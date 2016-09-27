Uniform-Triangle-Samples-Node-MCMC.out : Uniform-Triangle-Samples-Node-MCMC.cpp  graph.h  random_walk.h random.o graphlet.h inEdgeList.h
	g++ -O3 -g -o Uniform-Triangle-Samples-Node-MCMC.out random.o Uniform-Triangle-Samples-Node-MCMC.cpp 

random.o: random.h  random.cpp
	g++ -O3 -g -c random.cpp


clean:
	rm -f *.o
	rm -f *.out
	rm -f *.gch
	rm -r *.dSYM

