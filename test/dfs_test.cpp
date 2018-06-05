#include <gtest/gtest.h>
// #define DFS_DEBUG
#include "sealib/dfs.h"

#define order 30

Node *s[order];
int sp = 0;

void pushN(Node *u) {
	s[sp++] = u;
}

TEST(DFSTest, runStd) {
	Node *nodes = reinterpret_cast<Node*>(calloc(1, sizeof(Node) * 2 * order));
	for(unsigned int a = 0; a < order - 2; a++) {
		nodes[a] = Node(new Adjacency(a + 1), 1U);
	}
	nodes[order - 1] = Node(new Adjacency(4), 1U);
	DFS::runStandardDFS(new Graph(nodes, order), pushN, DFS_NOP_EXPLORE,
        DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
	ASSERT_EQ(s[order - 1], nodes + (order - 1));
	while (sp-->0) {
		ASSERT_EQ(s[sp], nodes+sp);
	}
	free(nodes);
}
TEST(DFSTest,runSmall) {
  Node *nodes = reinterpret_cast<Node*>(calloc(1, sizeof(Node) * 2 * order));
	for(unsigned int a = 0; a < order - 2; a++) {
		nodes[a] = Node(new Adjacency(a + 1), 1U);
	}
	nodes[order - 1] = Node(new Adjacency(4), 1U);
  for(int a=0; a<order; a++) s[a]=static_cast<Node*>(0);
  sp=0;
  DFS::runSmallDFS(new Graph(nodes,order),pushN,DFS_NOP_EXPLORE,DFS_NOP_EXPLORE,DFS_NOP_PROCESS);
  while (sp-->0) {
		ASSERT_EQ(s[sp-1], nodes+sp);
	}
  free(nodes);
}
