#include "sealib/dfs.h"

// starting point of the DFS algorithm: O(n+m) time, O(n*log n) bits
// TODO step 1: O((n+m)*log(n)) time, O((ld(3)+e)*n) bits
// TODO step 2: O(n+m) time, O(n*log log n) bits

void DFS::process_standard(Graph *g,
	UserFunc1 preProcess,UserFunc2 preExplore,
	UserFunc2 postExplore,UserFunc1 postProcess,
	uint *color,uint u) {
	#ifdef DFS_DEBUG
	printf("process %u\n",u);
	#endif
	color[u]=DFS_GRAY;
	Node *un=g->getNode(u);
	#ifdef DFS_DEBUG
	printf("preprocess %u:\n",u);
	#endif
	preProcess(un);
	for(uint k=0; k<un->getDegree(); k++) {
		#ifdef DFS_DEBUG
		printf("head %u,%u = ",u,k);
		#endif
		uint v=g->head(u,k);
		#ifdef DFS_DEBUG
		printf("%u\n",v);
		#endif
		Node *vn=g->getNode(v);
		#ifdef DFS_DEBUG
		printf("preexplore %u,%u:\n",u,v);
		#endif
		preExplore(un,vn);
		if(color[v]==DFS_WHITE) process_standard(g,preProcess,preExplore,
			postExplore,postProcess,color,v);
		#ifdef DFS_DEBUG
		printf("postexplore %u,%u:\n",u,v);
		#endif
		postExplore(un,vn);
	}
	#ifdef DFS_DEBUG
	printf("postprocess %u:\n",u);
	#endif
	postProcess(un);
	color[u]=DFS_BLACK;
}

void DFS::nop() {}

void DFS::runStandardDFS(Graph *g,void (*preProcess)(Node *),void (*preExplore)(Node *,Node *),
			void (*postExplore)(Node *,Node *),void (*postProcess)(Node *)) { 
	uint *color=new uint[g->getOrder()];
	for(uint u=0; u<g->getOrder(); u++) color[u]=DFS_WHITE;
	//Stack stack=Stack(g->getOrder());
	for(uint u=0; u<g->getOrder(); u++) {
		if(color[u]==DFS_WHITE) {
			process_standard(g,preProcess,preExplore,
				postExplore,postProcess,color,u);
		}
	}
	delete[] color;
}
void DFS::runSmallDFS(Graph *g,void (*preProcess)(Node *),void (*preExplore)(Node *,Node *),
			void (*postExplore)(Node *,Node *),void (*postProcess)(Node *)) { 
	CompactArray *color=new CompactArray(g->getOrder());
	for(uint a=0; a<g->getOrder(); a++) color->insert(a,DFS_GRAY);
	printf("%p %p %p %p\n",(void*)preProcess,(void*)preExplore,(void*)postExplore,(void*)postProcess);
}
