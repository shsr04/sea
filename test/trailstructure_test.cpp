#include <gtest/gtest.h>
#include "../src/trail/trailstructure.h"
#include "../src/dyck/dyckwordlexicon.h"
#include "../src/trail/simpletrailstructure.h"
#include "../src/trail/naivetrailstructure.h"

using Sealib::TrailStructure;

TEST(TrailStructureTest, enter) {
    TrailStructure ts = TrailStructure(5);

    ASSERT_EQ(ts.enter(2), 3);
    ASSERT_EQ(ts.enter(4), 0);
    ASSERT_EQ(ts.enter(1), (uint) -1);  // last element
}

TEST(TrailStructureTest, leave) {
    TrailStructure ts = TrailStructure(5);

    ASSERT_EQ(ts.leave(), 0);

    ts = TrailStructure(5);

    ASSERT_EQ(ts.leave(), 0);
    ASSERT_EQ(ts.leave(), 1);
    ASSERT_EQ(ts.leave(), 2);
    ASSERT_EQ(ts.leave(), 3);
    ASSERT_EQ(ts.leave(), 4);
}

TEST(TrailStructureTest, matches) {
    TrailStructure ts = TrailStructure(5);

    ts.enter(2);
    ts.enter(4);
    ts.enter(1);

    ASSERT_EQ(ts.getMatched(0), 4);
    ASSERT_EQ(ts.getMatched(1), 1);  // has no match, same idx returned
    ASSERT_EQ(ts.getMatched(2), 3);
    ASSERT_EQ(ts.getMatched(3), 2);
    ASSERT_EQ(ts.getMatched(4), 0);
}

TEST(TrailStructureTest, marry) {
    TrailStructure ts = TrailStructure(5);

    ts.enter(2);
    ts.enter(4);
    ts.enter(1);

    ASSERT_EQ(ts.getMatched(0), 4);
    ASSERT_EQ(ts.getMatched(4), 0);
    ASSERT_EQ(ts.getMatched(1), 1);
    ts.marry(0, 1);
    ASSERT_EQ(ts.getMatched(0), 1);
    ASSERT_EQ(ts.getMatched(1), 0);
    ASSERT_EQ(ts.getMatched(4), 4);

    ASSERT_EQ(ts.getMatched(0), 1);
    ASSERT_EQ(ts.getMatched(1), 0);  // had no match before
    ASSERT_EQ(ts.getMatched(2), 3);
    ASSERT_EQ(ts.getMatched(3), 2);
}

TEST(TrailStructureTest, enterLeaveCombination) {
    TrailStructure ts = TrailStructure(5);

    ASSERT_EQ(ts.enter(0), 1);
    ASSERT_EQ(ts.enter(4), 2);
    ASSERT_EQ(ts.leave(), 3);
}

TEST(SimpleTrailStructureTest, allEvenPossibilities) {
    const uint64_t maxLen = 16;
    for (uint len = 2; len < maxLen; len += 2) {
        Sealib::DyckWordLexicon lex(len);
        for (const Sealib::Bitset<uint8_t> &word : lex.getLexicon()) {
            std::vector<std::vector<uint64_t>> depths(len);
            for (uint j = 0; j < len; j++) {
                if (word[j]) {
                    uint64_t match =
                        Sealib::DyckMatchingStructure::getMatchNaive(word, j);
                    uint64_t d = match - j;
                    depths[d].push_back(j);
                }
            }

            for (uint k = 0; k < len; k++) {
                std::vector<std::vector<uint64_t>> shiftedDepths(depths);
                for (auto &shiftedDepth : shiftedDepths) {
                    for (uint64_t &j : shiftedDepth) {
                        j = (j + k) % len;
                    }
                }
                Sealib::NaiveTrailStructure naiveTrailStructure(len);
                Sealib::SimpleTrailStructure simpleTrailStructure(len);
                Sealib::TrailStructure trailStructure(len);

                for (std::vector<uint64_t> &depthVector : shiftedDepths) {
                    if (!depthVector.empty()) {
                        for (uint64_t &idx : depthVector) {
                            naiveTrailStructure.enter(static_cast<uint>(idx));
                            simpleTrailStructure.enter(static_cast<uint>(idx));
                            trailStructure.enter(static_cast<uint>(idx));
                        }
                    }
                }
                for (uint i = 0; i < len; i++) {
                    uint naiveMatch = naiveTrailStructure.getMatched(i);
                    uint simpleMatch = simpleTrailStructure.getMatched(i);
                    uint match = trailStructure.getMatched(i);
                    ASSERT_NE(naiveMatch, i);
                    ASSERT_NE(simpleMatch, i);
                    ASSERT_NE(match, i);
                    ASSERT_EQ(simpleMatch, match);
                }
            }
        }
    }
}
