Weighted-Triple-Triple-MCMC.out : Weighted-Triple-Triple-MCMC.cpp  graph.o  tripleMCMC.o random.o graphlet.h inEdgeList.h
	g++ -O3 -I /Users/mmrahman/Research/eclipse_cpp/boost_1_46_1 -g -o Weighted-Triple-Triple-MCMC.out random.o graph.o tripleMCMC.o Weighted-Triple-Triple-MCMC.cpp 

graph.o: graph.h graph.cpp
	g++ -O3 -I /Users/mmrahman/Research/eclipse_cpp/boost_1_46_1 -g -c graph.cpp

tripleMCMC.o: tripleMCMC.h  tripleMCMC.cpp
	g++ -O3 -I /Users/mmrahman/Research/eclipse_cpp/boost_1_46_1 -g -c tripleMCMC.cpp

random.o: random.h  random.cpp
	g++ -O3 -I /Users/mmrahman/Research/eclipse_cpp/boost_1_46_1 -g -c random.cpp


clean:
	rm -f *.o
	rm -f *.out
	rm -f *.gch
	rm -r *.dSYM

