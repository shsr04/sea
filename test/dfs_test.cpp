#include <gtest/gtest.h>
//#define DFS_DEBUG
#include <sealib/dfs.h>
#define DFS_NOP_PROCESS (void(*)(Node*))DFS::nop
#define DFS_NOP_EXPLORE (void(*)(Node*,Node*))DFS::nop
TEST(DFSTest,runStd) {
	int order=6;
	Node *nodes=(Node*)calloc(1,sizeof(Node)*order);
	for(int a=0; a<order-2; a++) nodes[a]=Node(new Adjacency(a+1),1);
	nodes[order-1]=Node(new Adjacency(4),1);
	DFS::runStandardDFS(new Graph(nodes,order),DFS_NOP_PROCESS,DFS_NOP_EXPLORE,DFS_NOP_EXPLORE,DFS_NOP_PROCESS);
	free(nodes);
}
TEST(DFSTest,runSmall) {
	int order=520;
	Node *nodes=(Node*)calloc(1,sizeof(Node)*order);
	for(int a=0; a<order-2; a++) nodes[a]=Node(new Adjacency(a+1),1);
	nodes[order-1]=Node(new Adjacency(4),1);
	DFS::runSmallDFS(new Graph(nodes,order),DFS_NOP_PROCESS,DFS_NOP_EXPLORE,DFS_NOP_EXPLORE,DFS_NOP_PROCESS);
}
