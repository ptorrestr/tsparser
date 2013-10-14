#include "rose.h"

using namespace std;

int main (int argc, char* argv[]) {
	// Build the AST used by ROSE
	SgProject* project = frontend(argc, argv);
	
	// generate a dot file to use in visualizing the AST graph
	generateDOT(*project);
	return 0;
}
