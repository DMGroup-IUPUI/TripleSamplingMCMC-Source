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
  cerr<<"Usage: "<<prog<<" -d data-file -sc sampleCount"<<endl;
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

	cout <<"Number of edges: "<< graphs[0]->getEdgeCount()<<"\n";

	tt.stop();
	cout<<"Graph loaded. Time : "<<tt.print()<<"s\n\n";

    //Exact Clustering Coefficient. Done for testing (can be skipped)
	time_tracker exactCoEff;
	exactCoEff.start();
	double exact=graphs[0]->exactClusteringCoEff();

	cout<<"Exact clustering co efficient: "<<exact<<"\n";

	exactCoEff.stop();
	cout<<"Exact clustering Co Eff time: "<<exactCoEff.print()<<"\n";
    //Exact Clustering Coefficient
    
    
    //Clustering Coefficient by Triple MCMC
	time_tracker sampleCoEff;
	sampleCoEff.start();
	random_walk* rw;
	rw = new random_walk(graphs[0]);
    cout<<"\nSample Size: "<<SC<<"\n";
    
    float CC=rw->sampleCoEff(SC,SC);
	cout<<"Sample clustering co efficient: "<<CC<<"\n";

	sampleCoEff.stop();
	cout<<"Sample clustering Co Eff time: "<<sampleCoEff.print()<<"\n";

}
