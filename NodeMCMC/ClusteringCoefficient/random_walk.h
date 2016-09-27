//without tuple look up
#include <fstream>
#include <exception>
#include <iostream>
#include <iterator>
#include <set>
#include <map>
#include <string>
#include "random.h"
#include "graphlet.h" 
#include "graph.h"
#define TRI 2
#define PATH 1
using namespace std;

class random_walk
{
private:
	graph_ * g;//graph on which MH will run
	struct neighbor* currentNeighbors,*nextNeighbors; //structures used to store neighbors
public:
	random_walk(graph_ *G)
	{
		g = G;
		currentNeighbors = new neighbor;
		nextNeighbors = new neighbor;
		currentNeighbors->neiList.resize(3);
		currentNeighbors->neiListEnd.resize(3);
		currentNeighbors->listOffset.resize(3);
		currentNeighbors->neiList[0].resize(g->getMaxAdjLen()+g->getMaxAdjLen());
		currentNeighbors->neiList[1].resize(g->getMaxAdjLen()+g->getMaxAdjLen());
		currentNeighbors->neiList[2].resize(g->getMaxAdjLen()+g->getMaxAdjLen());

		nextNeighbors->neiList.resize(3);
		nextNeighbors->neiListEnd.resize(3);
		nextNeighbors->listOffset.resize(3);
		nextNeighbors->neiList[0].resize(g->getMaxAdjLen()+g->getMaxAdjLen());
		nextNeighbors->neiList[1].resize(g->getMaxAdjLen()+g->getMaxAdjLen());
		nextNeighbors->neiList[2].resize(g->getMaxAdjLen()+g->getMaxAdjLen());

	}

	void printtriNotUniform(long sampleCount)//returns row
	{
		long currentSC=0;
		//int foundInSmall=0;
		graphlet *next=new graphlet(1,2,3);
		while(currentSC<sampleCount){
			g->get_next_not_uniform(next);//simple sampler
			if (g->graphletType(next) == TRI or g->graphletType(next) == PATH){
				next->print_graphlet();
				currentSC++;
			}
			else{
				cout<<"error\n";
				exit(1);
			}
		}
		//cout << "triangle sampled: "<<sampleCount<<"\ntriangle found in smaller graph: "<<foundInSmall<<"\n";
		//return (double)foundInSmall/(double)sampleCount;
	}

	void printtriUniform(long sampleCount)//returns row
	{
		long currentSC=0;
		//int foundInSmall=0;

		graphlet *next=new graphlet(1,2,3);
		while(currentSC<sampleCount){
			g->get_next_uniform(next);//not efficient, weighted implimentation
			if (g->graphletType(next) == TRI or g->graphletType(next) == PATH){
				next->print_graphlet();
				currentSC++;
			}
		}
		//cout << "triangle sampled: "<<sampleCount<<"\ntriangle found in smaller graph: "<<foundInSmall<<"\n";
		//return (double)foundInSmall/(double)sampleCount;
	}



	double sampletriNotUniform(int sampleCount, graph_* smallG)//returns row
	{
		int currentSC=0;
		int foundInSmall=0;

		graphlet *next=new graphlet(1,2,3);
		while(currentSC<sampleCount){
			g->get_next_not_uniform(next);//simple sampler
			if (g->graphletType(next) == TRI){
				if (smallG->triExist(next)){
					foundInSmall++;
				}
				currentSC++;
				//cout<<currentSC<<"\n";
			}
		}
		cout << "triangle sampled: "<<sampleCount<<"\ntriangle found in smaller graph: "<<foundInSmall<<"\n";
		return (double)foundInSmall/(double)sampleCount;
	}

	double sampletriUniform(int sampleCount, graph_* smallG)//returns row
	{
		int currentSC=0;
		int foundInSmall=0;

		graphlet *next=new graphlet(1,2,3);
		while(currentSC<sampleCount){
			g->get_next_uniform(next);//not efficient, weighted implimentation
			if (g->graphletType(next) == TRI){
				if (smallG->triExist(next)){
					foundInSmall++;
				}
				currentSC++;
				//cout<<currentSC<<"\n";
			}
		}
		cout << "triangle sampled: "<<sampleCount<<"\ntriangle found in smaller graph: "<<foundInSmall<<"\n";
		return (double)foundInSmall/(double)sampleCount;
	}



	double sampletri(int sampleCount,int walkLength, graph_* smallG)//returns row
	{
		int currentSC=0;
		int currentWL=0;
		int foundInSmall=0;
		struct neighbor *swap;

		graphlet *next;
		graphlet *current;
		graphlet *GLSwap;
		//cout<<"before initial graphlet found\n";
		current = g->get_a_initial_graphlet();//just to give a graphlet data structure
		next = g->get_a_initial_graphlet();
		g->populate_neighbors(next, nextNeighbors);
		//cout<<"initial graphlet found\n";
		//g->populate_neighbors(next, nextNeighbors);
		//cout<<"neighbors populated\n";
		while(currentSC<sampleCount){

			if (currentWL == walkLength){
				delete next;
				next = g->get_a_initial_graphlet();
				g->populate_neighbors(next, nextNeighbors);
				currentWL=0;
			}

			//g->populate_neighbors(next, nextNeighbors);//it will go inside get_next() function

			GLSwap=current;
			current = next;
			next=GLSwap;

			swap = currentNeighbors;
			currentNeighbors = nextNeighbors;
			nextNeighbors = swap;

			get_next(current,next);
			//cout<<"2\n";
			if (g->graphletType(current) == TRI){
				if (smallG->triExist(current)){
					foundInSmall++;
				}
				//triList.push_back(next);
				currentSC++;
			}
			currentWL++;
			/*if (current->graphletid != 2 ){
				//cout<<"test1\n";
				//delete current;
				//cout<<"test2\n";
			}*/
			//cout<<"currentSC: "<<currentSC<<"\n";
		}
		cout << "triangle sampled: "<<sampleCount<<"\ntriangle found in smaller graph: "<<foundInSmall<<"\n";
		return (double)foundInSmall/(double)sampleCount;
	}

	double sampleCoEff(int sampleCount,int walkLength,double exact)//returns row
	{
		int tri=0,path=0,currentSC=0;
		int currentWL=0;

		struct neighbor *swap;

		graphlet *next;
		graphlet *current;
		graphlet *GLSwap;
		//cout<<"before initial graphlet found\n";
		current = g->get_a_initial_graphlet();//just to give a graphlet data structure
		next = g->get_a_initial_graphlet();
		g->populate_neighbors(next, nextNeighbors);

		//cout<<"initial graphlet found\n";
		//g->populate_neighbors(next, nextNeighbors);
		//cout<<"neighbors populated\n";
		while(currentSC<sampleCount){

			if (currentWL == walkLength){
				delete next;
				next = g->get_a_initial_graphlet();
				g->populate_neighbors(next, nextNeighbors);
				currentWL=0;
			}

			//g->populate_neighbors(next, nextNeighbors);

			GLSwap=current;
			current = next;
			next=GLSwap;

			swap = currentNeighbors;
			currentNeighbors = nextNeighbors;
			nextNeighbors = swap;

			get_next(current,next);
			//cout<<"2\n";
			if (g->graphletType(current) == TRI){
				tri+=3;
			}
			else if(g->graphletType(current) == PATH){
				path++;
			}
			else{
				cout<<"error: neither triangle nor path";
			}
			currentSC++;
			currentWL++;
			if (currentSC%1000 == 0){
				double temp=(double)tri/((double)tri+(double)path);
				cout<<currentSC<<": "<<"tri: "<<tri<<"\tpath: "<<path<<"\tclusteringCoEff: "<<temp<<"\terror: "<<(abs(exact-temp)/exact)*100<<"%\n";
			}
		}
		return (double)tri/((double)tri+(double)path);

	}

	void sampleUniformTriple(long sampleCount,int walkLength)//returns row
	{
		long tri=0,path=0,currentSC=0;
		long currentWL=0;

		struct neighbor *swap;

		graphlet *next;
		graphlet *current;
		graphlet *GLSwap;
		//cout<<"before initial graphlet found\n";
		current = g->get_a_initial_graphlet();//just to give a graphlet data structure
		next = g->get_a_initial_graphlet();
		g->populate_neighbors(next, nextNeighbors);

		//cout<<"initial graphlet found\n";
		//g->populate_neighbors(next, nextNeighbors);
		//cout<<"neighbors populated\n";
		while(currentSC<sampleCount){

			if (currentWL == walkLength){
				delete next;
				next = g->get_a_initial_graphlet();
				g->populate_neighbors(next, nextNeighbors);
				currentWL=0;
			}

			//g->populate_neighbors(next, nextNeighbors);

			GLSwap=current;
			current = next;
			next=GLSwap;

			swap = currentNeighbors;
			currentNeighbors = nextNeighbors;
			nextNeighbors = swap;

			get_next(current,next);
			//cout<<"2\n";
			if (g->graphletType(current) == TRI){
				tri++;
				current->print_graphlet();
			}
			else if(g->graphletType(current) == PATH){
				path++;
				current->print_graphlet();
			}
			else{
				cout<<"error: neither triangle nor path";
			}
			currentSC++;
			currentWL++;
			/*if (currentSC%1000 == 0){
				double temp=(double)tri/((double)tri+(double)path);
				cout<<currentSC<<": "<<"tri: "<<tri<<"\tpath: "<<path<<"\tclusteringCoEff: "<<temp<<"\terror: "<<(abs(exact-temp)/exact)*100<<"%\n";
			}*/
		}
		//return (double)tri/((double)tri+(double)path);

	}



	void get_next(graphlet *&current, graphlet*&next)
	{
		int neighborCount,v0,v1,v2;
		v0=currentNeighbors->neiListEnd[0]-currentNeighbors->neiList[0].begin();
		v1=currentNeighbors->neiListEnd[1]-currentNeighbors->neiList[1].begin();
		v2=currentNeighbors->neiListEnd[2]-currentNeighbors->neiList[2].begin();
		neighborCount=v0+v1+v2;
		int neighborCountExact=neighborCount-currentNeighbors->listOffset[0]-currentNeighbors->listOffset[1]-currentNeighbors->listOffset[2];
		if (neighborCountExact==0){
			next->graphletReplace(current->vertices[0],current->vertices[1],current->vertices[2]);
			g->populate_neighbors(next, nextNeighbors);
			return;
		}else if (neighborCountExact<0){
			cout<< "neighbour count error\n";
			exit(1);
		}


		int nextNC=0;
		int i=0;
		int rand;
		while(i<10){//there is a small possibility of dead lock//simple
			i++;
			rand = boost_get_a_random_number(0,neighborCount);
			if (rand < v0){
				if (currentNeighbors->neiList[0][rand]==current->vertices[1] or
						currentNeighbors->neiList[0][rand]==current->vertices[1] or
						currentNeighbors->neiList[0][rand]==current->vertices[2]) continue;
				next->graphletReplace(currentNeighbors->neiList[0][rand],current->vertices[1],current->vertices[2]);
				//return;
			}else if (rand < v0 + v1){
				rand =rand - v0;
				if (currentNeighbors->neiList[1][rand]==current->vertices[0] or
						currentNeighbors->neiList[1][rand]==current->vertices[1] or
						currentNeighbors->neiList[1][rand]==current->vertices[2]) continue;
				next->graphletReplace(currentNeighbors->neiList[1][rand],current->vertices[0],current->vertices[2]);
				//return;
			}else{
				rand =rand - v0 - v1;
				if (currentNeighbors->neiList[2][rand]==current->vertices[2] or
						currentNeighbors->neiList[2][rand]==current->vertices[1] or
						currentNeighbors->neiList[2][rand]==current->vertices[0]) continue;
				next->graphletReplace(currentNeighbors->neiList[2][rand],current->vertices[0],current->vertices[1]);
				//return;
			}
			g->populate_neighbors(next, nextNeighbors);
			nextNC+=nextNeighbors->neiListEnd[0]-nextNeighbors->neiList[0].begin();
			nextNC+=nextNeighbors->neiListEnd[1]-nextNeighbors->neiList[1].begin();
			nextNC+=nextNeighbors->neiListEnd[2]-nextNeighbors->neiList[2].begin();
			nextNC=nextNC - nextNeighbors->listOffset[0] - nextNeighbors->listOffset[1] - nextNeighbors->listOffset[2];
			//cout<<nextNC<<"\n";
			if(nextNC<=neighborCountExact){
				return;
			}else if(boost_get_a_random_number(0,nextNC)< neighborCountExact){
				return;
			}

		}
		next->graphletReplace(current->vertices[0],current->vertices[1],current->vertices[2]);
		//cout<<"Bad random walk situation at get_next()\n";
		g->populate_neighbors(next, nextNeighbors);
		return;
	}
//node based MCMC
	double sampleCoEffNodeMCMC(long sampleCount,long walkLength)//returns row
	{
		long tri=0,path=0,currentSC=0;
		long currentWL=0;

		long currentNode_I,currentNode_D;
		graphlet *gl = new graphlet(1,2,3);
		currentNode_I=g->get_an_initial_node();
		currentNode_D = g->get_adj_size(currentNode_I);

		while(currentSC<sampleCount){

			if (currentWL == walkLength){
				currentNode_I=g->get_an_initial_node();
				currentNode_D = g->get_adj_size(currentNode_I);
				currentWL=0;
			}

			get_next(currentNode_I,currentNode_D);//gets a new node and replaces
			g->get_graphlet(currentNode_I,gl);


			if (g->graphletType(gl) == TRI){
				tri++;
			}
			else if(g->graphletType(gl) == PATH){
				path++;
			}
			else{
				cout<<"error: neither triangle nor path";
			}
			currentSC++;
			currentWL++;
			if (currentSC%1000 == 0){
				double temp=(double)tri/((double)tri+(double)path);
				cout<<currentSC<<": "<<"tri: "<<tri<<"\tpath: "<<path<<"\tclusteringCoEff: "<<temp<<"\n";
			}
		}
		return (double)tri/((double)tri+(double)path);

	}

	double sampleTriangleNodeMCMC(long sampleCount,int walkLength, graph_* smallG)//returns row
	{
		long currentSC=0;
		long currentWL=0;
		long foundInSmall=0;

		long currentNode_I,currentNode_D;
		graphlet *gl = new graphlet(1,2,3);
		currentNode_I=g->get_an_initial_node();
		currentNode_D = g->get_adj_size(currentNode_I);

		while(currentSC<sampleCount){

			if (currentWL == walkLength){
				currentNode_I=g->get_an_initial_node();
				currentNode_D = g->get_adj_size(currentNode_I);
				currentWL=0;
			}

			get_next(currentNode_I,currentNode_D);//gets a new node and replaces
			g->get_graphlet(currentNode_I,gl);


			if (g->graphletType(gl) == TRI){
				if (smallG->triExist(gl)){
					foundInSmall++;
				}
				currentSC++;
			}
			currentWL++;
		}
		cout << "triangle sampled: "<<sampleCount<<"\ntriangle found in smaller graph: "<<foundInSmall<<"\n";
		return (double)foundInSmall/(double)sampleCount;
	}

	void sampleTripleNodeMCMC(long sampleCount,int walkLength)//returns row
	{
		long tri=0,path=0,currentSC=0;
		long currentWL=0;

		long currentNode_I,currentNode_D;

		graphlet *gl = new graphlet(1,2,3);

		currentNode_I=g->get_an_initial_node();
		currentNode_D = g->get_adj_size(currentNode_I);

		while(currentSC<sampleCount){

			if (currentWL == walkLength){
				currentNode_I=g->get_an_initial_node();
				currentNode_D = g->get_adj_size(currentNode_I);
				currentWL=0;
			}

			get_next(currentNode_I,currentNode_D);//gets a new node and replaces

			g->get_graphlet(currentNode_I,gl);

			if (g->graphletType(gl) == TRI){
				tri++;
				cout<<"tri: ";
				gl->print_graphlet();
			}
			else if(g->graphletType(gl) == PATH){
				path++;
				cout<<"path: ";
				gl->print_graphlet();
			}
			else{
				cout<<"error: neither triangle nor path";
			}
			currentSC++;
			currentWL++;
			/*if (currentSC%1000 == 0){
				double temp=(double)tri/((double)tri+(double)path);
				cout<<currentSC<<": "<<"tri: "<<tri<<"\tpath: "<<path<<"\tclusteringCoEff: "<<temp<<"\terror: "<<(abs(exact-temp)/exact)*100<<"%\n";
			}*/
		}
		//return (double)tri/((double)tri+(double)path);

	}

	void sampleNodeNodeMCMC(long sampleCount,int walkLength)//returns row
	{
		long tri=0,path=0,currentSC=0;
		long currentWL=0;

		long currentNode_I,currentNode_D;

		graphlet *gl = new graphlet(1,2,3);

		currentNode_I=g->get_an_initial_node();
		currentNode_D = g->get_adj_size(currentNode_I);

		while(currentSC<sampleCount){

			if (currentWL == walkLength){
				currentNode_I=g->get_an_initial_node();
				currentNode_D = g->get_adj_size(currentNode_I);
				currentWL=0;
			}

			get_next(currentNode_I,currentNode_D);//gets a new node and replaces

			cout<<currentNode_I<<"\n";
			/*g->get_graphlet(currentNode_I,gl);

			if (g->graphletType(gl) == TRI){
				tri++;
				cout<<"tri: ";
				gl->print_graphlet();
			}
			else if(g->graphletType(gl) == PATH){
				path++;
				cout<<"path: ";
				gl->print_graphlet();
			}
			else{
				cout<<"error: neither triangle nor path";
			}*/
			currentSC++;
			currentWL++;
			/*if (currentSC%1000 == 0){
				double temp=(double)tri/((double)tri+(double)path);
				cout<<currentSC<<": "<<"tri: "<<tri<<"\tpath: "<<path<<"\tclusteringCoEff: "<<temp<<"\terror: "<<(abs(exact-temp)/exact)*100<<"%\n";
			}*/
		}
		//return (double)tri/((double)tri+(double)path);

	}

	void get_next(long &currentNode_I,long & currentNode_D){
		long next_I,next_D;
		if (currentNode_D==0) {
			cout<<"error: node degree 0\n";
			exit(1);
		}
		int i=0;
		while(i<10){
			i++;
			next_I=g->get_random_neighbor_vertex(currentNode_I);
			next_D=g->get_adj_size(next_I);
			//next_D=(next_D*(next_D-1))/2;
			if (next_D<=1) continue; //neighbor has adj of size 1

			if (next_D>currentNode_D){
				currentNode_I=next_I;
				currentNode_D=next_D;
				return;
			}else if(boost_get_a_random_number(0,currentNode_D-1) < next_D-1){
				currentNode_I=next_I;
				currentNode_D=next_D;
				return;
			}
			return;
		}
	}

};

