#include "sealib/graph/virtualgraph.h"

namespace Sealib {

VirtualGraph::VirtualGraph(Graph const &graph)
    : g(graph),
      n(g.getOrder()),
      presentVertices(n),
      presentEdges(),
      actualDegree(g),
      virtualEdges(n) {
    for (uint64_t u = 0; u < n; u++) {
        presentVertices.insert(u);
        presentEdges.emplace_back(ChoiceDictionary(g.deg(u)));
        actualDegree.insert(u, g.deg(u));
        for (uint64_t k = 0; k < g.deg(u); k++) {
            presentEdges[u].insert(k);
        }
    }
}

uint64_t VirtualGraph::deg(uint64_t u) const { return actualDegree.get(u); }

uint64_t VirtualGraph::head(uint64_t u, uint64_t k) const {
    ChoiceDictionaryIterator c(presentEdges[u]);
    uint64_t a = 0;
    while (c.more()) {
        if (a == k) {
            return c.next();
        }
        a++;
    }
    return INVALID;
}

uint64_t VirtualGraph::getOrder() const { return n; }

void VirtualGraph::removeVertex(uint64_t u) {
    presentVertices.remove(u);
    n--;
}

bool VirtualGraph::hasVertex(uint64_t u) const {
    return presentVertices.get(u);
}

void VirtualGraph::removeEdge(uint64_t u, uint64_t v) {
    bool done = false;
    ChoiceDictionaryIterator c(presentEdges[u]);
    uint64_t b = 0;
    while (c.more()) {
        uint64_t a = c.next();
        if (a == v) {
            presentEdges[u].remove(b);
            done = true;
            break;
        }
        b++;
    }
    if (!done) {
        if (virtualEdges.member(u) && virtualEdges.member(v)) {
            // if v.length=1:
            virtualEdges.remove(u);
            virtualEdges.remove(v);
            done = true;
        }
    }
    if (done) {
        actualDegree.insert(u, actualDegree.get(u) - 1);
        actualDegree.insert(v, actualDegree.get(v) - 1);
    }
}

void VirtualGraph::addEdge(uint64_t u, uint64_t v) {
    if (virtualEdges.member(u)) {
        virtualEdges.insert(u, /*+*/ v);
    }
    if (virtualEdges.member(v)) {
        virtualEdges.insert(v, /*+*/ u);
    }
    actualDegree.insert(u, actualDegree.get(u) + 1);
    actualDegree.insert(v, actualDegree.get(v) + 1);
}

}  // namespace Sealib
