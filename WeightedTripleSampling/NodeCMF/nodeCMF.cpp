#include "nodeCMF.h"
#include "random.h"

nodeCMF::nodeCMF(graph_ *G)
: G_ (G)
{
	Neighbors_ = new neighbor;
	
	Neighbors_->neiList.resize(3); //three nodes three neighbor lists
	Neighbors_->neiListEnd.resize(3); //three iterattor to the end of the list
	Neighbors_->listOffset.resize(3); //three offset values to account for invalid results
	
	Neighbors_->neiList[0].resize(G_->getMaxAdjLen()*2); //maximum size of the vector needed. should increase efficiency
	Neighbors_->neiList[1].resize(G_->getMaxAdjLen()*2);
	Neighbors_->neiList[2].resize(G_->getMaxAdjLen()*2);
}

nodeCMF::~nodeCMF()
{
	Neighbors_->neiList[0].clear();
	Neighbors_->neiList[1].clear();
	Neighbors_->neiList[2].clear();
	
	Neighbors_->neiList.clear();
	Neighbors_->neiListEnd.clear();
	Neighbors_->listOffset.clear();
	
	delete Neighbors_;
}

void nodeCMF::sampleTriple(long sampleCount)//not finished
{
    cout<<"Original weights of triples:\n";
	constructNodeCMF();
    cout<<"Sample triples:\n";
	unsigned int v0;
	graphlet * tgl = new graphlet(-1,-2,-3);
	while (sampleCount > 0) {
		v0=sampleNode();
		sampleTriple(v0,tgl);
		tgl->print_graphlet();
		sampleCount--;
	}
	delete tgl;
}

void nodeCMF::constructNodeCMF()
{
	nodeCMF_.resize(G_->getVertexCount());
	
	nodeCMF_[0] = G_->getNodeWeight(0,Neighbors_);
	for (int i=1; i<nodeCMF_.size(); ++i) {
		nodeCMF_[i] = nodeCMF_[i-1] + G_->getNodeWeight(i,Neighbors_);
	}
	cout << "--------------\n";

}

unsigned int nodeCMF::sampleNode()
{
	double rand = random_uni01() * nodeCMF_[nodeCMF_.size()-1];
	vector<double>::iterator node = lower_bound (nodeCMF_.begin(),nodeCMF_.end(),rand);
	return node-nodeCMF_.begin();
}

void nodeCMF::sampleTriple(unsigned int v0, graphlet *tgl)
{
	double nodeweight = nodeCMF_[v0];
	if (v0!=0) {
		nodeweight -= nodeCMF_[v0-1];
	}
	double rand = random_uni01() * nodeweight;
	
	G_->sampleTripleFromNode(v0, rand, tgl,Neighbors_);
}

void nodeCMF::printDistribution()
{
    for (int i=0; i<G_->getVertexCount(); ++i) {
        G_->printTriplesOfNode(i,Neighbors_	);
    }
    cout << "--------------\n";
}





























