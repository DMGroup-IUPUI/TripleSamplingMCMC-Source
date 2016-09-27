#include <iostream>
#include <vector>
#include "graph.h"
#include "time_tracker.h"
#include "random_walk.h"
#include "graphlet.h"
#include "inEdgeList.h"
#include "string.h"

using namespace std;

//Input file name
char* datafile;

//Sample Count
int SC;

//Graph Object
vector<graph_*> graphs;

//Program Usage
void print_usage(char *prog) {
  cerr<<"Usage: "<<prog<<" -d data-file -sc Sample-Count"<<endl;
  exit(0);
}

//Parsing Command Line Argument
void parse_args(int argc, char* argv[]) {
    if(argc<5) {
        print_usage(argv[0]);
    }
    for (int i=1; i < argc; ++i){
        if (strcmp(argv[i], "-d") == 0){
            datafile=argv[++i];
        }
        if (strcmp(argv[i], "-sc") == 0){
            SC=atoi(argv[++i]);
        }
    }
}//end parse_args()


int main(int argc, char *argv[]) {
    //Argument parsing
	parse_args(argc, argv);
	graphs.resize(1);

    //Load Graph
	time_tracker tt; //time for graph loading
	tt.start();
	inEdgeList* in;
	graphs[0]=new graph_();
	in = new inEdgeList(datafile,graphs[0]); //load the graph data from an input file
	delete in;  //delete input object
	tt.stop();
	cout<<"Graph loaded. Time : "<<tt.print()<<"s\n";
    cout<<"\nStart Sampling Trinagles (Uniformly)\n";
    
    //Sampling triangle
	time_tracker sampleTime;
	sampleTime.start();
	random_walk* rw;
	rw = new random_walk(graphs[0]);
	
    /*
    double triples = graphs[0]->exactTriSeqM3();
	triples/=3;
	cout<< "Total triangles: "<<triples<<"\n";
     */
    rw->sampleUniformTriangle(SC,SC);
	sampleTime.stop();
	cout<<"Sample time: "<<sampleTime.print()<<"\n";

}
