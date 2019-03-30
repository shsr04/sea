#include "sealib/iterator/outerplanarchecker.h"
#include <cmath>
#include "sealib/iterator/choicedictionaryiterator.h"

namespace Sealib {

static uint64_t countEdges(UndirectedGraph const &g) {
    uint64_t r = 0;
    for (uint64_t a = 0; a < g.getOrder(); a++) {
        for (uint64_t b = 0; b < g.deg(a); b++) {
            r++;
        }
    }
    return r / 2;
}

OuterplanarChecker::OuterplanarChecker(UndirectedGraph const &graph)
    : g(graph),
      n(g.getOrder()),
      m(countEdges(graph)),
      present(n),
      degreeTwo(n),
      virtualEdges(n),
      removedPaths(m) {}

bool OuterplanarChecker::isOuterplanar() {
    if (m > 2 * n - 3) {
        return false;
    }
    if (!removeClosedChains()) {
        return false;
    }
    return true;
}

bool OuterplanarChecker::removeClosedChains() {
    ChoiceDictionary d(n);
    ChoiceDictionaryIterator di(d);
    for (uint64_t round = 0; round < log2(log2(n)) + 1; round++) {
        for (uint64_t u = 0; u < n; u++) {
            if (g.deg(u) == 2) d.insert(u);
        }
        di.init();
        if (!di.more()) {
            return false;
        }
        while (di.more()) {
            uint64_t u = di.next();
            ChainData c = chain(u);
            if (c.isClosed) {
                bool repeat = false;
                do {
                    forEach(c, [this](uint64_t u) { g.removeVertex(u); });
                    // if an endpoint becomes degree 2, repeat immediately
                    if (g.deg(c.c1.first) == 2) {
                        c = chain(c.c1.first);
                        repeat = true;
                    } else if (g.deg(c.c2.first) == 2) {
                        c = chain(c.c2.first);
                        repeat = true;
                    } else {
                        repeat = false;
                    }
                } while (repeat);
            }
        }
    }
    return true;
}

OuterplanarChecker::ChainData OuterplanarChecker::chain(uint64_t u) {
    ChainData r;
    assert(g.deg(u) == 2);
    uint64_t v1 = g.head(u, 0), v2 = g.head(u, 1);
    uint64_t p1 = u, p2 = u;
    uint64_t k1, k2;
    while (g.deg(v1) == 2 || g.deg(v2) == 2) {
        if (v1 == u || v2 == u) {
            // cycle detected (a cycle is a valid chain as long as the two
            // chosen endpoints are adjacent)
            r.isCycle = true;
            break;
        }
        if (g.deg(v1) == 2) {
            k1 = g.head(v1, 0) == p1;
            p1 = v1;
            v1 = g.head(v1, k1);
        }
        if (g.deg(v2) == 2) {
            k2 = g.head(v2, 0) == p2;
            p2 = v2;
            v2 = g.head(v2, k2);
        }
    }
    r.c1 = {v1, k1}, r.c2 = {v2, k2};
    uint64_t &va = g.deg(v1) < g.deg(v2) ? v1 : v2,
             &vb = g.deg(v1) < g.deg(v2) ? v2 : v1;
    for (uint64_t ka = 0; ka < g.deg(va); ka++) {
        if (g.head(va, ka) == vb) {
            r.isClosed = true;
            break;
        }
    }
    r.isGood = g.deg(v1) <= 4 || g.deg(v2) <= 4;
    return r;
}

void OuterplanarChecker::forEach(OuterplanarChecker::ChainData const &c,
                                 Consumer f) {
    uint64_t u = g.head(c.c1.first, c.c1.second);
    uint64_t p = c.c1.first;
    while (u != c.c2.first) {
        f(u);
        uint64_t k = g.head(u, 0) == p;
        p = u;
        u = g.head(u, k);
    }
}

/*bool OuterplanarChecker::isOuterplanar() {
    bool r = false;
    if (m <= 2 * n - 3) {
        double roundLimit = n * log2(log2(n));
        for (uint64_t round = 0; round < roundLimit; round++) {
            uint64_t actualOrder = 0;
            ChoiceDictionary tried(n);
            ChoiceDictionaryIterator presentI(present);
            presentI.init();
            while (presentI.more()) {
                uint64_t u = presentI.next();
                if (g.deg(u) == 2) {
                    degreeTwo.insert(u);
                }
                actualOrder++;
            }
            if (actualOrder > 0) {
                ChoiceDictionaryIterator degI(degreeTwo);
                degI.init();
                // run stages until D is empty
                while (degI.more()) {
                    uint64_t u = degI.next();
                    bool stage = true;
                    bool firstChain = true;
                    while (stage) {
                        auto cr = chain(u, [&tried, &stage](uint64_t v) {
                            if (tried.get(v)) {
                                stage = false;
                            }
                        });
                        std::pair<uint64_t, uint64_t> c = cr.first;
                        if (stage) {
                            if (isClosed(c)) {
                                chain(u, [this](uint64_t v) {
                                    present.remove(v);
                                    degreeTwo.remove(v);
                                });
                                g.removeEdge(c.first, cr.second.first);
                                g.removeEdge(c.second, cr.second.second);
                                if (g.deg(c.first) != 2 &&
                                    degreeTwo.get(c.first)) {
                                    degreeTwo.remove(c.first);
                                }
                                if (g.deg(c.second) != 2 &&
                                    degreeTwo.get(c.second)) {
                                    degreeTwo.remove(c.second);
                                }
                                if (g.deg(c.first) == 2 ||
                                    g.deg(c.second) == 2) {
                                    u = g.deg(c.first) == 2 ? c.first
                                                            : c.second;
                                    if (g.deg(c.first) == 2) {
                                        degreeTwo.insert(c.first);
                                    }
                                    if (g.deg(c.second) == 2) {
                                        degreeTwo.insert(c.second);
                                    }
                                } else {
                                    stage = false;
                                }
                            } else {
                                // if (!firstChain) {
                                chain(u, [&tried](uint64_t v) {
                                    tried.insert(v);
                                });
                                //}
                                stage = false;
                            }
                            firstChain = false;
                            degI.init();
                        }
                    }
                }
            } else {
                r = true;
                break;
            }
        }
    }
    return r;
}*/

}  // namespace Sealib
