#ifndef SRC_VIEW_TEST_ALGORITHMS_H_
#define SRC_VIEW_TEST_ALGORITHMS_H_
#include <sealib/_types.h>
#include <sealib/graph/graphcreator.h>
#include <sealib/iterator/dfs.h>
#include <sealib/runtimetest.h>
#include <cstdio>

namespace Sealib {
/**
 * Helper class that contains prepared algorithm runtime/space tests.
 */
class AlgorithmComparison {
 public:
    static void runtimeDFS(std::string output, uint from = 1e5, uint to = 1e6) {
        RuntimeTest t1, t2;
        for (uint n = from; n < to; n += from) {
            DirectedGraph g = GraphCreator::createRandomKRegularGraph(n, 20);
            t1.runTest([&]() { DFS::standardDFS(&g); }, n, 20 * n);
            t2.runTest([&]() { DFS::nloglognBitDFS(&g); }, n, 20 * n);
        }
        t1.printResults();
        printf("-----\n");
        t2.printResults();
    }

    static void spaceDFS(std::string output, uint from = 1e5, uint to = 1e6) {
        RuntimeTest t1, t2;
        for (uint n = from; n < to; n += from) {
            DirectedGraph g = GraphCreator::createRandomKRegularGraph(n, 20);

            {
            std::vector<uint> color(n);
            std::vector<std::pair<uint, uint>> s;
            for (uint u = 0; u < n; u++) {
                if (color[u] == DFS_WHITE) {
                    DFS::visit_standard(u, &g, &color, &s, DFS_NOP_PROCESS, DFS_NOP_EXPLORE,
                                DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
                }
            }
            uint64_t usage=color.capacity()*sizeof(uint)+s.capacity()*sizeof(std::pair<uint, uint>);
            t1.addLine(n,20*n,usage/(1<<10));
            }
            {
            CompactArray color(n, 3);
            ExtendedSegmentStack s(n, &g, &color);
            for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
            for (uint a = 0; a < n; a++) {
                if (color.get(a) == DFS_WHITE)
                    DFS::visit_nloglogn(a, &g, &color, &s, DFS::restore_top, DFS_NOP_PROCESS, DFS_NOP_EXPLORE,
                                DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
            }
            uint64_t usage=color.byteSize()+s.byteSize();
            t2.addLine(n,20*n,usage/(1<<10));
            }
        }
        t1.printResults();
        printf("-----\n");
        t2.printResults();
    }
};

}  // namespace Sealib
#endif  // SRC_VIEW_TEST_ALGORITHMS_H_
