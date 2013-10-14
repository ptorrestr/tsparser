/** Main Function
 * Add TLS to all static variable.
 */

#include "VisitorTraversal.h"

int main ( int argc, char* argv[] ) {
	if ( argc < 2)
	{
		std::cerr << "Parameter missing" << std::endl;
		std::cerr << "Usage: parser input_file" << std::endl;
		exit(-1);
	}

    // Build AST
	SgProject* project = frontend(argc,argv);
	ROSE_ASSERT (project != NULL);

	// Build traversal object
	VisitorTraversal theTraversal;

	// Call the traversal function (member function of AstSimpleProcessing)
	// starting at the project node of the AST, using a preorder traversal.
	theTraversal.traverseInputFiles(project, preorder);
    
	std::cout << "Number of scopes (SgScopeStatement) = " 
		<< SgScopeStatement::numberOfNodes() 
		<< std::endl;
	std::cout << "Number of scopes (SgBasicBlock) = "
		<< SgBasicBlock::numberOfNodes()
		<< std::endl;

    return backend(project);
}
