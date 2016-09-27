#ifndef GRAPHLET_H_
#define GRAPHLET_H_
#include <sstream>
#include <iostream>
//#include "graph.h"
//#define TRI 2
//#define PATH 1
using namespace std;

class graphlet
{
public:
	int * vertices;

public:
	~graphlet(){
		delete vertices;
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
		
		vertices = new int[3];
		vertices[0] = X;
		vertices[1] = Y;
		vertices[2] = Z;
		sort(vertices,vertices+3);
	}

	void graphletReplace(int X,int Y,int Z)//reuse the data structure
	{
		vertices[0] = X;
		vertices[1] = Y;
		vertices[2] = Z;
		sort(vertices,vertices+3); ///deone specialy for origianl defination of triple
	}

	void print_graphlet(){
		cout<<"<"<<vertices[0]<<","<<vertices[1]<<","<<vertices[2]<<">\n";
	}

	graphlet* clone() const { //can be a wastefull process to use
		graphlet* clone = new graphlet(vertices[0],vertices[1],vertices[2]);
		return clone;
	}
};

#endif /* TRIANGLE_H_ */
