#ifndef SEALIB_GRAPH_VIRTUALGRAPH_H_
#define SEALIB_GRAPH_VIRTUALGRAPH_H_
#include <vector>
#include "sealib/dictionary/raggeddictionary.h"
#include "sealib/dictionary/choicedictionary.h"
#include "sealib/collection/staticspacestorage.h"
#include "sealib/graph/graph.h"

namespace Sealib {

/**
 * A virtual subgraph of a given directed or undirected graph. Vertices and
 * edges can be deleted from the virtual graph. Adding new edges is only
 * supported in a rudimentary way (should be implemented in the using algorithm
 * if necessary).
 *
 * @author Simon Heuser
 */
class VirtualGraph : Graph {
 public:
    /**
     * Creates a new virtual graph.
     * @param g Graph
     */
    explicit VirtualGraph(Graph const &g);

    /**
     * Returns the degree of the given vertex.
     * @param u Vertex u
     * @return number of neighbours for vertex u
     */
    uint64_t deg(uint64_t u) const override;

    /**
     * Returns the vertex v that u points at with its k-th edge.
     * @param u vertex u
     * @param k index in the adjacency vector of node u
     * @return k-th neighbor of u
     */
    uint64_t head(uint64_t u, uint64_t k) const override;

    /**
     * @return Returns the order of the graph, i.e, the total number of
     * vertices.
     */
    uint64_t getOrder() const override;

    /**
     * Remove the given vertex from the virtual graph.
     * @param u vertex to be removed
     */
    void removeVertex(uint64_t u);

    /**
     * Check if the given vertex is still present in the virtual graph.
     * @param u vertex to check
     * @return true if vertex u has not been removed (yet)
     */
    bool hasVertex(uint64_t u) const;

    /**
     * Remove the given outgoing edge from the virtual graph.
     * EFFICIENCY: O(deg(u) ~ log(log(n))) time
     * @param u first endpoint of the edge
     * @param v second endpoint of the edge
     */
    void removeEdge(uint64_t u, uint64_t v);

    /**
     * Adds a virtual edge between the given vertices.
     * Only up to n/log2(n) virtual edges may be present at any time.
     * EFFICIENCY: O(log(log(n))) time
     * @param u endpoint of the virtual edge
     * @param v endpoint of the virtual edge
     */
    void addEdge(uint64_t u, uint64_t v);

 private:
    Graph const &g;
    uint64_t n;
    ChoiceDictionary presentVertices;
    std::vector<ChoiceDictionary> presentEdges;
    StaticSpaceStorage actualDegree;
    RaggedDictionary virtualEdges;
};

}  // namespace Sealib

#endif  // SEALIB_GRAPH_VIRTUALGRAPH_H_