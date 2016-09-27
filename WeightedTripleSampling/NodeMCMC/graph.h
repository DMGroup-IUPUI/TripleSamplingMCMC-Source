#ifndef GRAPH_H_
#define GRAPH_H_
#include <algorithm>
#include <map>
#include "random.h"
#include "graphlet.h"
using namespace std;
#define TRI 2
#define PATH 1
struct neighbor{//used in random_walk.h
	vector<vector<unsigned int> > neiList;
	vector<vector<unsigned int>::iterator> neiListEnd; 
	vector<int> listOffset; //nodes which are invalid must be discarder from calculation
	//bool valid;
};


class graph_{
private:
	vector<unsigned int> ADJ; //adjacency lists as a single list
	vector<unsigned int> ADJIndexStart;
	vector<unsigned int> ADJIndexEnd;

	vector<pair<unsigned int,unsigned int> > edgeList;//needed only for making threaded triangle counter faster
	//some meta data about the graph
	unsigned int noVertices,noEdges;
	int MaxAdj;//length of longest adj list; calculated inside loadgraph()
	//Adjacency lists are also sorted based on vertex id.


    
	//data structure used for only set intersection at functions exactTriSeqM3()
    vector<unsigned int> v;
    vector<unsigned int>::iterator vit;
	

public:
	~graph_();
    graph_();
    unsigned int getEdgeCount();
    unsigned int getVertexCount();
	bool loadgraph(vector<vector<unsigned int> > &tempAdj,unsigned int size);//called from an input object with adjacency list and size
    void print_ADJ();
	//struct neighbor* tempNeighbors is given by the caller , caller allocates memory and deallocation is done by caller too.
	double getNodeWeight(unsigned int node, struct neighbor* tempNeighbors); //weight of a node
	void printTriplesOfNode(unsigned int node, struct neighbor* tempNeighbors);//print weights of all triple of a node
	double getTripleWeight(graphlet* gl, struct neighbor* tempNeighbors);//weight of a triple
	unsigned int totalNeighbors(struct neighbor* tempNeighbors);//total neighbors of a triple
	unsigned int totalDegree(graphlet* gl);//total degree of a triple
	void sampleTripleFromNode(unsigned int v0, double rand, graphlet *tgl, struct neighbor* tempNeighbors);
	unsigned int randAdjNode(unsigned int vc);
	unsigned int nodeDegree(unsigned int node);

public:
    unsigned long exactTriSeqM3();//less efficient (returns 3* triangles)
	unsigned long exactTriSeq();//more efficient
 
    double exactClusteringCoEff();
    unsigned long exactConnectedTriple();

    //additional functions for sample triples
    bool edge_exists(int a, int b);
	graphlet* get_a_initial_graphlet();//there can be situation when this function can fall in an infinite loop
    
    void populate_neighbors(graphlet* gl, struct neighbor* Neighbors);
	
    bool triExist(graphlet *gl);//returns true if gl is a triangle;
    int graphletType(graphlet* gl);
    int getMaxAdjLen();


};
#endif
