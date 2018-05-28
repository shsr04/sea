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
void DFS::process_small(uint node,
	Graph *g,CompactArray *color,
	UserFunc1 preProcess,UserFunc2 preExplore,
	UserFunc2 postExplore,UserFunc1 postProcess) {
	unsigned n=g->getOrder();
	unsigned q=(unsigned)ceil(n/log(n)); //TODO 2q entries on S shall take up at most (e/3)n bits
	#ifdef DFS_DEBUG
	printf("q=%u, n=%u, (e/3)n=%.0f\n",q,n,(1.5/3)*n);
	#endif
	Stack *st=new Stack((unsigned)ceil(n/(double)q)),
		*kt=new Stack((unsigned)ceil(n/(double)q));
	Stack *s1=new Stack(q),*s2=new Stack(q);
	Stack *k1=new Stack(q),*k2=new Stack(q);
	/*for(uint a=1; a<=n; a++) {
		if(a%(q+1)==q||a==n) trailers->push(a);
		uint b=tryPush(a,s1,s2);
		if(b!=STACK_FAULT) trailers->push(b);
	}
	while(trailers->peek()!=STACK_FAULT) {
		printf(" %u \n",trailers->pop());
	}
	printf("Top segment:\n");
	while(s2->peek()!=STACK_FAULT) {
		printf(" %u \n",s2->pop());
	}
	printf("Bottom segment:\n");
	while(s1->peek()!=STACK_FAULT) {
		printf(" %u \n",s1->pop());
	}*/
	tryPush(node,s1,s2,st);
	tryPush(1,k1,k2,kt);
	uint u,k;
	while(tryPop(s1,s2,st)!=STACK_FAULT) {
		u=tryPop(s1,s2,st);
		k=tryPop(k1,k2,kt);
		color->insert(u,DFS_GRAY);
		Node *un=g->getNode(u);
		preProcess(un);
		if(k<=un->getDegree()) {
			tryPush(u,s1,s2,st);
			tryPush(k+1,k1,k2,kt);
			uint v=g->head(u,k);
			if(color->get(v)==DFS_WHITE) {
				Node *vn=g->getNode(v);
				preExplore(un,vn);
				if(color->get(v)==DFS_WHITE) {
					tryPush(v,s1,s2,st);
					tryPush(1,k1,k2,kt);
				}
				postExplore(un,vn);
			}
		} else {
			color->insert(u,DFS_BLACK);
		}
		postProcess(un);
	}
}
void DFS::tryPush(uint u,Stack *low,Stack *high,Stack *trailers) {
	if(!low->isFull()) {
		low->push(u);
		trailers->pop(); trailers->push(u);
	}
	else if(!high->isFull()) {
		high->push(u);
		trailers->pop(); trailers->push(u);
	}
	else {
		delete low;
		low=high;
		high=new Stack(low->getSize());
		high->push(u);
	}
}
uint DFS::tryPop(Stack *low, Stack *high,Stack *trailers) {
	if(high->peek()!=STACK_FAULT) {
		return high->pop();
	} else if(low->peek()!=STACK_FAULT) {
		return low->pop();
	} else {
		if(trailers->peek()!=STACK_FAULT) {
			// restore
			return STACK_FAULT;
		} else return STACK_FAULT;
	}
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
	unsigned n=g->getOrder();
	double e=n%2==0?1.5:3; //assume that 3/e is an integer that divides n
	CompactArray *color=new CompactArray(n,e);
	for(uint a=1; a<=g->getOrder(); a++) color->insert(a,DFS_WHITE);
	process_small(1,g,color,preProcess,preExplore,postExplore,postProcess);
	delete color;
}
