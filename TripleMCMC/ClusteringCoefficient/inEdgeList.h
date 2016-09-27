/*
 * inEdgeList.h
 *
 *  Created on: Jul 2, 2012
 *      Author: mmrahman
 */

#ifndef INEDGELIST_H_
#define INEDGELIST_H_
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "graph.h"
using namespace std;

class inEdgeList{
private:
	map<unsigned int,unsigned int> vertexMap; //used to map vertices to a continuous range of id, starting from 0.
	unsigned int nextID;

	//temporary data structure for graph
	vector<vector<unsigned int> > tempAdj;
	set<pair<int,int> > test;
public:
	~inEdgeList(){
		vertexMap.clear();
		for (int i=0;i < tempAdj.size();i++){
			tempAdj[i].clear();
		}
		tempAdj.clear();
		test.clear();
		cout<<"inEdgeList: Object DELETED SUCCESSFULLY\n";
	}
	inEdgeList(const char* filename, graph_* g){
		ifstream infile(filename, ios::in);
		unsigned int a,b; //used for input
		unsigned int totalAdjEntry=0;
		nextID=0;
		while(1){
			infile >> a >> b;
			if(infile.eof()) break;
			if (vertexMap.find(a)==vertexMap.end()){
				vertexMap.insert(pair<unsigned int,unsigned int>(a,nextID));
				tempAdj.resize(nextID+1);
				nextID++;
			}
			if (vertexMap.find(b)==vertexMap.end()){
				vertexMap.insert(pair<unsigned int,unsigned int>(b,nextID));
				tempAdj.resize(nextID+1);
				nextID++;
			}

			if(a<b){
				if(test.find(pair<int,int>(a,b))!=test.end()){
					//cout<<"error: "<<a<<","<<b<<"\n";
					continue;
				}
				//cout<<a<<"\t"<<b<<"\n";
				test.insert(pair<int,int>(a,b));
			}else if(a>b){
				if(test.find(pair<int,int>(b,a))!=test.end()){
					//cout<<"error: "<<a<<","<<b<<"\n";
					continue;
				}
				//cout<<b<<"\t"<<a<<"\n";
				test.insert(pair<int,int>(b,a));
			}else if(a==b){
				//cout<<"error: "<<a<<","<<b<<"\n";
				continue;
			}
			//test.insert(pair<int,int>(a,b));


			tempAdj[vertexMap.find(a)->second].push_back(vertexMap.find(b)->second);
			tempAdj[vertexMap.find(b)->second].push_back(vertexMap.find(a)->second);
			totalAdjEntry+=2;

			//cout <<"("<<a <<","<<b<<") ("<<vertexMap.find(a)->second<<","<<vertexMap.find(b)->second<<")\n";
		}
		for (int i=0;i < tempAdj.size();i++){
			sort(tempAdj[i].begin(),tempAdj[i].end());//adjacency list is sorted for letter convenience
			//printing for checking the construction of adjacency list
			/*for(int j=0;j< tempAdj[i].size();j++){
				cout<<tempAdj[i][j]<<" ";
			}
			cout<<"\n";*/
		}

		//file read done. now load into the graph_ class

		if(g->loadgraph(tempAdj,totalAdjEntry)){
			//g->print_ADJ(); //used for testing
		}
		infile.close();
	}
};

#endif /* INEDGELIST_H_ */
