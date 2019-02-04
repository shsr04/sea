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
            //...
        }
        t1.printResults();
        printf("-----\n");
        t2.printResults();
    }
};

}  // namespace Sealib
#endif  // SRC_VIEW_TEST_ALGORITHMS_H_
