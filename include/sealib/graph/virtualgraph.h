#ifndef SEALIB_GRAPH_VIRTUALGRAPH_H_
#define SEALIB_GRAPH_VIRTUALGRAPH_H_
#include <vector>
#include "sealib/collection/staticspacestorage.h"
#include "sealib/graph/graph.h"

namespace Sealib {

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

    void removeEdge(uint64_t u, uint64_t v);

 private:
    Graph const &g;
    uint64_t n;
    StaticSpaceStorage actualDegrees;
    std::vector<std::vector<bool>> deletedAdjacencies;
};

}  // namespace Sealib

#endif  // SEALIB_GRAPH_VIRTUALGRAPH_H_
