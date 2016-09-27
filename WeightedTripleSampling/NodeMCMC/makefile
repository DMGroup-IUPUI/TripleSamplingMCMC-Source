Weighted-Triple-Node-MCMC.out : Weighted-Triple-Node-MCMC.cpp  graph.o  nodeMCMCtripleCMF.o random.o graphlet.h inEdgeList.h
	g++ -O3 -I /Users/mmrahman/Research/eclipse_cpp/boost_1_46_1 -g -o Weighted-Triple-Node-MCMC.out random.o graph.o nodeMCMCtripleCMF.o Weighted-Triple-Node-MCMC.cpp 

graph.o: graph.h graph.cpp
	g++ -O3 -I /Users/mmrahman/Research/eclipse_cpp/boost_1_46_1 -g -c graph.cpp

nodeMCMCtripleCMF.o: nodeMCMCtripleCMF.h  nodeMCMCtripleCMF.cpp
	g++ -O3 -I /Users/mmrahman/Research/eclipse_cpp/boost_1_46_1 -g -c nodeMCMCtripleCMF.cpp

random.o: random.h  random.cpp
	g++ -O3 -I /Users/mmrahman/Research/eclipse_cpp/boost_1_46_1 -g -c random.cpp


clean:
	rm -f *.o
	rm -f *.out
	rm -f *.gch
	rm -r *.dSYM

