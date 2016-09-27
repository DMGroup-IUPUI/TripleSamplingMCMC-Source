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
	vector<int> listOffset;
	//bool valid;
};


class graph_{
private:
	vector<unsigned int> ADJ; //adjacency lists as a single list
	//map<unsigned int,unsigned int> ADJIndexStart; //pointer to the beginning of an adjacency list;
	//map<unsigned int,unsigned int> ADJIndexEnd; //pointer to the end+1 of an adjacency list; Kind of redundant information
	vector<unsigned int> ADJIndexStart;
	vector<unsigned int> ADJIndexEnd;

	vector<pair<unsigned int,unsigned int> > edgeList;//needed only for making threaded triangle counter faster
	//some meta data about the graph
	unsigned int noVertices,noEdges;
	int MaxAdj;//length of longest adj list;

	//Adjacency lists are also sorted based on vertex id.
public:
	~graph_(){
	}
    graph_(){
    }
    graph_(graph_* parent,int percentEdge){
    	vector<vector<unsigned int> > tempAdj;
    	unsigned int AdjSize=parent->copyGraph(tempAdj,percentEdge);
    	loadgraph(tempAdj,AdjSize);
    	for (int i=0;i < tempAdj.size();i++){
    		tempAdj[i].clear();
    	}
    	tempAdj.clear();
    	cout<<"graph reduced \n";
    }

    unsigned int copyGraph(vector<vector<unsigned int> > &tempAdj,int percentEdge){//returns size of adj list
    	tempAdj.resize(ADJIndexStart.size());
    	int rand;
    	int adjsize=0;
    	for(int i=0;i<noVertices;i++){
    		start1=ADJIndexStart[i];
    		end1=ADJIndexEnd[i];
    		unsigned int t=start1;
    		while(t<end1 and ADJ[t] < i){ //until this line we just selected an edge (i,ADJ[t])
    			rand=boost_get_a_random_number(1,101);
    			//cout<<"rand: "<<rand<<"\n";
    			if (rand < percentEdge){
    				tempAdj[i].push_back(ADJ[t]);
    				tempAdj[ADJ[t]].push_back(i);
    				adjsize+=2;
    			}
    			t++;
    		}
    	}
    	return adjsize;
    }

    unsigned int getEdgeCount(){
    	return noEdges;
    }
    unsigned int getVertexCount(){
    	return noVertices;
    }
    bool loadgraph(vector<vector<unsigned int> > &tempAdj,unsigned int size){
    	ADJ.resize(size);
    	unsigned int currentIndex=0;
    	noVertices=tempAdj.size();
    	noEdges=size/2;
    	cout<<"vertices: "<<noVertices<<" edges:"<<noEdges<<"\n";
    	ADJIndexStart.resize(tempAdj.size());
    	ADJIndexEnd.resize(tempAdj.size());
    	MaxAdj=0;
    	for(int i=0;i< tempAdj.size();i++){
    		if (MaxAdj<tempAdj[i].size()){
    			MaxAdj=tempAdj[i].size();
    		}
    		ADJIndexStart[i]=currentIndex;
    		for(int j=0;j<tempAdj[i].size();j++){
    			ADJ[currentIndex]=tempAdj[i][j];
    			currentIndex++;
    		}
    		ADJIndexEnd[i]=currentIndex;
    	}
        return true;
    	//print_ADJ();
    }

    void print_ADJ(){
    	for(int i=0;i<noVertices;i++){
    		unsigned int start,end;
    		start=ADJIndexStart[i];
    		end=ADJIndexEnd[i];
    		while(start<end){
    			cout << ADJ[start++]<<" ";
    		}
    		cout<<"\n";
    	}
    }


private:
    //data structure used for only set intersection at functions exactTriSeqM3()
    vector<unsigned int> v;
    vector<unsigned int>::iterator vit;
    //only for clustering coeff
    vector<unsigned int> vUnion;
    vector<unsigned int>::iterator vitUnion;

    vector<unsigned int> vDiff;
    vector<unsigned int>::iterator vitDiff;


public:
    unsigned long exactTriSeqM3(){
    	unsigned long tri=0;
    	unsigned int start1,end1,start2,end2,t;
    	v.resize(MaxAdj+MaxAdj);
    	for(int i=0;i<noVertices;i++){
    		start1=ADJIndexStart[i];
    		end1=ADJIndexEnd[i];
    		t=start1;
    		while(t<end1 and ADJ[t] < i){ //until this line we just selected an edge (i,ADJ[t])
    			start2=ADJIndexStart[ADJ[t]];
    			end2=ADJIndexEnd[ADJ[t]];
    			/*if(v.size() < ((end1 - start1)+(end2 - start2))){
    				v.resize((end1 - start1)+(end2 - start2));
    			}*/
    			vit=set_intersection(ADJ.begin()+start1,ADJ.begin()+end1,ADJ.begin()+start2,ADJ.begin()+end2,v.begin());
    			tri+=vit-v.begin();
    			t++;
    		}
    	}
    	v.clear();
    	return tri;
    }

    double exactClusteringCoEff(){
    	unsigned long tri=0,path=0;
    	unsigned int start1,end1,start2,end2,t;
    	v.resize(MaxAdj+MaxAdj);
    	//vUnion.resize(MaxAdj+MaxAdj);
    	//vDiff.resize(MaxAdj+MaxAdj);
    	long triple=0,n;
    	for(int i=0;i<noVertices;i++){
    		start1=ADJIndexStart[i];
    		end1=ADJIndexEnd[i];
    		t=start1;
    		n=end1-start1;
    		triple+=n*(n-1)/2;
    		while(t<end1 and ADJ[t] < i){ //until this line we just selected an edge (i,ADJ[t])
    			start2=ADJIndexStart[ADJ[t]];
    			end2=ADJIndexEnd[ADJ[t]];
    			/*if(v.size() < ((end1 - start1)+(end2 - start2))){
    				v.resize((end1 - start1)+(end2 - start2));
    			}*/
    			vit=set_intersection(ADJ.begin()+start1,ADJ.begin()+end1,ADJ.begin()+start2,ADJ.begin()+end2,v.begin());
    			tri+=vit-v.begin();
    			t++;
    		}
    	}

    	v.clear();
    	//vUnion.clear();
    	//vDiff.clear();
    	//tri/=3;
    	path=triple-tri;
    	cout<<"triangles * 3: "<<tri<<"\npaths: "<<path<<"\n";
    	cout<<"triple : "<<triple<<"\n";
    	return (double)tri/((double)tri+(double)path);
    }

    double exactConnectedTriple(){
    	unsigned long tri=0,path=0;
    	unsigned int start1,end1,start2,end2,t;
    	v.resize(MaxAdj+MaxAdj);
    	//vUnion.resize(MaxAdj+MaxAdj);
    	//vDiff.resize(MaxAdj+MaxAdj);
    	long triple=0,n;
    	for(int i=0;i<noVertices;i++){
    		start1=ADJIndexStart[i];
    		end1=ADJIndexEnd[i];
    		t=start1;

    		n=end1-start1;
    		triple+=n*(n-1)/2;

    		while(t<end1 and ADJ[t] < i){ //until this line we just selected an edge (i,ADJ[t])
    			start2=ADJIndexStart[ADJ[t]];
    			end2=ADJIndexEnd[ADJ[t]];

    			vit=set_intersection(ADJ.begin()+start1,ADJ.begin()+end1,ADJ.begin()+start2,ADJ.begin()+end2,v.begin());
    			tri+=vit-v.begin();
    			t++;
    			//vitUnion=set_union(ADJ.begin()+start1,ADJ.begin()+end1,ADJ.begin()+start2,ADJ.begin()+end2,vUnion.begin());
    			//vitDiff=set_difference(vUnion.begin(),vitUnion,v.begin(),vit,vDiff.begin());
    			//path+=vitDiff-vDiff.begin()-2;
    		}
    	}
    	v.clear();
    	//vUnion.clear();
    	//vDiff.clear();
    	//tri/=3;
    	path=triple-tri;
    	//tri/=3;
    	//cout<<"triangles: "<<tri<<"\npaths: "<<path<<"\n";
    	return tri+path;
    }



    unsigned long exactTriEdge(unsigned int a,unsigned int b,vector<unsigned int> &vt){
    	//can not use global variable here as it hampers parallelism
    	//a vector from caller is passed as reference, for speed up.
    	vector<unsigned int>::iterator it;
    	unsigned int start1,end1,start2,end2;
    	start1=ADJIndexStart[a];
    	end1=ADJIndexEnd[a];
    	start2=ADJIndexStart[b];
    	end2=ADJIndexEnd[b];

   		if(vt.size() < ((end1 - start1)+(end2 - start2))){
   			vt.resize((end1 - start1)+(end2 - start2));
    	}
    	it=set_intersection(ADJ.begin()+start1,ADJ.begin()+end1,ADJ.begin()+start2,ADJ.begin()+end2,vt.begin());
   		return it-vt.begin();
    }
private:
    //data structure used for only functions get_edges
    //parallel execution
    unsigned int v1,v2;
    unsigned int start1,end1;
public:
    void initiate_parellel_computation(){
    	v1=0;
    	v2=0;
    }
    int get_edges(vector<pair<unsigned int,unsigned int> >& edgeList, int maxEdges){
    	int t=0;
    	while (v1<noVertices){
    		start1=ADJIndexStart[v1];
    		end1=ADJIndexEnd[v1];
       		if(v2<start1 or v2 >end1){
       			v2=start1;
       		}
       		while(v2<end1 and ADJ[v2] < v1){ //until this line we just selected an edge (i,ADJ[t])
       			edgeList[t] = pair<unsigned int,unsigned int>(v1,ADJ[v2]);
       			t++;
       			v2++;
       			if (t==maxEdges) return maxEdges;
    		}
    		v1++;
    	}
    	return t;
    }
private:
    //data structure used for only function get_edgeUsingEdgeList
    //parallel execution
    unsigned int edgeListCurrentIndex;
    unsigned int totalWorkLoad; //no of total edges to be sampled
public:
    void initiate_parellel_computationEL(float sf){//uses edgeList
    	edgeListCurrentIndex=0;
    	loadEdgeList();
    	totalWorkLoad=sf*edgeList.size();
    	if (totalWorkLoad>edgeList.size()){
    		totalWorkLoad=edgeList.size();
    	}
    	randomizeEdgeList();
    }
    int get_edgesEL(int &begin, int maxEdges){//uses edge list
    	//returns end of the allotment
    	if(edgeListCurrentIndex==totalWorkLoad){
    		return -1;
    	}
    	begin=edgeListCurrentIndex;
    	if (edgeListCurrentIndex+maxEdges <= totalWorkLoad){
    		edgeListCurrentIndex+=maxEdges;
    		return edgeListCurrentIndex;
    	}
    	edgeListCurrentIndex=totalWorkLoad;
    	return totalWorkLoad;
    }

    void loadEdgeList(){//call this function before starting threaded approximation using edgeList
    	unsigned int start1,end1,t,ep=0;
    	edgeList.resize(noEdges);
    	for(int i=0;i<noVertices;i++){
    		start1=ADJIndexStart[i];
    		end1=ADJIndexEnd[i];
    		while(start1<end1 and ADJ[start1] < i){ //until this line we just selected an edge (i,ADJ[t])
    			edgeList[ep++]=pair<unsigned int,unsigned int>(i,ADJ[start1]);
    			start1++;
    		}
    	}
    }

    void randomizeEdgeList(){
    	unsigned int rand;
    	pair<unsigned int,unsigned int> swapT;
    	for(unsigned int i=0;i<totalWorkLoad;i++){
    		rand=boost_get_a_random_number(i,edgeList.size());
    		swapT=edgeList[i];
    		edgeList[i]=edgeList[rand];
    		edgeList[rand]=swapT;
    	}
    }

    unsigned long triCountEdgeList(unsigned int begin,unsigned int end,vector<unsigned int> &vt){
    	unsigned long tri=0;
    	for(unsigned int i=begin;i<end;i++){
    		tri+=exactTriEdge(edgeList[i].first,edgeList[i].second,vt);
    	}
    	return tri;
    }
    //additional functions for sample triples
    bool edge_exists(int a, int b){
    	return binary_search(ADJ.begin()+ADJIndexStart[a],ADJ.begin()+ADJIndexEnd[a],b);
    }

    graphlet* get_a_initial_graphlet(){//there can be situation when this function can fall in an infinite loop
    	int v1,v2,v3;
    	while(1){
    		v1=boost_get_a_random_number(0,noVertices);
    		if (ADJIndexStart[v1] >= ADJIndexEnd[v1]) continue;
    		//v1 found
    		v2=boost_get_a_random_number(ADJIndexStart[v1],ADJIndexEnd[v1]);
    		v2=ADJ[v2];
    		if(ADJIndexStart[v2] >= ADJIndexEnd[v2] or v1 == v2) continue;
    		//v2 found
    		v3=boost_get_a_random_number(ADJIndexStart[v2],ADJIndexEnd[v2]);
    		v3=ADJ[v3];
    		if(ADJIndexStart[v3] >= ADJIndexEnd[v3] or v1 == v3 or v2 == v3) continue;
    		break;
    	}

    	return new graphlet(v1,v2,v3);
    }

    void populate_neighbors(graphlet* gl, struct neighbor* Neighbors){
    	int v0,v1,v2;
    	v0=gl->vertices[0];
    	v1=gl->vertices[1];
    	v2=gl->vertices[2];
    	//deletinf vertex 0
    	/*if(Neighbors->neiList[0].size()<(ADJIndexEnd[v1]-ADJIndexStart[v1]+ADJIndexEnd[v2]-ADJIndexStart[v2])){
    		Neighbors->neiList[0].resize(ADJIndexEnd[v1]-ADJIndexStart[v1]+ADJIndexEnd[v2]-ADJIndexStart[v2]);
    	}*/
    	if (edge_exists(gl->vertices[1],gl->vertices[2])){
    		Neighbors->neiListEnd[0]=set_union(ADJ.begin()+ADJIndexStart[v1],ADJ.begin()+ADJIndexEnd[v1],ADJ.begin()+ADJIndexStart[v2],ADJ.begin()+ADJIndexEnd[v2],Neighbors->neiList[0].begin());
    		Neighbors->listOffset[0]=3;
    	}else{
    		Neighbors->neiListEnd[0]=set_intersection(ADJ.begin()+ADJIndexStart[v1],ADJ.begin()+ADJIndexEnd[v1],ADJ.begin()+ADJIndexStart[v2],ADJ.begin()+ADJIndexEnd[v2],Neighbors->neiList[0].begin());
    		Neighbors->listOffset[0]=1;
    	}

    	//deletinf vertex 1
    	/*if(Neighbors->neiList[1].size()<(ADJIndexEnd[v0]-ADJIndexStart[v0]+ADJIndexEnd[v2]-ADJIndexStart[v2])){
    		Neighbors->neiList[1].resize(ADJIndexEnd[v0]-ADJIndexStart[v0]+ADJIndexEnd[v2]-ADJIndexStart[v2]);
    	}*/
    	if (edge_exists(gl->vertices[0],gl->vertices[2])){
    		Neighbors->neiListEnd[1]=set_union(ADJ.begin()+ADJIndexStart[v0],ADJ.begin()+ADJIndexEnd[v0],ADJ.begin()+ADJIndexStart[v2],ADJ.begin()+ADJIndexEnd[v2],Neighbors->neiList[1].begin());
    		Neighbors->listOffset[1]=3;
    	}else{
    		Neighbors->neiListEnd[1]=set_intersection(ADJ.begin()+ADJIndexStart[v0],ADJ.begin()+ADJIndexEnd[v0],ADJ.begin()+ADJIndexStart[v2],ADJ.begin()+ADJIndexEnd[v2],Neighbors->neiList[1].begin());
    		Neighbors->listOffset[1]=1;
    	}

    	//deletinf vertex 2
    	/*if(Neighbors->neiList[2].size()<(ADJIndexEnd[v0]-ADJIndexStart[v0]+ADJIndexEnd[v1]-ADJIndexStart[v1])){
    		Neighbors->neiList[2].resize(ADJIndexEnd[v0]-ADJIndexStart[v0]+ADJIndexEnd[v1]-ADJIndexStart[v1]);
    	}*/
    	if (edge_exists(gl->vertices[0],gl->vertices[1])){
    		Neighbors->neiListEnd[2]=set_union(ADJ.begin()+ADJIndexStart[v0],ADJ.begin()+ADJIndexEnd[v0],ADJ.begin()+ADJIndexStart[v1],ADJ.begin()+ADJIndexEnd[v1],Neighbors->neiList[2].begin());
    		Neighbors->listOffset[2]=3;
    	}else{
    		Neighbors->neiListEnd[2]=set_intersection(ADJ.begin()+ADJIndexStart[v0],ADJ.begin()+ADJIndexEnd[v0],ADJ.begin()+ADJIndexStart[v1],ADJ.begin()+ADJIndexEnd[v1],Neighbors->neiList[2].begin());
    		Neighbors->listOffset[2]=1;
    	}
    }
    bool triExist(graphlet *gl){
    	if(!edge_exists(gl->vertices[0],gl->vertices[1])) return false;
    	if(!edge_exists(gl->vertices[0],gl->vertices[2])) return false;
    	if(!edge_exists(gl->vertices[1],gl->vertices[2])) return false;
    	return true;
    }
    int graphletType(graphlet* gl){
    	int te=0;
    	if(edge_exists(gl->vertices[0],gl->vertices[1])) te++;
    	if(edge_exists(gl->vertices[0],gl->vertices[2])) te++;
    	if(edge_exists(gl->vertices[1],gl->vertices[2])) te++;
    	if (te==2) return PATH;
    	if (te==3) return TRI;
    	cout<<"("<<gl->vertices[0]<<","<<gl->vertices[1]<<","<<gl->vertices[2]<<")\n";
    	cout<<"error triple is neither a path nor a triangle\n";
    	exit(1);
    }
    int getMaxAdjLen(){
    	return MaxAdj;
    }

public:
    //VLDB 2013
    //LoadEdgeList
    vector<int> edgeTripleCount;
    vector<int> edgeTripleCountCumu;

    void initiateSimplesampling(){
    	loadEdgeList();
    	vUnion.resize(MaxAdj*2);
    }

    void initiateWeightedSampling(){
       	loadEdgeList();
       	edgeTripleCount.resize(noEdges);
       	edgeTripleCountCumu.resize(noEdges);
       	edgeTripleCountCumu[0]=0;
       	vUnion.resize(MaxAdj*2);
       	int v1,v2;
       	vector<unsigned int>::iterator s1,s2,e1,e2;
       	for(int i=0;i<edgeList.size();i++){
       		v1=edgeList[i].first;
    		v2=edgeList[i].second;
    		s1=ADJ.begin()+ADJIndexStart[v1];
    		s2=ADJ.begin()+ADJIndexStart[v2];
    		e1=ADJ.begin()+ADJIndexEnd[v1];
    		e2=ADJ.begin()+ADJIndexEnd[v2];
    		vitUnion=set_union(s1,e1,s2,e2,vUnion.begin());
    		edgeTripleCount[i]=vitUnion-vUnion.begin() -2;
       	}
       	edgeTripleCountCumu[0]=edgeTripleCount[0];
       	for(int i=1;i<edgeTripleCountCumu.size();i++){
       		edgeTripleCountCumu[i]=edgeTripleCountCumu[i-1]+edgeTripleCount[i];
       	}

    }

    void print_edges(){
    	int t=0;
    	while(t<edgeList.size()){
    		cout<<"("<<edgeList[t].first<<","<<edgeList[t].second<<")\n";
    		t++;
    	}
    }

    void get_next_not_uniform(graphlet*&next){
    	//get a random edge
    	int t;
    	int v1,v2,v3;
    	vector<unsigned int>::iterator s1,s2,e1,e2;

    	while(1){
    		t = boost_get_a_random_number(0,edgeList.size());

    		v1=edgeList[t].first;
    		v2=edgeList[t].second;
    		s1=ADJ.begin()+ADJIndexStart[v1];
    		s2=ADJ.begin()+ADJIndexStart[v2];
    		e1=ADJ.begin()+ADJIndexEnd[v1];
    		e2=ADJ.begin()+ADJIndexEnd[v2];
    		vitUnion=set_union(s1,e1,s2,e2,vUnion.begin());

    		v3=boost_get_a_random_number(0,vitUnion-vUnion.begin());
    		//cout<<vitUnion-vUnion.begin()<<"\n";
    		v3=vUnion[v3];
    		if(v1==v3 or v2==v3) {
    			//cout<<v1<<","<<v2<<","<<v3<<"\n";
    			continue;
    		}
    		next->graphletReplace(v1,v2,v3);
    		return;
    	}
    }

    void get_next_uniform(graphlet*&next){
    	//get a random edge
    	int t;
    	int v1,v2,v3;
    	vector<unsigned int>::iterator s1,s2,e1,e2;

    	while(1){
    		t = boost_get_a_random_number(0,edgeTripleCountCumu[edgeTripleCountCumu.size()-1]);
    		vector<int>::iterator tit=lower_bound(edgeTripleCountCumu.begin(),edgeTripleCountCumu.end(),t);
    		t=int(tit-edgeTripleCountCumu.begin());
    		v1=edgeList[t].first;
    		v2=edgeList[t].second;
    		s1=ADJ.begin()+ADJIndexStart[v1];
    		s2=ADJ.begin()+ADJIndexStart[v2];
    		e1=ADJ.begin()+ADJIndexEnd[v1];
    		e2=ADJ.begin()+ADJIndexEnd[v2];
    		vitUnion=set_union(s1,e1,s2,e2,vUnion.begin());

    		v3=boost_get_a_random_number(0,vitUnion-vUnion.begin());
    		//cout<<vitUnion-vUnion.begin()<<"\n";
    		v3=vUnion[v3];
    		if(v1==v3 or v2==v3) {
    			//cout<<v1<<","<<v2<<","<<v3<<"\n";
    			continue;
    		}
    		next->graphletReplace(v1,v2,v3);
    		return;
    	}
    }
//node based MCMC;
    unsigned int get_an_initial_node(){
    	unsigned int v1;
    	while(1){
    		v1=boost_get_a_random_number(0,ADJIndexStart.size());
    		if(ADJIndexEnd[v1]-ADJIndexStart[v1] >= 2){
    			return v1;
    		}
    	}
    }

    long get_adj_size (int v){
    	if (v>=ADJIndexStart.size()){
    		cout<<"error: adj list out of bound\n";
    		exit(1);
    	}
    	return long(ADJIndexEnd[v]-ADJIndexStart[v]);
    }

    void get_graphlet(int NodeID,graphlet*& gl){
    	int v1,v2;
    	v1=boost_get_a_random_number(ADJIndexStart[NodeID],ADJIndexEnd[NodeID]);
    	while((v2=boost_get_a_random_number(ADJIndexStart[NodeID],ADJIndexEnd[NodeID]))==v1){}//there can be infinite loop
    	//cout<<NodeID<<"," << ADJ[v1]<<","<<ADJ[v2]<<"\n";
    	gl->graphletReplace(NodeID,ADJ[v1],ADJ[v2]);
    }

    unsigned int get_random_neighbor_vertex(int current){
    	int t=boost_get_a_random_number(ADJIndexStart[current],ADJIndexEnd[current]);
    	//cout<<current<<","<<ADJ[t]<<'\n';
    	return ADJ[t];
    }

    void printNodeTripleCount(){
    	int node;
    	int tri;
    	for(node=0;node < ADJIndexStart.size();node++){
    		tri=ADJIndexEnd[node]-ADJIndexStart[node];
    		tri=(tri*(tri-1))/2;
    		if(tri==0) continue;
    		cout<<tri<<"\t"<<node<<"\n";
    	}
    }


};
#endif
