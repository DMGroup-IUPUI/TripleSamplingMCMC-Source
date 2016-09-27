#TripleMCMC

**Samples Triple from weighted distribution**
- Samples Triples using MCMC

**Weighted-Triple-Triple-MCMC.cpp**
- Main Function

**inEdgeList.h**
- Reads edge list from file

**graph.h, graph.cpp**
- Graph Data Structure
- Populates neighbors of current triple
- Weight corresponding to a triple is computed using the function "double graph_::getTripleWeight(graphlet* gl, struct neighbor* tempNeighbors)"

**graphlet.h**
- Triple representation

**tripleMCMC.h, tripleMCMC.cpp**
- Samples triple following dirstribution defined in "graph.cpp"

**Random number generator**
- random.h
- random.cpp

**terget_achived_plot.py**
- generates terget-distribution and achieved-distribution for plotting

**results**
- Example results

