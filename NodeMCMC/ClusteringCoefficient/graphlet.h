#ifndef GRAPHLET_H_
#define GRAPHLET_H_
#include <sstream>
#include <iostream>
//#include "graph.h"
#define TRI 2
#define PATH 1
using namespace std;

class graphlet
{
public:
	int * vertices;
private:
	//bool * edges;
	//int graphletType;
	//void* g;
public:
	~graphlet(){
		delete vertices;
		//delete edges;
	}

	graphlet(int X,int Y,int Z)
	{
		//may be deleted once we are sure
		if(X==Y or Y==Z or Z==X){
			cout<<X<<","<<Y<<","<<Z<<"\n";
			cout<<"triangle parameter is wrong!!!\n";
			exit(1);
		}
		//END:may be deleted once we are sure
		//g=g1;
		vertices = new int[3];
		vertices[0] = X;
		vertices[1] = Y;
		vertices[2] = Z;
		sort(vertices,vertices+3);
		//edges = new bool[3];
		//edges[0]=(graph_*)g->edge_exists(vertices[0],vertices[1]);
		//edges[1]=g->edge_exists(vertices[0],vertices[2]);
		//edges[2]=g->edge_exists(vertices[1],vertices[2]);

		//int te=0;
		//for (int i=0;i<3;i++){
		//	if (edges[i]==true){
		//		te++;
		//	}
		//}

		//if (te == 2){
		//	graphType=PATH;
		//}else if (te == 3){
		//	graphType=TRI;
		//}else{
		//	cout<<"error graphlet not triangle or path\n";
		//	exit(1);
		//}

	}

	void graphletReplace(int X,int Y,int Z)//reuse the data structure
	{
		//may be deleted once we are sure
		/*if(X==Y or Y==Z or Z==X){
			cout<<X<<","<<Y<<","<<Z<<"\n";
			cout<<"triangle parameter is wrong!!!\n";
			exit(1);
		}*/
		//END:may be deleted once we are sure
		//g=g1;
		vertices[0] = X;
		vertices[1] = Y;
		vertices[2] = Z;
		sort(vertices+1,vertices+3); ///deone specialy for origianl defination of triple
		//edges[0]=g->edge_exists(vertices[0],vertices[1]);
		//edges[1]=g->edge_exists(vertices[0],vertices[2]);
		//edges[2]=g->edge_exists(vertices[1],vertices[2]);

		//int te=0;
		//for (int i=0;i<3;i++){
		//	if (edges[i]==true){
		//		te++;
		//	}
		//}

		//if (te == 2){
		//	graphType=PATH;
		//}else if (te == 3){
		//	graphType=TRI;
		//}else{
		//	cout<<"error graphlet not triangle or path\n";
		//	exit(1);
		//}

	}

	void print_graphlet(){
		cout<<vertices[0]<<","<<vertices[1]<<","<<vertices[2]<<"\n";
		/*cout<<"graphlet:(";
		for(int i=0; i< 3; i++)
			 cout << vertices[i] << ",";
		cout <<")"<<endl;*/
	}

	graphlet* clone() const { //can be a wastefull process to use
		graphlet* clone = new graphlet(vertices[0],vertices[1],vertices[2]);
		return clone;
	}
};

#endif /* TRIANGLE_H_ */
