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

static std::vector<bool> makeBits(UndirectedGraph const &g) {
    std::vector<bool> r;
    for (uint64_t u = 0; u < g.getOrder(); u++) {
        r.push_back(1);
        for (uint64_t k = 0; k < g.deg(u) + 2; k++) {
            r.push_back(0);
        }
    }
    return r;
}

OuterplanarChecker::OuterplanarChecker(UndirectedGraph const &graph)
    : g(graph),
      n(g.getOrder()),
      m(countEdges(graph)),
      tried(n),
      paths(2 * m + 2 * n, 3),
      pathOffset(Bitset<uint8_t>(makeBits(graph))),
      shortcuts(2 * n) {}

bool OuterplanarChecker::isOuterplanar() {
    if (m > 2 * n - 3) {
        return false;
    }
    if (!removeClosedChains()) {
        return false;
    }
    if (!removeAllChains()) {
        return false;
    }
    return true;
}

bool OuterplanarChecker::removeClosedChains() {
    for (uint64_t round = 0; round < log2(log2(n)) + 1; round++) {
        if (g.getOrder() <= 3) {
            return true;
        }
        ChoiceDictionary d(n);
        g.vertices().forEach([&](uint64_t u) {
            if (g.deg(u) == 2) {
                ChainData c = chain(u);
                if (!d.get(g.head(c.c1.first, c.c1.second))) {
                    d.insert(g.head(c.c1.first, c.c1.second));
                }
            }
        });
        ChoiceDictionaryIterator di(d);
        di.init();
        if (!di.more()) {
            return false;
        }
        while (di.more()) {
            if (g.getOrder() <= 3) {
                return true;
            }
            uint64_t u = di.next();
            d.remove(u);
            ChainData c = chain(u);
            if (c.isClosed) {
                bool repeat = false, repeatedOnce = false;
                do {
                    bool gotTried = false, tooManyPaths = false;
                    forEach(c,
                            [&](uint64_t u) {
                                if (tried[u]) {
                                    gotTried = true;
                                }
                                g.removeVertex(u);
                                if (d.get(u)) {
                                    d.remove(u);
                                }
                            },
                            [&](uint64_t u, uint64_t k) {
                                if (!incrementPaths(u, k)) {
                                    tooManyPaths = true;
                                }
                            });
                    // remove edges leading into the chain
                    for (std::pair<uint64_t, uint64_t> e : {c.c1, c.c2}) {
                        if (g.deg(e.first) < e.second + 1) {
                            return false;
                        }
                        g.removeEdge(e.first, g.head(e.first, e.second));
                    }
                    if (tooManyPaths) {
                        return false;
                    } else if (c.isCycle || gotTried) {
                        break;
                    }
                    // if an endpoint becomes degree 2, repeat immediately
                    repeat = false;
                    for (uint64_t e : {c.c1.first, c.c2.first}) {
                        if (g.deg(e) == 2) {
                            if (chain(e).isClosed) {
                                c = chain(e);
                                repeat = true;
                                repeatedOnce = true;
                                break;
                            }
                        }
                    }
                } while (repeat);
                if (g.getOrder() <= 3) {
                    return true;
                }
                if (repeatedOnce) {
                    // if chain could not be reduced further
                    forEach(c, [this](uint64_t u) { tried[u] = 1; },
                            [](uint64_t, uint64_t) {});
                }
            }
            di.init();  // re-init
        }
        tried.assign(tried.size(), 0);
    }
    return true;
}

bool OuterplanarChecker::removeAllChains() {
    if (g.getOrder() <= 3) {
        return true;
    }
    ChoiceDictionary d(n);
    // insert one vertex for each good closed chain
    g.vertices().forEach([&](uint64_t u) {
        if (g.deg(u) == 2) {
            ChainData c = chain(u);
            if (c.isClosed && c.isGood &&
                !d.get(g.head(c.c1.first, c.c1.second))) {
                d.insert(g.head(c.c1.first, c.c1.second));
            }
        }
    });
    ChoiceDictionaryIterator di(d);
    di.init();
    if (!di.more()) {
        return false;
    }
    while (di.more()) {
        if (g.getOrder() <= 3) {
            return true;
        }
        uint64_t u = di.next();
        ChainData c = chain(u);
        if (c.isGood) {
            d.remove(u);
            bool tooManyPaths = false;
            forEach(c, [&](uint64_t u) { g.removeVertex(u); },
                    [&](uint64_t u, uint64_t k) {
                        if (!incrementPaths(u, k)) {
                            tooManyPaths = true;
                        }
                    });
            if (tooManyPaths) {
                return false;
            }
            if (!c.isClosed) {
                g.addEdge(c.c1.first, c.c2.first);
            } else {
                // check if an endpoint got degree 2, 3 or 4
                for (uint64_t e : {c.c1.first, c.c2.first}) {
                    if (g.deg(e) == 2) {
                        d.insert(e);
                    } else if (g.deg(e) == 3 || g.deg(e) == 4) {
                        for (uint64_t k = 0; k < g.deg(e); k++) {
                            if (g.deg(g.head(e, k)) == 2) {
                                d.insert(g.head(e, k));
                            }
                        }
                    }
                }
            }
        } else {
            // clear old deg. 2 vertices and make shortcut
            forEach(c, [this, &d](uint64_t u) {
                if (d.get(u)) {
                    d.remove(u);
                }
                // if (shortcuts.get(u)) {
                //    shortcuts.remove(shortcuts.get(u));
                //    shortcuts.remove(u);
                //}
            });
            // shortcuts.insert(u, g.head(c.c2.first, c.c2.second));
            // shortcuts.insert(g.head(c.c2.first, c.c2.second), u);
        }
        di.init();
    }
    return true;
}

bool OuterplanarChecker::incrementPaths(uint64_t u, uint64_t k) {
    uint64_t a = (pathOffset.select(u + 1) - u) + g.index(u, k);
    paths.insert(a, paths.get(a) + 1);
    return paths.get(a) <= 2;
}

OuterplanarChecker::ChainData OuterplanarChecker::chain(uint64_t u) {
    ChainData r;
    assert(g.deg(u) == 2);
    uint64_t v1 = g.head(u, 0), v2 = g.head(u, 1);
    uint64_t p1 = u, p2 = u;
    while (g.deg(v1) == 2 || g.deg(v2) == 2) {
        if (v1 == u || v2 == u) {
            // cycle detected (a cycle is a valid chain as long as the two
            // chosen endpoints are adjacent)
            r.isCycle = true;
            break;
        }
        if (g.deg(v1) == 2) {
            uint64_t k1 = g.head(v1, 0) == p1;
            p1 = v1;
            v1 = g.head(v1, k1);
        }
        if (g.deg(v2) == 2) {
            uint64_t k2 = g.head(v2, 0) == p2;
            p2 = v2;
            v2 = g.head(v2, k2);
        }
    }
    if (r.isCycle) {
        uint64_t a = g.head(u, 0);
        r.c1 = {u, 1}, r.c2 = {a, g.head(a, 0) == u};
        r.isClosed = true, r.isGood = true;
    } else {
        // find edges pointing inwards
        uint64_t k1 = INVALID, k2 = INVALID;
        for (uint64_t k = 0; k < g.deg(v1); k++) {
            if (g.head(v1, k) == p1) {
                k1 = k;
                break;
            }
        }
        for (uint64_t k = 0; k < g.deg(v2); k++) {
            if (g.head(v2, k) == p2) {
                k2 = k;
                break;
            }
        }
        assert(k1 != INVALID && k2 != INVALID);
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
    }
    return r;
}

void OuterplanarChecker::forEach(OuterplanarChecker::ChainData const &c,
                                 Consumer v, BiConsumer e) {
    uint64_t u = g.head(c.c1.first, c.c1.second),
             k = g.head(u, 0) == c.c1.first;
    e(c.c1.first, c.c1.second);
    e(u, g.head(u, 0) != c.c1.first);
    uint64_t p = c.c1.first;
    while (u != c.c2.first) {
        v(u);
        e(u, g.head(u, 0) == p);  // forward edge
        p = u;
        u = g.head(u, k);
        k = g.head(u, 0) == p;
        e(u, g.head(u, 0) != p);  // backward edge
    }
}

}  // namespace Sealib
