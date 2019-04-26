#include "sealib/graph/virtualgraph.h"

namespace Sealib {

VirtualGraph::VirtualGraph(UndirectedGraph const &graph)
    : UndirectedGraph(0),
      g(graph),
      n(g.getOrder()),
      presentVertices(n),
      presentEdges(),
      actualDegree(g),
      virtualEdges(4 * n) {
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
    c.init();
    uint64_t a = 0;
    while (c.more()) {
        uint64_t v = g.head(u, c.next());
        if (a == k) {
            return v;
        }
        a++;
    }
    if (virtualEdges.member(u) && a >= k - 1) {
        std::pair<uint64_t, uint64_t> p = virtualEdges.get(u);
        if (a == k) {
            return p.first;
        } else if (a == k - 1) {
            return p.second;
        }
    }
    return INVALID;
}

uint64_t VirtualGraph::mate(uint64_t u, uint64_t k) const {
    ChoiceDictionaryIterator c(presentEdges[u]);
    c.init();
    uint64_t a = 0;
    while (c.more()) {
        uint64_t r = g.mate(u, c.next());
        if (a == k) {
            return r;
        }
        a++;
    }
    if (virtualEdges.member(u) && a >= k - 1) {
        std::pair<uint64_t, uint64_t> q = virtualEdges.get(head(u, k));
        assert(q.first == u || q.second == u);
        return q.first == u && q.second != INVALID ? deg(head(u, k)) - 2
                                                   : deg(head(u, k)) - 1;
    }
    return INVALID;
}

uint64_t VirtualGraph::index(uint64_t u, uint64_t k) const {
    ChoiceDictionaryIterator c(presentEdges[u]);
    c.init();
    uint64_t a = 0;
    while (c.more()) {
        uint64_t b = c.next();
        if (a == k) {
            return b;
        }
        a++;
    }
    if (virtualEdges.member(u) && (a == k || a == k - 1)) {
        if (a == k) {
            return g.deg(u) - 2;
        } else if (a == k - 1) {
            return g.deg(u) - 1;
        }
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

ChoiceDictionaryIterator VirtualGraph::vertices() const {
    return ChoiceDictionaryIterator(presentVertices);
}

void VirtualGraph::removeEdge(uint64_t u, uint64_t v) {
    bool done = false;
    // delete edge between u and v
    for (uint64_t e : {u, v}) {
        ChoiceDictionaryIterator c(presentEdges[e]);
        c.init();
        uint64_t f = e == u ? v : u;
        while (c.more()) {
            uint64_t b = c.next();
            if (g.head(e, b) == f) {
                presentEdges[e].remove(b);
                done = true;
                break;
            }
        }
    }
    if (!done) {
        if (virtualEdges.member(u) && virtualEdges.member(v)) {
            // search virtual edge {u,v}
            std::pair<uint64_t, uint64_t> pu = virtualEdges.get(u),
                                          pv = virtualEdges.get(v);
            if (pu.second != INVALID) {
                virtualEdges.insert(
                    u, {pu.first == v ? pu.second : pu.first, INVALID});
            } else {
                virtualEdges.remove(u);
            }
            if (pv.second != INVALID) {
                virtualEdges.insert(
                    v, {pv.first == u ? pv.second : pv.first, INVALID});
            } else {
                virtualEdges.remove(v);
            }
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
        // update with second edge
        virtualEdges.insert(u, {virtualEdges.get(u).first, v});
    } else {
        // add first edge
        virtualEdges.insert(u, {v, INVALID});
    }
    if (virtualEdges.member(v)) {
        virtualEdges.insert(v, {virtualEdges.get(v).first, u});
    } else {
        virtualEdges.insert(v, {u, INVALID});
    }
    actualDegree.insert(u, actualDegree.get(u) + 1);
    actualDegree.insert(v, actualDegree.get(v) + 1);
}

}  // namespace Sealib
