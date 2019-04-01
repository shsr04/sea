#include "sealib/graph/virtualgraph.h"

namespace Sealib {

VirtualGraph::VirtualGraph(Graph const &graph)
    : g(graph),
      n(g.getOrder()),
      presentVertices(n),
      presentEdges(n),
      virtualEdges(n) {
    presentVertices.flip();
    for (uint64_t u = 0; u < n; u++) {
        presentEdges[u] = std::vector<bool>(g.deg(u));
        presentEdges[u].flip();
    }
}

uint64_t VirtualGraph::deg(uint64_t u) const {
    uint64_t r = 0;
    for (bool p : presentEdges[u]) {
        r += p;
    }
    return r;
}

uint64_t VirtualGraph::head(uint64_t u, uint64_t k) const {
    uint64_t b = 0;
    std::vector<bool> adj = presentEdges[u];
    for (uint64_t a = 0; a < g.deg(u); a++) {
        if (adj[a] == 1) {
            if (b == k) {
                return g.head(u, a);
            }
            b++;
        }
    }
    return INVALID;
}

uint64_t VirtualGraph::getOrder() const { return n; }

void VirtualGraph::removeVertex(uint64_t u) {
    presentVertices[u] = 0;
    n--;
}

bool VirtualGraph::hasVertex(uint64_t u) const { return presentVertices[u]; }

void VirtualGraph::removeEdge(uint64_t u, uint64_t v) {
    for (uint64_t a = 0; a < g.deg(u); a++) {
        if (g.head(u, a) == v) {
            presentEdges[u][a] = 0;
            break;
        }
    }
}

void VirtualGraph::addVirtualEdge(uint64_t u, uint64_t v) {
    if (u < v) {
        virtualEdges.insert(u, v);
    } else {
        virtualEdges.insert(v, u);
    }
}

void VirtualGraph::removeVirtualEdge(uint64_t u, uint64_t v) {
    if (u < v) {
        virtualEdges.remove(u);
    } else {
        virtualEdges.remove(v);
    }
}

}  // namespace Sealib
