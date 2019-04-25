#include <gtest/gtest.h>
#include "../src/planar/simpleouterplanarchecker.h"
#include "sealib/graph/graphcreator.h"
#include "sealib/graph/graphio.h"
#include "sealib/graph/undirectedgraph.h"
#include "sealib/iterator/bcciterator.h"
#include "sealib/iterator/outerplanarchecker.h"

namespace Sealib {

TEST(OuterplanarCheckerTest, random) {
    for (uint64_t a = 0; a < 1000; a++) {
        BCCOutput b(GraphCreator::kRegular(1000, 3));
        std::vector<ExtendedNode> nodes(1000);
        b.traverse(0, [](uint64_t) {},
                   [&nodes](uint64_t u, uint64_t v) {
                       nodes[u].addAdjacency({v, nodes[v].getDegree()});
                       nodes[v].addAdjacency({u, nodes[u].getDegree() - 1});
                   });
        UndirectedGraph g(std::move(nodes));

        // g=GraphImporter::importGML<UndirectedGraph>("opg-fail1.gml");
        SimpleOuterplanarChecker s1(g);
        OuterplanarChecker s2(g);
        bool r1 = s1.isOuterplanar(), r2 = s2.isOuterplanar();
        if (r1 != r2) {
            GraphExporter::exportGML(g, false, "opg-fail1.gml");
        } else if (r1 && r2) {
            GraphExporter::exportGML(g, false, "opg-true1.gml");
        }
        EXPECT_EQ(r1, r2);
    }
}

TEST(OuterplanarCheckerTest, triangulated) {
    UndirectedGraph g = GraphCreator::triangulated(5000);
    {
        // triangle graph => BOP
        SimpleOuterplanarChecker s1(g);
        OuterplanarChecker s2(g);
        bool r1 = s1.isOuterplanar(), r2 = s2.isOuterplanar();
        EXPECT_TRUE(r1);
        EXPECT_TRUE(r2);
    }

    {
        // triangle graph with one bad edge => not BOP
        g.getNode(1).addAdjacency({g.getOrder() - 2, 3});
        g.getNode(g.getOrder() - 2).addAdjacency({1, 3});
        SimpleOuterplanarChecker s1(g);
        OuterplanarChecker s2(g);
        bool r1 = s1.isOuterplanar(), r2 = s2.isOuterplanar();
        EXPECT_FALSE(r1);
        EXPECT_FALSE(r2);
    }
}

TEST(OuterplanarCheckerTest, cycle) {
    for (uint64_t k = 1; k < 20; k++) {
        UndirectedGraph g = GraphCreator::cycle(500, k);
        EXPECT_TRUE(SimpleOuterplanarChecker(g).isOuterplanar()) << k;
        EXPECT_TRUE(OuterplanarChecker(g).isOuterplanar()) << k;

        // add chord that intersects first chord => not BOP
        g.getNode(1).addAdjacency({g.getOrder() / 2 + 1, 2});
        g.getNode(g.getOrder() / 2 + 1).addAdjacency({1, 2});
        EXPECT_FALSE(SimpleOuterplanarChecker(g).isOuterplanar()) << k;
        EXPECT_FALSE(OuterplanarChecker(g).isOuterplanar()) << k;
    }
}

TEST(OuterplanarCheckerTest, sample1) {
    std::vector<ExtendedNode> nodes(4);
    nodes[0].addAdjacency({1, 0}), nodes[0].addAdjacency({3, 1});
    nodes[1].addAdjacency({0, 0}), nodes[1].addAdjacency({2, 0});
    nodes[2].addAdjacency({1, 1}), nodes[2].addAdjacency({3, 0});
    nodes[3].addAdjacency({0, 1}), nodes[3].addAdjacency({2, 1});
    UndirectedGraph g(nodes);
    SimpleOuterplanarChecker s(g);
    EXPECT_TRUE(s.isOuterplanar()) << "failed at 4-cycle";
    OuterplanarChecker o(g);
    EXPECT_TRUE(o.isOuterplanar()) << "failed at 4-cycle";
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
    OuterplanarChecker o(g);
    EXPECT_TRUE(o.isOuterplanar()) << "failed at 4/1-cycle";
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
    OuterplanarChecker o(g);
    EXPECT_FALSE(o.isOuterplanar()) << "failed at 4/2-cycle";
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
    OuterplanarChecker o(g);
    EXPECT_FALSE(o.isOuterplanar()) << "failed at 4(1)-cycle";
}
TEST(OuterplanarCheckerTest, sample5) {
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
    OuterplanarChecker o(g);
    EXPECT_TRUE(o.isOuterplanar()) << "failed at 5/1-cycle";
}

}  // namespace Sealib
