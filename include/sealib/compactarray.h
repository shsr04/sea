#include <math.h>

/*
	This compact array tries to use the space of an unsigned int array as well as possible.
	
	The compact array consists of _data_ elements (the source array slots),
	which are split into a number of _groups_,
	which contain a number of _values_ (the slots of the compact array).
*/
class CompactArray {
	private:
		unsigned int *data;
		double e;
		/*
			data width: no. bits in unsigned int
			group width: no. bits a group occupies
			value width: no. bits a value needs (e.g. ld(3) for 3 possible states)
		*/
		int valueWidth,
			groupWidth,groupCount,
			dataWidth,dataCount;
	
	public:
		CompactArray(unsigned int count);
		
		/* Insert a value to the given index.
			@param i the destination index
			@param p the value to insert
		*/
		void insert(unsigned int i,unsigned int p);
};
