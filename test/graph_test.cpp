#include <gtest/gtest.h>
#include <sealib/graph/graphcreator.h>
#include <sealib/graph/virtualgraph.h>
#include <stdlib.h>

using Sealib::DirectedGraph;
using Sealib::Graph;
using Sealib::GraphCreator;
using Sealib::UndirectedGraph;
using Sealib::VirtualGraph;

TEST(GraphTest, graph_integrity) {
    uint64_t order = 4;
    uint64_t **adj_mtrx = new uint64_t *[order];
    /**
     * (n)       0       1       2       3
     *      **********************************
     *  0   *    0   *   2   *   0   *   1   *
     *      **********************************
     *  1   *    2   *   0   *   1   *   0   *
     *      **********************************
     *  2   *    0   *   1   *   0   *   1   *
     *      **********************************
     *  3   *    1   *   0   *   1   *   0   *
     *      **********************************
     */
    adj_mtrx[0] = new uint64_t[order]{0, 2, 0, 1};
    adj_mtrx[1] = new uint64_t[order]{2, 0, 1, 0};
    adj_mtrx[2] = new uint64_t[order]{0, 1, 0, 1};
    adj_mtrx[3] = new uint64_t[order]{1, 0, 1, 0};

    UndirectedGraph g =
        GraphCreator::createFromAdjacencyMatrix(adj_mtrx, order);

    // verify correct order and degree of nodes
    ASSERT_EQ(g.getOrder(), order);
    ASSERT_EQ(g.getNode(0).getDegree(), 3);
    ASSERT_EQ(g.getNode(1).getDegree(), 3);
    ASSERT_EQ(g.getNode(2).getDegree(), 2);
    ASSERT_EQ(g.getNode(3).getDegree(), 2);

    // verify the adj vector contents of each node
    ASSERT_EQ(g.getNode(0).getAdj()[0].first, 1);
    ASSERT_EQ(g.getNode(0).getAdj()[1].first, 1);
    ASSERT_EQ(g.getNode(0).getAdj()[2].first, 3);

    ASSERT_EQ(g.getNode(1).getAdj()[0].first, 0);
    ASSERT_EQ(g.getNode(1).getAdj()[1].first, 0);
    ASSERT_EQ(g.getNode(1).getAdj()[2].first, 2);

    ASSERT_EQ(g.getNode(2).getAdj()[0].first, 1);
    ASSERT_EQ(g.getNode(2).getAdj()[1].first, 3);

    ASSERT_EQ(g.getNode(3).getAdj()[0].first, 0);
    ASSERT_EQ(g.getNode(3).getAdj()[1].first, 2);

    // verify the adj vector crossindexes of each node
    ASSERT_EQ(g.getNode(0).getAdj()[0].second, 0);
    ASSERT_EQ(g.getNode(0).getAdj()[1].second, 1);
    ASSERT_EQ(g.getNode(0).getAdj()[2].second, 0);

    ASSERT_EQ(g.getNode(1).getAdj()[0].second, 0);
    ASSERT_EQ(g.getNode(1).getAdj()[1].second, 1);
    ASSERT_EQ(g.getNode(1).getAdj()[2].second, 0);

    ASSERT_EQ(g.getNode(2).getAdj()[0].second, 2);
    ASSERT_EQ(g.getNode(2).getAdj()[1].second, 1);

    ASSERT_EQ(g.getNode(3).getAdj()[0].second, 2);
    ASSERT_EQ(g.getNode(3).getAdj()[1].second, 1);
}

TEST(VirtualGraphTest, basicOperations) {
    uint32_t n = 100, deg = 15;
    DirectedGraph baseGraph = GraphCreator::kOutdegree(n, deg);
    VirtualGraph g(baseGraph);
    EXPECT_EQ(g.getOrder(), n);
    for (uint32_t a = 0; a < n; a++) {
        EXPECT_EQ(g.deg(a), deg);
        for (uint b = 0; b < deg; b++) {
            EXPECT_EQ(g.head(a, b), baseGraph.head(a, b));
        }
    }

    g.removeEdge(3, g.head(3, 7));
    ASSERT_EQ(g.deg(3), deg - 1);
    for (uint a = 0; a < 7; a++) {
        EXPECT_EQ(g.head(3, a), baseGraph.head(3, a)) << "failed at " << a;
    }
    for (uint a = 8; a < deg - 1; a++) {
        EXPECT_EQ(g.head(3, a), baseGraph.head(3, a + 1)) << "failed at " << a;
    }
    EXPECT_EQ(g.head(3, deg - 1), Sealib::INVALID);
}
