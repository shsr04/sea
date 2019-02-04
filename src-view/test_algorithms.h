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
    static void runtimeDFS(std::string file1, std::string file2,
                           uint from = 1e5, uint to = 1e6) {
        RuntimeTest t1, t2;
        for (uint n = from; n < to; n += from) {
            DirectedGraph g = GraphCreator::createRandomKRegularGraph(n, 20);
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
            DirectedGraph g = GraphCreator::createRandomKRegularGraph(n, 20);

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
                uint64_t usageKB = (color.capacity() * sizeof(uint) +
                                 s.capacity() * sizeof(std::pair<uint, uint>))/(1<<10);
                t1.addLine(n, 20 * n, usageKB);
            }
            {
                CompactArray color(n, 3);
                ExtendedSegmentStack s(n, &g, &color);
                for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
                for (uint a = 0; a < n; a++) {
                    if (color.get(a) == DFS_WHITE)
                        DFS::visit_nloglogn(a, &g, &color, &s, DFS::restore_top,
                                            DFS_NOP_PROCESS, DFS_NOP_EXPLORE,
                                            DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
                }
                uint64_t usageKB = (color.byteSize() + s.byteSize())/(1<<10);
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
