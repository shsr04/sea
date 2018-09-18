#include "sealib/graphexporter.h"

using Sealib::GraphExporter;

void GraphExporter::exportAsDot(Graph *g, std::string filename, std::string getNodeColor(unsigned)) {
	std::ofstream out(filename);
	out << "digraph g {\n";
	for(unsigned a=0; a<g->getOrder(); a++) {
		out << a << " [color=\"" << getNodeColor(a) << "\"]\n";
		for(unsigned b=0; b<g->getNodeDegree(a); b++) {
			out << a << " -> " << g->head(a,b) << " [color=\"" << "blue" << "\"]\n";
		}
	}
	out << "}";
	out.close();
}
