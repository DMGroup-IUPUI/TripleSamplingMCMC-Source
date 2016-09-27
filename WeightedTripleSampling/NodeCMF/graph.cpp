#include "graph.h"

graph_::graph_()
{
}

graph_::~graph_()
{
	ADJ.clear();
	ADJIndexStart.clear();
	ADJIndexEnd.clear();
	edgeList.clear();
}

unsigned int graph_::getEdgeCount(){
	return noEdges;
}
unsigned int graph_::getVertexCount(){
	return noVertices;
}

bool graph_::loadgraph(vector<vector<unsigned int> > &tempAdj,unsigned int size){
	ADJ.resize(size);
	unsigned int currentIndex=0;
	noVertices=tempAdj.size();
	noEdges=size/2;
	//cout<<"vertices: "<<noVertices<<" edges:"<<noEdges<<"\n";
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
	//print_ADJ();
    return true;
}

void graph_::print_ADJ(){
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

unsigned long graph_::exactTriSeqM3(){//less efficient (returns 3* triangles)
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
			
			vit=set_intersection(ADJ.begin()+start1,ADJ.begin()+end1,ADJ.begin()+start2,ADJ.begin()+end2,v.begin());
			tri+=vit-v.begin();
			t++;
		}
	}
	v.clear();
	return tri;
}

unsigned long graph_::exactTriSeq(){//more efficient
	unsigned long tri=0;
	unsigned int start1,end1,start2,end2,t;
	vector<unsigned int>::iterator E1,E2;
	v.resize(MaxAdj+MaxAdj);
	for(int i=0;i<noVertices;i++){
		start1=ADJIndexStart[i];
		end1=ADJIndexEnd[i];
		E1=lower_bound (ADJ.begin()+start1, ADJ.begin()+end1, i);
		t=start1;
		while(t<end1 and ADJ[t] < i){ //until this line we just selected an edge (i,ADJ[t])
			start2=ADJIndexStart[ADJ[t]];
			end2=ADJIndexEnd[ADJ[t]];
			E2=lower_bound (ADJ.begin()+start2, ADJ.begin()+end2, ADJ[t]);
			
			vit=set_intersection(ADJ.begin()+start1,E1,ADJ.begin()+start2,E2,v.begin());
			tri+=vit-v.begin();
			t++;
		}
	}
	v.clear();
	//cout<<"in exact tria seq: "<<tri<<"\n";
	return tri;
}

double graph_::exactClusteringCoEff(){
	unsigned long tri=0,path=0;
	unsigned int start1,end1,start2,end2,t;
	vector<unsigned int>::iterator E1,E2;
	v.resize(MaxAdj+MaxAdj);
	//vUnion.resize(MaxAdj+MaxAdj);
	//vDiff.resize(MaxAdj+MaxAdj);
	long triple=0,n;
	for(int i=0;i<noVertices;i++){
		start1=ADJIndexStart[i];
		end1=ADJIndexEnd[i];
		E1=lower_bound (ADJ.begin()+start1, ADJ.begin()+end1, i);
		t=start1;
		n=end1-start1;
		triple+=n*(n-1)/2;
		while(t<end1 and ADJ[t] < i){ //until this line we just selected an edge (i,ADJ[t])
			start2=ADJIndexStart[ADJ[t]];
			end2=ADJIndexEnd[ADJ[t]];
			E2=lower_bound (ADJ.begin()+start2, ADJ.begin()+end2, ADJ[t]);
			
			vit=set_intersection(ADJ.begin()+start1,E1,ADJ.begin()+start2,E2,v.begin());
			//vit=set_intersection(ADJ.begin()+start1,ADJ.begin()+end1,ADJ.begin()+start2,ADJ.begin()+end2,v.begin());
			tri+=vit-v.begin();
			t++;
		}
	}
	
	v.clear();
	//vUnion.clear();
	//vDiff.clear();
	tri*=3;
	path=triple-tri;
	cout<<"triangles * 3: "<<tri<<"\npaths: "<<path<<"\n";
	cout<<"triple : "<<triple<<"\n";
	return (double)tri/((double)tri+(double)path);
}

bool graph_::edge_exists(int a, int b){
	return binary_search(ADJ.begin()+ADJIndexStart[a],ADJ.begin()+ADJIndexEnd[a],b);
}

graphlet* graph_::get_a_initial_graphlet(){//there can be situation when this function can fall in an infinite loop
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

void graph_::populate_neighbors(graphlet* gl, struct neighbor* Neighbors){
	int v0,v1,v2;
	v0=gl->vertices[0];
	v1=gl->vertices[1];
	v2=gl->vertices[2];
	if (edge_exists(gl->vertices[1],gl->vertices[2])){
		Neighbors->neiListEnd[0]=set_union(ADJ.begin()+ADJIndexStart[v1],ADJ.begin()+ADJIndexEnd[v1],ADJ.begin()+ADJIndexStart[v2],ADJ.begin()+ADJIndexEnd[v2],Neighbors->neiList[0].begin());
		Neighbors->listOffset[0]=3;
	}else{
		Neighbors->neiListEnd[0]=set_intersection(ADJ.begin()+ADJIndexStart[v1],ADJ.begin()+ADJIndexEnd[v1],ADJ.begin()+ADJIndexStart[v2],ADJ.begin()+ADJIndexEnd[v2],Neighbors->neiList[0].begin());
		Neighbors->listOffset[0]=1;
	}
	
	if (edge_exists(gl->vertices[0],gl->vertices[2])){
		Neighbors->neiListEnd[1]=set_union(ADJ.begin()+ADJIndexStart[v0],ADJ.begin()+ADJIndexEnd[v0],ADJ.begin()+ADJIndexStart[v2],ADJ.begin()+ADJIndexEnd[v2],Neighbors->neiList[1].begin());
		Neighbors->listOffset[1]=3;
	}else{
		Neighbors->neiListEnd[1]=set_intersection(ADJ.begin()+ADJIndexStart[v0],ADJ.begin()+ADJIndexEnd[v0],ADJ.begin()+ADJIndexStart[v2],ADJ.begin()+ADJIndexEnd[v2],Neighbors->neiList[1].begin());
		Neighbors->listOffset[1]=1;
	}
	
	if (edge_exists(gl->vertices[0],gl->vertices[1])){
		Neighbors->neiListEnd[2]=set_union(ADJ.begin()+ADJIndexStart[v0],ADJ.begin()+ADJIndexEnd[v0],ADJ.begin()+ADJIndexStart[v1],ADJ.begin()+ADJIndexEnd[v1],Neighbors->neiList[2].begin());
		Neighbors->listOffset[2]=3;
	}else{
		Neighbors->neiListEnd[2]=set_intersection(ADJ.begin()+ADJIndexStart[v0],ADJ.begin()+ADJIndexEnd[v0],ADJ.begin()+ADJIndexStart[v1],ADJ.begin()+ADJIndexEnd[v1],Neighbors->neiList[2].begin());
		Neighbors->listOffset[2]=1;
	}
}

bool graph_::triExist(graphlet *gl){
	if(!edge_exists(gl->vertices[0],gl->vertices[1])) return false;
	if(!edge_exists(gl->vertices[0],gl->vertices[2])) return false;
	if(!edge_exists(gl->vertices[1],gl->vertices[2])) return false;
	return true;
}

int graph_::graphletType(graphlet* gl){
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

int graph_::getMaxAdjLen(){
	return MaxAdj;
}


unsigned long graph_::exactConnectedTriple(){
	unsigned int start1,end1,start2,end2,t;
	long triple=0,n;
	for(int i=0;i<noVertices;i++){
		start1=ADJIndexStart[i];
		end1=ADJIndexEnd[i];
		n=end1-start1;
		triple+=n*(n-1)/2;
	}
	v.clear();
	return triple;
}

double graph_::getNodeWeight(unsigned int node, struct neighbor* tempNeighbors)
{
	double w = 0;
	double t = 0;
	graphlet *gl = new graphlet(-1,-2,-3);
	unsigned int s1,s2,e1;
	s1 = ADJIndexStart[node];
	e1 = ADJIndexEnd[node];
	while (s1 < e1) {
		s2=s1+1;
		while (s2 < e1) {
			//<node, s1, s2> is a triple
			gl->graphletReplace(node,ADJ[s1],ADJ[s2]);
			t=getTripleWeight(gl,tempNeighbors);
			w+=t;
			cout << t<<"\t";
			gl->print_graphlet();
			s2++;
		}
		s1++;
	}
	return w;
}

void graph_::sampleTripleFromNode(unsigned int v0, double rand, graphlet *tgl,struct neighbor* tempNeighbors)
{
	double w = 0;
	unsigned int s1,s2,e1;
	s1 = ADJIndexStart[v0];
	e1 = ADJIndexEnd[v0];
	while (s1 < e1) {
		s2=s1+1;
		while (s2 < e1) {
			//<node, s1, s2> is a triple
			tgl->graphletReplace(v0,ADJ[s1],ADJ[s2]);
			w+=getTripleWeight(tgl,tempNeighbors);
			if (w >= rand) {
				return;
			}
			s2++;
		}
		s1++;
	}
}


double graph_::getTripleWeight(graphlet* gl, struct neighbor* tempNeighbors)
{
	unsigned int n,d;
	populate_neighbors(gl, tempNeighbors);
	n = totalNeighbors(tempNeighbors);
	d = totalDegree(gl);
	//cout << d<<"\t"<<n<<"\t:";
	return (double)d/(double)n;
	//return (double)n;
}

unsigned int graph_::totalNeighbors(struct neighbor* tempNeighbors)
{
	unsigned int n =0 ;
	n+=tempNeighbors->neiListEnd[0]-tempNeighbors->neiList[0].begin();
	n+=tempNeighbors->neiListEnd[1]-tempNeighbors->neiList[1].begin();
	n+=tempNeighbors->neiListEnd[2]-tempNeighbors->neiList[2].begin();
	n=n - tempNeighbors->listOffset[0] - tempNeighbors->listOffset[1] - tempNeighbors->listOffset[2];
	return n;
}

unsigned int graph_::totalDegree(graphlet* gl)
{	
	unsigned int d=0;
	int v0,v1,v2;
	
	v0=gl->vertices[0];
	v1=gl->vertices[1];
	v2=gl->vertices[2];
	
	d += (ADJIndexEnd[v0] - ADJIndexStart[v0]); 
	//cout <<"d1: " <<d<<"\t";
	d += (ADJIndexEnd[v1] - ADJIndexStart[v1]); 
	//cout <<"d2: " <<d<<"\t";
	d += (ADJIndexEnd[v2] - ADJIndexStart[v2]); 
	//cout <<"d3: " <<d<<"\t";
	
	if (triExist(gl)) {
		d-=6;
	}else {
		d-=4;
	}
	return d;
}

void graph_::printTriplesOfNode(unsigned int node, struct neighbor* tempNeighbors)//print weights of all triple of a node
{
    double t = 0;
    graphlet *gl = new graphlet(-1,-2,-3);
    unsigned int s1,s2,e1;
    s1 = ADJIndexStart[node];
    e1 = ADJIndexEnd[node];
    while (s1 < e1) {
        s2=s1+1;
        while (s2 < e1) {
            //<node, s1, s2> is a triple
            gl->graphletReplace(node,ADJ[s1],ADJ[s2]);
            t=getTripleWeight(gl,tempNeighbors);
            cout << t<<"\t";
            gl->print_graphlet();
            s2++;
        }
        s1++;
    }
    
}

