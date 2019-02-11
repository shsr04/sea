#include <gtest/gtest.h>
#include <sealib/graph/graphcreator.h>
#include "../src/trail/naiveeulertrail.h"
#include "../src/trail/simpletrailstructure.h"
#include <sealib/iterator/eulertrail.h>
#include <stdlib.h>
#include "../src/trail/trailstructure.h"

TEST(EulerTrailTest, hierholzerTrail) {
    uint order = 6;
    auto **adj_mtrx = reinterpret_cast<uint **>(malloc(sizeof(uint) * order * order));
    adj_mtrx[0] = new uint[order]{0, 1, 0, 1, 1, 1};
    adj_mtrx[1] = new uint[order]{1, 0, 1, 0, 0, 0};
    adj_mtrx[2] = new uint[order]{0, 1, 0, 1, 0, 0};
    adj_mtrx[3] = new uint[order]{1, 0, 1, 0, 0, 0};
    adj_mtrx[4] = new uint[order]{1, 0, 0, 0, 0, 0};
    adj_mtrx[5] = new uint[order]{1, 0, 0, 0, 0, 0};

    std::shared_ptr<Sealib::UndirectedGraph>
        graph_ptr(
            new Sealib::UndirectedGraph(Sealib::GraphCreator::createFromAdjacencyMatrix(
                adj_mtrx,
                order)));
    Sealib::EulerTrail<Sealib::TrailStructure> et(graph_ptr);
    Sealib::EulerTrail<Sealib::SimpleTrailStructure> et2(graph_ptr);
    Sealib::NaiveEulerTrail et3(graph_ptr);
    SUCCEED();
}
