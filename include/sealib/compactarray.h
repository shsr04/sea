#include <math.h>

class CompactArray {
	private:
		unsigned int *data;
		double e;
		int groupWidth,groupCount,dataCount;
	
	public:
		CompactArray(unsigned int count);
		
		void insert(unsigned int i,unsigned int p);
};
