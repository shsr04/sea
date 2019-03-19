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

std::pair<std::pair<uint64_t, uint64_t>, std::pair<uint64_t, uint64_t>>
OuterplanarChecker::chain(uint64_t u, Consumer action) {
    std::pair<std::pair<uint64_t, uint64_t>, std::pair<uint64_t, uint64_t>> r(
        {INVALID, INVALID}, {INVALID, INVALID});
    if (g.deg(u) == 2) {
        uint64_t v1 = g.head(u, 0), v2 = g.head(u, 1);
        uint64_t p1 = u, p2 = u;
        bool isCycle = false;
        while (g.deg(v1) == 2 || g.deg(v2) == 2) {
            if (v1 == u || v2 == u) {
                // cycle detected (a cycle is a valid chain as long as the two
                // chosen endpoints are adjacent)
                isCycle = true;
                break;
            }
            if (g.deg(v1) == 2) {
                uint64_t next1 = g.head(v1, g.head(v1, 0) == p1);
                p1 = v1;
                v1 = next1;
            }
            if (g.deg(v2) == 2) {
                uint64_t next2 = g.head(v2, g.head(v2, 0) == p2);
                p2 = v2;
                v2 = next2;
            }
        }
        if (!isCycle) {
            action(u);
            while (g.deg(v1) == 2 || g.deg(v2) == 2) {
                if (g.deg(v1) == 2) {
                    uint64_t next = g.head(v1, g.head(v1, 0) == p1);
                    action(v1);
                    p1 = v1;
                    v1 = next;
                }
                if (g.deg(v2) == 2) {
                    uint64_t next = g.head(v2, g.head(v2, 0) == p2);
                    action(v2);
                    p2 = v2;
                    v2 = next;
                }
            }
        } else {
            v1 = u;
            p1 = g.head(u, 0);
            do {
                uint64_t next = g.head(v1, g.head(v1, 0) == p1);
                action(v1);
                p1 = v1;
                v1 = next;
            } while (v1 != u);
            v2 = g.head(u, 0);
            p2 = u;
        }
        r = {{v1, v2}, {p1, p2}};
    }
    return r;
}

bool OuterplanarChecker::isOuterplanar() {
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
}

}  // namespace Sealib
