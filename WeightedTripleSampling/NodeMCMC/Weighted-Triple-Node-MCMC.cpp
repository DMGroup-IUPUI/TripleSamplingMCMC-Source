#include <iostream>
#include <vector>
#include "graph.h"
#include "time_tracker.h"
#include "nodeMCMCtripleCMF.h"
#include "graphlet.h"
#include "inEdgeList.h"
#include "string.h"
using namespace std;

//Input File
char* datafile;

//Sample Size
long sampleCount;

//Network
vector<graph_*> graphs;

void print_usage(char *prog) {
    cerr<<"Usage: "<<prog<<" -d data-file -sc sampleCount"<<endl;
    exit(0);
}

void parse_args(int argc, char* argv[]) {
    if(argc<5) {
        print_usage(argv[0]);
    }
    for (int i=1; i < argc; ++i){
        if (strcmp(argv[i], "-d") == 0){
            datafile=argv[++i];
        }else if (strcmp(argv[i], "-sc") == 0){
            sampleCount=atoi(argv[++i]);
        }
    }
}//end parse_args()

int main(int argc, char *argv[]) {
    //Parse Arguments
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

    //Counts total Triples and triangles and prints stats
    unsigned long exact=graphs[0]->exactTriSeq();
    cout <<"total edges in the entire network: "<< graphs[0]->getEdgeCount()<<"\n";
    cout <<"total vertices in the entire network: "<< graphs[0]->getVertexCount()<<"\n";
    cout <<"total triples: "<<graphs[0]->exactConnectedTriple()<<"\n";
    cout <<"sample size: "<<sampleCount<<"\n";
    cout<<"ExactTriangle: "<<exact<<"\n\n";
    

    //Compute weights of each triples
	nodeMCMCtripleCMF *sample;
	sample = new nodeMCMCtripleCMF(graphs[0]);
    cout<<"Original weights of triples:\n";
	sample->printDistribution();
	
    //Sample From the Distribution
	time_tracker sampleTripleTime;
	sampleTripleTime.start();
    cout<<"Sample triples:\n";
	sample->sampleTriple(sampleCount);
	sampleTripleTime.stop();
	
	delete sample;
	
	cout<<"\nSampleTime: "<<sampleTripleTime.print()<<"\n";

	delete graphs[0];
}

