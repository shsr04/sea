#include "sealib/dfs.h"
#include <gtest/gtest.h>
#include <random>
#include <vector>

using Sealib::DFS;
using Sealib::CompactArray;
using Sealib::Graph;
using Sealib::Node;
using Sealib::Adjacency;

unsigned c1 = 0, c2 = 0, c3 = 0, c4 = 0;
unsigned tmp = 0;
void incr1(uint u) {
  c1++;
  tmp = u;
}
void incr4(uint u) {
  c4++;
  tmp = u;
}
void incr2(uint u, uint v) {
  c2++;
  tmp = u;
  tmp = v;
}
void incr3(uint u, uint v) {
  c3++;
  tmp = u;
  tmp = v;
}
void p0(uint u) { printf("preprocess %u\n", u); }
void p1(uint u) { printf("postprocess %u\n", u); }
void e0(uint u, uint v) { printf("preexplore %u,%u\n", u, v); }
void e1(uint u, uint v) { printf("postexplore %u,%u\n", u, v); }
unsigned *cnt;
void incrCnt(unsigned u) { cnt[u]++; }

std::random_device rnd;
const unsigned GRAPHCOUNT = 10;  // how many random graphs to generate?
unsigned order = 0;
class DFSTest : public ::testing::TestWithParam<Graph *> {
  virtual void SetUp() { c1 = c2 = c3 = c4 = 0; }
};
std::vector<Graph *> makeGraphs() {
  std::vector<Graph *> g = std::vector<Graph *>();
  for (uint c = 0; c < GRAPHCOUNT; c++) {
    order = 500;
    Node *n = reinterpret_cast<Node *>(malloc(sizeof(Node) * order));
    c1 = c2 = c3 = c4 = 0;
    for (unsigned int a = 0; a < order; a++) {
      int ai = 5;
      Adjacency *ad =
          reinterpret_cast<Adjacency *>(malloc(sizeof(Adjacency) * ai));
      for (int b = 0; b < ai; b++) ad[b] = Adjacency(rnd() % order);
      n[a] = Node(ad, ai);
    }
    g.push_back(new Graph(n, order));
  }
  return g;
}
std::vector<Graph *> graphs = makeGraphs();
INSTANTIATE_TEST_CASE_P(ParamTests, DFSTest, ::testing::ValuesIn(graphs));

TEST_P(DFSTest, Std_userproc) {
  DFS::runStandardDFS(GetParam(), incr1, incr2, incr3, incr4);
  // DFS::runStandardDFS(g, p0, e0, e1, p1);
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, 5 * order);  // every node has 5 edges
  EXPECT_EQ(c3, 5 * order);
  EXPECT_EQ(c4, order);
}
// TODO(!!!): fix random test failures!
TEST_P(DFSTest, EHK_userproc) {
  DFS::runEHKDFS(GetParam(), incr1, incr2, incr3, incr4);
  EXPECT_EQ(c1, order);
  EXPECT_EQ(c2, 5 * order);  // every node has 5 edges
  EXPECT_EQ(c3, 5 * order);
  EXPECT_EQ(c4, order);
}
