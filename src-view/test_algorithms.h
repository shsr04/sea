#ifndef SRC_VIEW_TEST_ALGORITHMS_H_
#define SRC_VIEW_TEST_ALGORITHMS_H_
#include <cstdio>
#include "../src/marker/simplecutvertexiterator.h"
#include "sealib/_types.h"
#include "sealib/graph/graphcreator.h"
#include "sealib/graph/graphio.h"
#include "sealib/iterator/cutvertexiterator.h"
#include "sealib/iterator/dfs.h"
#include "sealib/runtimetest.h"

namespace Sealib {
/**
 * Helper class that contains prepared algorithm runtime/space tests.
 */
class AlgorithmComparison {
 public:
    static void runtimeDFS(std::string file1, std::string file2,
                           uint from = 1e5, uint to = 1e6) {
        RuntimeTest t1, t2;
        for (uint n = from; n < to; n += from) {
            DirectedGraph g = GraphCreator::kOutdegree(n, 20);
            t1.runTest([&]() { DFS::standardDFS(&g); }, n, 20 * n);
            t2.runTest([&]() { DFS::nloglognBitDFS(&g); }, n, 20 * n);
        }
        t1.printResults();
        t1.saveCSV(file1);
        printf("-----\n");
        t2.printResults();
        t2.saveCSV(file2);
    }

    static void spaceDFS(std::string file1, std::string file2, uint from = 1e5,
                         uint to = 1e6) {
        RuntimeTest t1, t2;
        for (uint n = from; n < to; n += from) {
            DirectedGraph g = GraphCreator::kOutdegree(n, 20);

            {
                std::vector<uint> color(n);
                std::vector<std::pair<uint, uint>> s;
                for (uint u = 0; u < n; u++) {
                    if (color[u] == DFS_WHITE) {
                        DFS::visit_standard(u, &g, &color, &s, DFS_NOP_PROCESS,
                                            DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                                            DFS_NOP_PROCESS);
                    }
                }
                uint64_t usageKB =
                    (color.capacity() * sizeof(uint) +
                     s.capacity() * sizeof(std::pair<uint, uint>)) /
                    (1 << 10);
                t1.addLine(n, 20 * n, usageKB);
            }
            {
                CompactArray color(n, 3);
                ExtendedSegmentStack s(n, &g, &color);
                for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
                for (uint a = 0; a < n; a++) {
                    if (color.get(a) == DFS_WHITE)
                        DFS::visit_nloglogn(
                            a, &g, &color, &s,
                            [&](uint u0) {
                                DFS::restore_top(u0, &g, &color, &s);
                            },
                            DFS_NOP_PROCESS, DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                            DFS_NOP_PROCESS);
                }
                uint64_t usageKB =
                    (color.byteSize() + s.byteSize()) / (1 << 10);
                t2.addLine(n, 20 * n, usageKB);
            }
        }
        t1.printResults();
        t1.saveCSV(file1, "order,size,memory");
        printf("-----\n");
        t2.printResults();
        t2.saveCSV(file2, "order,size,memory");
    }

    static void spaceSeg() {
        DirectedGraph g = GraphCreator::kOutdegree(1e6, 20);
        {
            uint n = g.getOrder();
            CompactArray color(n, 3);
            ExtendedSegmentStack s(n, &g, &color);
            for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
            for (uint a = 0; a < n; a++) {
                if (color.get(a) == DFS_WHITE)
                    DFS::visit_nloglogn(
                        a, &g, &color, &s,
                        [&](uint u0) { DFS::restore_top(u0, &g, &color, &s); },
                        DFS_NOP_PROCESS, DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                        DFS_NOP_PROCESS);
            }
            uint64_t usage = color.byteSize() + s.byteSize();
            printf("Usage: %lu B\n", usage);
        }
    }

    static void runtimeCV(std::string file1, std::string file2, uint from = 1e5,
                          uint to = 1e6) {
        RuntimeTest t1, t2;
        for (uint n = from; n < to; n += from) {
            UndirectedGraph g = GraphCreator::sparseUndirected(n);
            t1.runTest(
                [&]() {
                    SimpleCutVertexIterator c(&g);
                    c.init();
                    while (c.more()) c.next();
                },
                n, 20 * n);
            t2.runTest(
                [&]() {
                    CutVertexIterator c(&g);
                    c.init();
                    while (c.more()) c.next();
                },
                n, 20 * n);
        }
        t1.printResults();
        t1.saveCSV(file1);
        printf("-----\n");
        t2.printResults();
        t2.saveCSV(file2);
    }

    static void spaceCV(std::string file1, std::string file2, uint from = 1e5,
                        uint to = 1e6) {
        RuntimeTest t1, t2;
        for (uint n = from; n < to; n += from) {
            UndirectedGraph g = GraphCreator::sparseUndirected(n);

            {
                SimpleCutVertexIterator c(&g);
                    c.init();
                    while (c.more()) c.next();
                uint64_t usageKB = c.byteSize() / (1 << 10);
                t1.addLine(n, 20 * n, usageKB);
            }
            {
                CutVertexIterator c(&g);
                    c.init();
                    while (c.more()) c.next();
                uint64_t usageKB = c.byteSize() / (1 << 10);
                t2.addLine(n, 20 * n, usageKB);
            }
        }
        t1.printResults();
        t1.saveCSV(file1, "order,size,memory");
        printf("-----\n");
        t2.printResults();
        t2.saveCSV(file2, "order,size,memory");
    }
};

}  // namespace Sealib
#endif  // SRC_VIEW_TEST_ALGORITHMS_H_
