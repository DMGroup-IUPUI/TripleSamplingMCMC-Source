#include "nodeMCMCtripleCMF.h"
#include "random.h"

nodeMCMCtripleCMF::nodeMCMCtripleCMF(graph_ *G)
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

nodeMCMCtripleCMF::~nodeMCMCtripleCMF()
{
	Neighbors_->neiList[0].clear();
	Neighbors_->neiList[1].clear();
	Neighbors_->neiList[2].clear();
	
	Neighbors_->neiList.clear();
	Neighbors_->neiListEnd.clear();
	Neighbors_->listOffset.clear();
	
	delete Neighbors_;
	nodeWeights_.clear();
}

void nodeMCMCtripleCMF::sampleTriple(long sampleCount)//need changes
{
	initializeNodeWeights();//initiallize all node weights to -1
	
	unsigned int vc,vn;//current vertex and next vertex
	vc = selectRandomVertex();
	graphlet * tgl = new graphlet(-1,-2,-3);
	while (sampleCount > 0) {
		vn=sampleNode(vc);
		sampleTriple(vn,tgl);
		tgl->print_graphlet();
		sampleCount--;
		vc=vn;
	}
	delete tgl;
}

unsigned int nodeMCMCtripleCMF::selectRandomVertex()
{
	return boost_get_a_random_number(0, G_->getVertexCount());
}

void nodeMCMCtripleCMF::initializeNodeWeights()
{
	nodeWeights_.resize(G_->getVertexCount());
	
	for (int i=0; i<nodeWeights_.size(); ++i) {
		nodeWeights_[i] = -1;
	}
	//cout << "--------------\n";
}

void nodeMCMCtripleCMF::printDistribution()
{
	for (int i=0; i<G_->getVertexCount(); ++i) {
		G_->printTriplesOfNode(i,Neighbors_	);
	}
	cout << "--------------\n";
}


unsigned int nodeMCMCtripleCMF::sampleNode(unsigned int vc)
{
	unsigned int vn;
	vn = G_->randAdjNode(vc);//not written
	if (nodeWeights_[vc] == -1) {
		nodeWeights_[vc] = G_->getNodeWeight(vc, Neighbors_) * (double)G_->nodeDegree(vn);
	}
	if (nodeWeights_[vn] == -1) {
		nodeWeights_[vn] = G_->getNodeWeight(vn, Neighbors_) * (double)G_->nodeDegree(vc);
	}
	
	if (nodeWeights_[vn] >= nodeWeights_[vc]){//this condition is used for only efficiency
		return vn;
	}else {
		double rand = random_uni01();
		//cout <<"acceptance probability: "<< (nodeWeights_[vn]/nodeWeights_[vc])<<"\n";
		if (rand <= (nodeWeights_[vn]/nodeWeights_[vc]) ) {
			return vn;
		}
		else {
			return vc;
		}

	}
}

void nodeMCMCtripleCMF::sampleTriple(unsigned int v0, graphlet *tgl)
{
	double rand = random_uni01() * nodeWeights_[v0];
	//cout << nodeWeights_[v0] <<"\t"<<rand<<"\n";
	G_->sampleTripleFromNode(v0, rand, tgl,Neighbors_);
}































