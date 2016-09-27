**Paper**
- Mahmudur Rahman, Mohammad Al Hasan. Sampling triples from restricted networks using MCMC strategy. CIKM ’14

**ExampleNetworks**
- Some Toy networks

**NodeMCMC**
- "ClusteringCoefficient" for CC estimation using NodeMCMC
- "UniformTriangleSample" for uniform triangle sampling using NodeMCMC
- "UniformTripleSample" for uniform triple sampling using NodeMCMC

**TripleMCMC**
- "ClusteringCoefficient" for CC estimation using TripleMCMC
- "UniformTriangleSample" for uniform triangle sampling using TripleMCMC
- "UniformTripleSample" for uniform triple sampling using TripleMCMC
- "WeightedTripleSample" for weighted triple sampling using TripleMCMC. Weight is defigned by the neighborhood of triples

**WeightedTripleSampling**
- Comparison among "NodeCMF", "NodeMCMC" and "TripleMCMC"
- For weighted triple sampling 
- Weight of triple defined in function "double graph_::getTripleWeight(graphlet* gl, struct neighbor* tempNeighbors)" in file "graph.cpp"
- Can be changed by changing "double graph_::getTripleWeight(graphlet* gl, struct neighbor* tempNeighbors)"
