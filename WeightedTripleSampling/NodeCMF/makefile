Weighted-Triple-Node-CMF.out : Weighted-Triple-Node-CMF.cpp  graph.o  nodeCMF.o random.o graphlet.h inEdgeList.h
	g++ -O3 -I /Users/mmrahman/Research/eclipse_cpp/boost_1_46_1 -g -o Weighted-Triple-Node-CMF.out random.o graph.o nodeCMF.o Weighted-Triple-Node-CMF.cpp 

graph.o: graph.h graph.cpp
	g++ -O3 -I /Users/mmrahman/Research/eclipse_cpp/boost_1_46_1 -g -c graph.cpp

nodeCMF.o: nodeCMF.h  nodeCMF.cpp
	g++ -O3 -I /Users/mmrahman/Research/eclipse_cpp/boost_1_46_1 -g -c nodeCMF.cpp

random.o: random.h  random.cpp
	g++ -O3 -I /Users/mmrahman/Research/eclipse_cpp/boost_1_46_1 -g -c random.cpp


clean:
	rm -f *.o
	rm -f *.out
	rm -f *.gch
	rm -r *.dSYM

