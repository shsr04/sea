#ifndef SEALIB_GRAPH_VIRTUALGRAPH_H_
#define SEALIB_GRAPH_VIRTUALGRAPH_H_
#include <vector>
#include "sealib/collection/staticspacestorage.h"
#include "sealib/graph/graph.h"

namespace Sealib {

/**
 * A virtual subgraph of a given directed or undirected graph. Vertices and
 * edges can be deleted from the virtual graph. Adding new edges is not part of
 * this class (it should be implemented in the using algorithm).
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
     * @param u Vertex u
     * @param k index in the adjacency vector of node u
     * @return Returns v that is the k-th neighbor of u.
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
     * Remove the given edge (u,v) from the virtual graph.
     * (Note: If you are using an underlying undirected graph, you need to
     * remove both edges (u,v) and (v,u).)
     * @param u first endpoint of the edge
     * @param v second endpoint of the edge
     */
    void removeEdge(uint64_t u, uint64_t v);

 private:
    Graph const &g;
    uint64_t n;
    std::vector<bool> presentVertices;
    std::vector<std::vector<bool>> presentEdges;
};

}  // namespace Sealib

#endif  // SEALIB_GRAPH_VIRTUALGRAPH_H_
