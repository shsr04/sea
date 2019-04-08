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
        ChoiceDictionaryIterator vertices = g.vertices();
        vertices.init();
        while (vertices.more()) {
            uint64_t u = vertices.next();
            if (g.deg(u) == 2) {
                d.insert(u);
            }
        }
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
            if (c.isClosed) {
                bool repeat = false, doTried = false;
                do {
                    bool gotTried = false;
                    forEach(c, [this, &d, &gotTried](uint64_t u, uint64_t) {
                        if (tried[u]) {
                            gotTried = true;
                        }
                        g.removeVertex(u);
                        d.remove(u);
                    });
                    if (c.isCycle || gotTried) {
                        break;
                    }
                    // if an endpoint becomes degree 2, repeat immediately
                    if (g.deg(c.c1.first) == 2) {
                        c = chain(c.c1.first);
                        if (c.isClosed) {
                            repeat = true;
                        }
                        doTried = true;
                    } else if (g.deg(c.c2.first) == 2) {
                        c = chain(c.c2.first);
                        if (c.isClosed) {
                            repeat = true;
                        }
                        doTried = true;
                    } else {
                        repeat = false;
                    }
                } while (repeat);
                if (doTried) {
                    forEach(c, [this](uint64_t u, uint64_t) { tried[u] = 1; });
                }
                di.init();  // re-init
            }
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
    ChoiceDictionaryIterator vertices = g.vertices();
    vertices.init();
    // insert one vertex for each good closed chain
    while (vertices.more()) {
        uint64_t u = vertices.next();
        if (g.deg(u) == 2) {
            ChainData c = chain(u);
            if (c.isClosed && c.isGood &&
                !d.get(g.head(c.c1.first, c.c1.second))) {
                d.insert(g.head(c.c1.first, c.c1.second));
            }
        }
    }
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
            forEach(c, [this](uint64_t u, uint64_t) { g.removeVertex(u); });
            if (!c.isClosed) {
                g.addEdge(c.c1.first, c.c2.first);
            } else {
                // check if an endpoint got degree 2 or 4
                for (uint64_t e : {c.c1.first, c.c2.first}) {
                    if (g.deg(e) == 2) {
                        d.insert(e);
                    } else if (g.deg(e) == 4) {
                        for (uint64_t k = 0; k < g.deg(e); k++) {
                            if (g.deg(g.head(e, k)) == 2) {
                                d.insert(g.head(e, k));
                            }
                        }
                    }
                }
            }
        } else {
            forEach(c, [this, &d](uint64_t u, uint64_t) {
                // clear old deg. 2 vertices and shortcuts
                if (d.get(u)) {
                    d.remove(u);
                }
                if (shortcuts.get(u)) {
                    shortcuts.remove(shortcuts.get(u));
                    shortcuts.remove(u);
                }
            });
            shortcuts.insert(u, g.head(c.c2.first, c.c2.second));
            shortcuts.insert(g.head(c.c2.first, c.c2.second), u);
        }
        di.init();
    }
    return true;
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
                                 BiConsumer f) {
    uint64_t u = c.c1.first, k = c.c1.second;
    uint64_t p;
    while (u != c.c2.first) {
        f(u, k);
        p = u;
        u = g.head(u, k);
        k = g.head(u, 0) == p;
    }
}

}  // namespace Sealib
