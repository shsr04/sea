#include "sealib/iterator/bfs.h"
#include <gtest/gtest.h>
#include <stdio.h>
#include "../src/bfs/simplebfs.h"
#include "sealib/_types.h"
#include "sealib/graph/graph.h"
#include "sealib/graph/graphcreator.h"

using namespace Sealib;  // NOLINT

static uint GRAPHCOUNT = 4, order = 500, degree = 20;
static std::vector<DirectedGraph> makeGraphs() {
    std::vector<DirectedGraph> g;
    for (uint c = 0; c < GRAPHCOUNT; c++) {
        g.emplace_back(GraphCreator::kOutdegree(order, degree));
    }
    return g;
}

class BFSTest : public ::testing::TestWithParam<DirectedGraph> {};

INSTANTIATE_TEST_CASE_P(ParamTests, BFSTest, ::testing::ValuesIn(makeGraphs()),
                        /**/);

TEST_P(BFSTest, userproc) {
    uint c1 = 0, c2 = 0;
    DirectedGraph g = GetParam();
    BFS bfs(&g, [&](uint) { c1++; }, [&](uint, uint) { c2++; });
    bfs.init();
    bfs.forEach([](std::pair<uint, uint>) {});
    EXPECT_EQ(c1, order);
    EXPECT_EQ(c2, order * degree);
}

TEST_P(BFSTest, stdUserproc) {
    uint c1 = 0, c2 = 0;
    DirectedGraph g = GetParam();
    SimpleBFS bfs(&g, [&](uint) { c1++; }, [&](uint, uint) { c2++; });
    bfs.init();
    bfs.forEach([](std::pair<uint, uint>) {});
    EXPECT_EQ(c1, order);
    EXPECT_EQ(c2, order * degree);
}

TEST(BFSTest, nextComponent) {
    uint c1 = 0, c2 = 0;
    DirectedGraph g = GraphCreator::kOutdegree(order, 0);
    BFS bfs(&g, [&](uint) { c1++; }, [&](uint, uint) { c2++; });
    uint cc = 0;
    bfs.init();
    bfs.forEach([&](std::pair<uint, uint>) { cc++; });
    EXPECT_EQ(cc, order);
    EXPECT_EQ(c1, order);
    EXPECT_EQ(c2, 0);
}

TEST(SimpleBFSTest, nextComponent) {
    uint c1 = 0, c2 = 0;
    DirectedGraph g = GraphCreator::kOutdegree(order, 0);
    SimpleBFS bfs(&g, [&](uint) { c1++; }, [&](uint, uint) { c2++; });
    uint cc = 0;
    bfs.init();
    bfs.forEach([&](std::pair<uint, uint>) { cc++; });
    EXPECT_EQ(cc, order);
    EXPECT_EQ(c1, order);
    EXPECT_EQ(c2, 0);
}
