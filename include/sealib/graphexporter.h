#ifndef SEALIB_GRAPHEXPORTER_H_
#define SEALIB_GRAPHEXPORTER_H_
#include <string>
#include <fstream>
#include "sealib/graph.h"
namespace Sealib {
	class GraphExporter {
		public:
		static void exportAsDot(Graph *g,std::string filename,std::string getNodeColor(unsigned));	
	};
}
#endif  // SEALIB_GRAPHEXPORTER_H_
