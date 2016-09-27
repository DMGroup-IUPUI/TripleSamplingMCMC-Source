#include <vector>
#include "graph.h"
using namespace std;

class nodeMCMCtripleCMF
{
private:
	graph_ *G_;
	struct neighbor *Neighbors_;
	vector<double> nodeWeights_;//not cmf , we save weights explicitly, initialized to -1
public:
	nodeMCMCtripleCMF(graph_ *G_);
	~nodeMCMCtripleCMF();
	void sampleTriple(long sampleCount);
	void initializeNodeWeights();
	unsigned int selectRandomVertex();
	unsigned int sampleNode(unsigned int vc);
	void sampleTriple(unsigned int v0, graphlet *tgl);
	void printDistribution();
};