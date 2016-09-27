#include "tripleMCMC.h"
#include "random.h"
#include <algorithm>

tripleMCMC::tripleMCMC(graph_ *G)
: G_ (G)
{
	Neighbors_c = new neighbor();
	
	Neighbors_c->neiList.resize(3); //three nodes three neighbor lists
	Neighbors_c->neiListEnd.resize(3); //three iterattor to the end of the list
	Neighbors_c->listOffset.resize(3); //three offset values to account for invalid results
	
	Neighbors_c->neiList[0].resize(G_->getMaxAdjLen()*2); //maximum size of the vector needed. should increase efficiency
	Neighbors_c->neiList[1].resize(G_->getMaxAdjLen()*2);
	Neighbors_c->neiList[2].resize(G_->getMaxAdjLen()*2);
	
	
	Neighbors_n = new neighbor();
	
	Neighbors_n->neiList.resize(3); //three nodes three neighbor lists
	Neighbors_n->neiListEnd.resize(3); //three iterattor to the end of the list
	Neighbors_n->listOffset.resize(3); //three offset values to account for invalid results
	
	Neighbors_n->neiList[0].resize(G_->getMaxAdjLen()*2); //maximum size of the vector needed. should increase efficiency
	Neighbors_n->neiList[1].resize(G_->getMaxAdjLen()*2);
	Neighbors_n->neiList[2].resize(G_->getMaxAdjLen()*2);
	
}

tripleMCMC::~tripleMCMC()
{
	Neighbors_c->neiList[0].clear();
	Neighbors_c->neiList[1].clear();
	Neighbors_c->neiList[2].clear();
	
	Neighbors_c->neiList.clear();
	Neighbors_c->neiListEnd.clear();
	Neighbors_c->listOffset.clear();
	
	delete Neighbors_c;

	
	Neighbors_n->neiList[0].clear();
	Neighbors_n->neiList[1].clear();
	Neighbors_n->neiList[2].clear();
	
	Neighbors_n->neiList.clear();
	Neighbors_n->neiListEnd.clear();
	Neighbors_n->listOffset.clear();
	
	delete Neighbors_n;
	
}

void tripleMCMC::sampleTriple(long sampleCount)//need changes
{
	graphlet * triple_c = new graphlet(-1,-2,-3);
	graphlet * triple_n = new graphlet(-1,-2,-3);
	G_->randomTriple(triple_c); //get a random triple as initial case (DONE)
	//G_->populate_neighbors(triple_c,Neighbors_c);
	
	while (sampleCount > 0) {
		mcmcNextTriple(triple_c,triple_n);//not written (DONE)
		triple_n->print_graphlet();
		//printNeighbors(triple_n,Neighbors_n);
		sampleCount--;
		//current triple will contian the next triple of previous iteration (next triple may contain grabage)
		//cout << "before s: \n";
		//triple_c->print_graphlet();
		//triple_n->print_graphlet();
		//swap_(Neighbors_c,Neighbors_n);//not written (DONE)
		swap_(triple_c,triple_n);//not written (DONE)
		//cout << "after s: \n";
		//triple_c->print_graphlet();
		//triple_n->print_graphlet();
		
	}
	delete triple_c;
	delete triple_n;
}

void tripleMCMC::printNeighbors(graphlet *gl, neighbor *neig)
{
	//G_->populate_neighbors(gl,neig);
	vector<unsigned int>::iterator it;
	cout<<"delete: "<<gl->vertices[0]<<" add: ";
	it=neig->neiList[0].begin();
	while (it != neig->neiListEnd[0]) {
		cout << *(it)<<" ";
		it++;
	}
	cout<<"\n";
	
	cout<<"delete: "<<gl->vertices[1]<<" add: ";
	it=neig->neiList[1].begin();
	while (it != neig->neiListEnd[1]) {
		cout << *(it)<<" ";
		it++;
	}
	cout<<"\n";
	
	cout<<"delete: "<<gl->vertices[2]<<" add: ";
	it=neig->neiList[2].begin();
	while (it != neig->neiListEnd[2]) {
		cout << *(it)<<" ";
		it++;
	}
	cout<<"\n";
	
}

void tripleMCMC::mcmcNextTriple(graphlet *triple_c,graphlet *triple_n)
{
	double wc,wn;
	G_->populate_neighbors(triple_c,Neighbors_c);
	randomTriple(triple_c,Neighbors_c,triple_n);//not written (DONE)
	G_->populate_neighbors(triple_n,Neighbors_n);
	
	
	wc=G_->getTripleWeight(triple_c,Neighbors_c) * G_->totalNeighbors(Neighbors_n);
	wn=G_->getTripleWeight(triple_n,Neighbors_n) * G_->totalNeighbors(Neighbors_c);
	
	if (wn >= wc) {
		return;
	}else {
		double rand = random_uni01();
		if (rand <= (wn/wc)) {
			return;
		}
		else {//next triple will contain the current triple (current triple may contain garbage)
			//swap_(Neighbors_c,Neighbors_n);//not written (DONE)
			//cout << "before swap:\n";
			//triple_c->print_graphlet();
			//triple_n->print_graphlet();

			swap_(triple_c,triple_n);//not written (DONE)
			
			//cout << "after swap:\n";
			//triple_c->print_graphlet();
			//triple_n->print_graphlet();

			return;
		}
	}
}

void tripleMCMC::randomTriple(graphlet *current,neighbor *Neighbors_current,graphlet *next)
{
	unsigned int neighbors_v0, neighbors_v1, neighbors_v2, neighbors_total;
	unsigned int i=0;
	unsigned int rand;
	
	neighbors_v0=Neighbors_current->neiListEnd[0]-Neighbors_current->neiList[0].begin();
	neighbors_v1=Neighbors_current->neiListEnd[1]-Neighbors_current->neiList[1].begin();
	neighbors_v2=Neighbors_current->neiListEnd[2]-Neighbors_current->neiList[2].begin();
	neighbors_total=neighbors_v0+neighbors_v1+neighbors_v2;
	
	while (i<100) {//if ever there is a dead lock situation!
		i++;
		rand = boost_get_a_random_number(0,neighbors_total);
		if (rand < neighbors_v0){
			if (Neighbors_current->neiList[0][rand]==current->vertices[0] or
				Neighbors_current->neiList[0][rand]==current->vertices[1] or
				Neighbors_current->neiList[0][rand]==current->vertices[2])
			{
				continue;
			}
			next->graphletReplace(Neighbors_current->neiList[0][rand],current->vertices[1],current->vertices[2]);
			//cout << "--\n";
			//current->print_graphlet();
			//next->print_graphlet();
			return;
		}else if (rand < neighbors_v0 + neighbors_v1){
			rand =rand - neighbors_v0;
			if (Neighbors_current->neiList[1][rand]==current->vertices[0] or
				Neighbors_current->neiList[1][rand]==current->vertices[1] or
				Neighbors_current->neiList[1][rand]==current->vertices[2]) 
			{
				continue;
			}
			next->graphletReplace(current->vertices[0],Neighbors_current->neiList[1][rand],current->vertices[2]);
			//cout << "--\n";
			//current->print_graphlet();
			//next->print_graphlet();
			return;
		}else{
			rand =rand - neighbors_v0 - neighbors_v1;
			if (Neighbors_current->neiList[2][rand]==current->vertices[2] or
				Neighbors_current->neiList[2][rand]==current->vertices[1] or
				Neighbors_current->neiList[2][rand]==current->vertices[0]) 
			{
				continue;
			}
			next->graphletReplace(current->vertices[0],current->vertices[1],Neighbors_current->neiList[2][rand]);
			//cout << "--\n";
			//current->print_graphlet();
			//next->print_graphlet();
			return;
		}
	}
	//cout << "bad situation\n";
	next->graphletReplace(current->vertices[0],current->vertices[1],current->vertices[2]);
	cout << "--\n";
	current->print_graphlet();
	next->print_graphlet();
	return;
}

void tripleMCMC::swap_(neighbor* &a, neighbor* &b)
{
	Neighbors_swap_temp=a;
	a=b;
	b=Neighbors_swap_temp;
}

void tripleMCMC::swap_(graphlet* &a, graphlet* &b)
{
	triple_swap_temp=a;
	a=b;
	b=triple_swap_temp;
}

void tripleMCMC::printDistribution()
{
	for (int i=0; i<G_->getVertexCount(); ++i) {
		G_->printTriplesOfNode(i,Neighbors_c);
	}
	cout << "--------------\n";
}




























