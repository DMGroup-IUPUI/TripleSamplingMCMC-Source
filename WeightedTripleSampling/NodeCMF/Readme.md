#NodeCMF

**Samples Triple from weighted distribution**
- Samples Nodes using Cumulative Mass Function
- Samples Triples from sampled Node

**Weighted-Triple-Node-CMF.cpp**
- Main Function

**inEdgeList.h**
- Reads edge list from file

**graph.h, graph.cpp**
- Graph Data Structure
- Populates neighbors of current triple
- Weight corresponding to a triple is computed using the function "double graph_::getTripleWeight(graphlet* gl, struct neighbor* tempNeighbors)"

**graphlet.h**
- Triple representation

**nodeCMF.h, nodeCMF.cpp**
- Samples triple following dirstribution defined in "graph.cpp"

**Random number generator**
- random.h
- random.cpp

**terget_achived_plot.py**
- generates terget-distribution and achieved-distribution for plotting

**results**
- Example results

