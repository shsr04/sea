#include <gtest/gtest.h>
#include <memory>
#include "../src/planar/simpleouterplanarchecker.h"
#include "sealib/graph/undirectedgraph.h"
#include "sealib/iterator/outerplanarchecker.h"

namespace Sealib {

TEST(OuterplanarCheckerTest, sample1) {
    std::vector<ExtendedNode> nodes(4);
    nodes[0].addAdjacency({1, 0}), nodes[0].addAdjacency({3, 1});
    nodes[1].addAdjacency({0, 0}), nodes[1].addAdjacency({2, 0});
    nodes[2].addAdjacency({1, 1}), nodes[2].addAdjacency({3, 0});
    nodes[3].addAdjacency({0, 1}), nodes[3].addAdjacency({2, 1});
    UndirectedGraph g(nodes);
    SimpleOuterplanarChecker s(g);
    EXPECT_TRUE(s.isOuterplanar()) << "failed at 4-cycle";
    // OuterplanarChecker o(g);
    // EXPECT_TRUE(o.isOuterplanar()) << "failed at 4-cycle";
}
TEST(OuterplanarCheckerTest, sample2) {
    std::vector<ExtendedNode> nodes(4);
    nodes[0].addAdjacency({1, 0}), nodes[0].addAdjacency({3, 1}),
        nodes[0].addAdjacency({2, 2});
    nodes[1].addAdjacency({0, 0}), nodes[1].addAdjacency({2, 0});
    nodes[2].addAdjacency({1, 1}), nodes[2].addAdjacency({3, 0}),
        nodes[2].addAdjacency({0, 2});
    nodes[3].addAdjacency({0, 1}), nodes[3].addAdjacency({2, 1});
    UndirectedGraph g(nodes);
    SimpleOuterplanarChecker s(g);
    EXPECT_TRUE(s.isOuterplanar()) << "failed at 4/1-cycle";
    // OuterplanarChecker o(g);
    // EXPECT_TRUE(o.isOuterplanar()) << "failed at 4/1-cycle";
}
TEST(OuterplanarCheckerTest, sample3) {
    std::vector<ExtendedNode> nodes(4);
    nodes[0].addAdjacency({1, 0}), nodes[0].addAdjacency({3, 1}),
        nodes[0].addAdjacency({2, 2});
    nodes[1].addAdjacency({0, 0}), nodes[1].addAdjacency({2, 0}),
        nodes[1].addAdjacency({3, 2});
    nodes[2].addAdjacency({1, 1}), nodes[2].addAdjacency({3, 0}),
        nodes[2].addAdjacency({0, 2});
    nodes[3].addAdjacency({0, 1}), nodes[3].addAdjacency({2, 1}),
        nodes[0].addAdjacency({1, 2});
    UndirectedGraph g(nodes);
    SimpleOuterplanarChecker s(g);
    EXPECT_FALSE(s.isOuterplanar()) << "failed at 4/2-cycle";
    // OuterplanarChecker o(g);
    // EXPECT_TRUE(o.isOuterplanar()) << "failed at 4/2-cycle";
}
TEST(OuterplanarCheckerTest, sample4) {
    std::vector<ExtendedNode> nodes(5);
    nodes[0].addAdjacency({1, 0}), nodes[0].addAdjacency({3, 1}),
        nodes[0].addAdjacency({2, 2});
    nodes[1].addAdjacency({0, 0}), nodes[1].addAdjacency({2, 0}),
        nodes[1].addAdjacency({4, 0});
    nodes[2].addAdjacency({1, 1}), nodes[2].addAdjacency({3, 0}),
        nodes[2].addAdjacency({0, 2});
    nodes[3].addAdjacency({0, 1}), nodes[3].addAdjacency({2, 1}),
        nodes[3].addAdjacency({4, 1});
    nodes[4].addAdjacency({1, 2}), nodes[4].addAdjacency({3, 2});
    UndirectedGraph g(nodes);
    SimpleOuterplanarChecker s(g);
    EXPECT_FALSE(s.isOuterplanar()) << "failed at 4(1)-cycle";
}
TEST(OuterplanarCheckerTest, sample6) {
    std::vector<ExtendedNode> nodes(5);
    nodes[0].addAdjacency({1, 0}), nodes[0].addAdjacency({3, 1}),
        nodes[0].addAdjacency({2, 2}), nodes[0].addAdjacency({4, 1});
    nodes[1].addAdjacency({0, 0}), nodes[1].addAdjacency({2, 0}),
        nodes[1].addAdjacency({4, 0});
    nodes[2].addAdjacency({1, 1}), nodes[2].addAdjacency({3, 0}),
        nodes[2].addAdjacency({0, 2});
    nodes[3].addAdjacency({0, 1}), nodes[3].addAdjacency({2, 1});
    nodes[4].addAdjacency({1, 2}), nodes[4].addAdjacency({0, 3});
    UndirectedGraph g(nodes);
    SimpleOuterplanarChecker s(g);
    EXPECT_TRUE(s.isOuterplanar()) << "failed at 5/1-cycle";
}

}  // namespace Sealib
