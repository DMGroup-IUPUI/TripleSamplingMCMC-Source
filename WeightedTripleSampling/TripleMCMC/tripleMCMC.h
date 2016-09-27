#include <vector>
#include "graph.h"
using namespace std;

class tripleMCMC
{
private:
	graph_ *G_;
	neighbor * Neighbors_c;//neighbors of current triple
	neighbor * Neighbors_n;//neighbors of next triple (for improving efficiency we have two structures)
	neighbor * Neighbors_swap_temp;
	graphlet * triple_swap_temp;
public:
	tripleMCMC(graph_ *G_);
	~tripleMCMC();
	void sampleTriple(long sampleCount);
	void mcmcNextTriple(graphlet *triple_c,graphlet *triple_n);
	void randomTriple(graphlet *current,neighbor *Neighbors_current,graphlet *next);
	void printDistribution();
	void printNeighbors(graphlet *gl, neighbor *neig);
	
	void swap_( neighbor *&a,  neighbor *&b);
	void swap_(graphlet *&a, graphlet *&b);
	
};