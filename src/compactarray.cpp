#include "sealib/compactarray.h"
#include <stdio.h>

void CompactArray::insert(unsigned int i,unsigned int p) {
	int groupIndex=(int)floor(i/(3/e));
	printf("gi=%d\n",groupIndex);
	printf("%u\n",p);
}

CompactArray::CompactArray(unsigned int count) {
	e=count%2==0?1.5:3; //assume that 3/e is an integer that divides n
	groupWidth=(int)ceil(3*(log(3)/log(2))/e); //bits for a group of 3/e (e.g. 2) consec. colors
	groupCount=(int)ceil(count/(double)groupWidth);
	dataCount=(int)ceil((groupCount*groupWidth)/(double)(8*sizeof(unsigned int)));
	printf("gw=%d, gc=%d, dc=%d\n",groupWidth,groupCount,dataCount);
	data=new unsigned int[dataCount];
}
