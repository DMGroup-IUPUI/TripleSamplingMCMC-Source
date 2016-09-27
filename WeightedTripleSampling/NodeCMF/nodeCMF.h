#include <vector>
#include "graph.h"
using namespace std;

class nodeCMF
{
private:
	graph_ *G_;
	struct neighbor *Neighbors_;
	vector<double> nodeCMF_;
public:
	nodeCMF(graph_ *G_);
	~nodeCMF();
	void sampleTriple(long sampleCount);
	void constructNodeCMF();
	unsigned int sampleNode();
	void sampleTriple(unsigned int v0, graphlet *tgl);
    void printDistribution();
};