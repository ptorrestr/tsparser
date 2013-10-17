#include "ASTModifying.h"
//TODO add exceptions

void
ASTModifying::setVariableToTLS
(SgVariableDeclaration* variableDeclaration)
{
    variableDeclaration->get_declarationModifier().get_storageModifier()
        .set_thread_local_storage(true);
}

void
ASTModifying::setVariableToTLS
(SgInitializedName* initializedName)
{   
    initializedName->get_declaration()->get_declarationModifier()
        .get_storageModifier().set_thread_local_storage(true);
}

void
ASTModifying::setVariableToStatic
(SgVariableDeclaration* variableDeclaration)
{
    variableDeclaration->get_declarationModifier().get_storageModifier()
        .setStatic();
}

SgIfStmt*
ASTModifying::createVariableIntialisation
(SgVarRefExp* reference, const SgInitializer* oldInitializer)
{
    // Symbol and name
    SgVariableSymbol* variableSymbol = reference->get_symbol();
    SgName name = ASTQuerying::getNameOfVariableReference(reference);
    // Initialisation expression
    SgNewExp* right = NULL;
    // is Constructor the initializer?
    if ( isSgConstructorInitializer(oldInitializer) ) 
    {
        // Get old constructor
        const SgConstructorInitializer* oldConstructor = 
            isSgConstructorInitializer(oldInitializer);
        // Create a new call function to old constructor
        SgConstructorInitializer* constructor = 
            SageBuilder::buildConstructorInitializer(
                oldConstructor->get_declaration(), oldConstructor->get_args()
                , oldConstructor->get_type(), false, true, true, true);
        // Create the "new" expression        
        right = SageBuilder::buildNewExp( 
            variableSymbol->get_type()->findBaseType(), NULL, constructor, NULL
            , 0, NULL );
    }
    else if ( isSgAggregateInitializer(oldInitializer) )
    {
        // It is an array pointer
        const SgAggregateInitializer* oldAggregate = 
            isSgAggregateInitializer(oldInitializer);
        // Get the size of the array
        int size = oldAggregate->get_initializers()->get_expressions().size();
        // Create a type of base with fixed size
        SgType* type = SageBuilder::buildArrayType(
            variableSymbol->get_type()->findBaseType()
            , SageBuilder::buildIntVal(size));
        // Create the "new" expression
        right = SageBuilder::buildNewExp( type, NULL, NULL, NULL, 0, NULL );
    }
    else if ( isSgAssignInitializer(oldInitializer) )
    {
        //TODO
        std::cout << "TODO: assign initializer" << std::endl;
    }
    else if ( isSgCompoundInitializer(oldInitializer) )
    {
        //TODO
        std::cout << "TODO: compound initializer" << std::endl;
    }
    else if ( isSgDesignatedInitializer(oldInitializer) )
    {
        //TODO
        std::cout << "TODO: designatedInitializer" << std::endl;
    }
    else 
    {
        // we assume as default constructor
        // Using default constructor
        SgType* type = variableSymbol->get_type();
        // Get default constructor
        SgDeclarationStatement* declarationStatement = 
            type->getAssociatedDeclaration();
        SgClassDeclaration* classDeclaration = 
            isSgClassDeclaration(declarationStatement);
        SgMemberFunctionDeclaration* constructorFunction = 
            SageInterface::getDefaultConstructor(classDeclaration);
        // Create a call function to default constructor
        SgConstructorInitializer* constructor =     
            SageBuilder::buildConstructorInitializer(constructorFunction, NULL
                , type, false, true, true, true);
        // Create the "new" expression
        right = SageBuilder::buildNewExp( type->findBaseType() , NULL
            , constructor, NULL, 0, NULL );
        
    }
    // Left expression
    SgVarRefExp* left = SageBuilder::buildVarRefExp(variableSymbol);
    // Create expression: type* pointer = new type();
    SgExprStatement* expr = SageBuilder::buildAssignStatement (left,right);

    // Create IF. (if (! Var) {init})
    // Create a NOT operand for the reference
    SgVarRefExp* newReference = SageBuilder::buildVarRefExp(variableSymbol);
    SgNotOp* notOp = SageBuilder::buildUnaryExpression<SgNotOp>(newReference);
    // Create true body statements
    SgBasicBlock* trueBody = SageBuilder::buildBasicBlock(expr);
    if ( isSgAggregateInitializer(oldInitializer) )
    {
        // Add array initializers
        // Get old aggregate initialiser
        const SgAggregateInitializer* oldAggregate = 
            isSgAggregateInitializer(oldInitializer);
        SgExpressionPtrList::iterator iter = 
            oldAggregate->get_initializers()->get_expressions().begin();
        SgExpressionPtrList::iterator end = 
            oldAggregate->get_initializers()->get_expressions().end();
        // index for new array pointer
        int i_index = 0;
        // For all initializers, add to the array
        for(; iter != end; ++iter)
        {
            // the initializer (val)
            SgExpression* expression = *iter;
            // create a reference to the array (array)
            SgVarRefExp* arrayReference = 
                SageBuilder::buildVarRefExp(variableSymbol);
            // create a index expression using i_index (i)
            SgIntVal* indexExpression = SageBuilder::buildIntVal(i_index++);
            // Create a type of base with fixed size
            SgType* type = SageBuilder::buildArrayType(
                variableSymbol->get_type()->findBaseType()
                , SageBuilder::buildIntVal(0));       
            // create a pointer array refence expression (array[i])
            SgPntrArrRefExp * leftExpression = new SgPntrArrRefExp(
                arrayReference, indexExpression, type);
            // Create assign statment (array[i] = val)
            SgExprStatement* assignStatement = 
                SageBuilder::buildAssignStatement(leftExpression, expression);
            // add statement to if-true-body
            trueBody->append_statement(assignStatement);
        }
    }
    // Create if
    SgIfStmt* newIf = SageBuilder::buildIfStmt(notOp,trueBody, NULL);
    return newIf;
 
}

// TODO: add more expressions
void
ASTModifying::dotReferenceToArrowReference
(SgVarRefExp* reference)
{
    SgName name = ASTQuerying::getNameOfVariableReference(reference);
    SgDotExp* dotNode = isSgDotExp(reference->get_parent());
    if ( dotNode != NULL)
    {
        std::cout << "parent is DotExp node " << std::endl;
        SgFunctionCallExp * functionNode = 
            isSgFunctionCallExp(dotNode->get_parent());
        // Is function call expression?
        if ( functionNode != NULL )
        {
            std::cout << "caso 1" << std::endl;
            std::cout<<"Function call expresion" << std::endl;
            //get children of dot expression
            std::vector<SgNode*> children = 
                dotNode->get_traversalSuccessorContainer();
            SgVarRefExp* left = isSgVarRefExp(children.at(0));	
            SgMemberFunctionRefExp* right = 
                isSgMemberFunctionRefExp(children.at(1));		
            // Build new expression using old ones				
            SgVarRefExp* newExp = 
                SageBuilder::buildVarRefExp(left->get_symbol());
            SgMemberFunctionRefExp* newExp2 = 
                SageBuilder::buildMemberFunctionRefExp(
                    right->get_symbol(), true, false);
            // Build new arrow expression.
            SgArrowExp* newArrow = 
                SageBuilder::buildBinaryExpression<SgArrowExp>(newExp, newExp2);
            SageInterface::replaceExpression(dotNode, newArrow, true);
        }
        // Assignment expression. Other case must be added...!
        else {
            std::cout << "caso 2" << std::endl;
            //Get children of dot expression
            std::vector<SgNode*> children = 
                dotNode->get_traversalSuccessorContainer();				
            SgVarRefExp* left = isSgVarRefExp(children.at(0));	
            SgVarRefExp* right = isSgVarRefExp(children.at(1));		
            //Build new expressions using old ones
            SgVarRefExp* newExpLeft = 
                SageBuilder::buildVarRefExp(left->get_symbol());
            SgVarRefExp* newExpRight = 
                SageBuilder::buildVarRefExp(right->get_symbol());
            SgArrowExp* newArrow = 
                SageBuilder::buildBinaryExpression<SgArrowExp>(newExpLeft
                    , newExpRight);
            SageInterface::replaceExpression(dotNode, newArrow, true);
        }
    }
}

SgVarRefExp*
ASTModifying::createVariableReference
(SgVarRefExp* reference, const NewVariable* newVariable)
{
    // Determine the scope of the variable
    SgScopeStatement* scopeStatement = 
        ASTQuerying::getScopeOfVariableReference(reference);
    // get variable name
    SgName name = newVariable->getName();
    // build new variable expression for new variable name
    SgVarRefExp *newVarRefExp = SageBuilder::buildVarRefExp( 
        newVariable->getInitializedName(), scopeStatement );
    return newVarRefExp;
}

SgVariableDeclaration*
ASTModifying::createCopyVariableDeclaration
(SgInitializedName* oldVariable, const NewVariable* newVariable)
{
    // Create new variable declaration using qualified name (the full name)
    SgInitializedName* newIntializedName = newVariable->getInitializedName();
    SgName newName = newIntializedName->get_qualified_name();   
    SgType* variableType = newVariable->getType();
    SgScopeStatement* scopeStatement = SageInterface::getScope(oldVariable);
    SgVariableDeclaration* result = NULL;
    // if pointer, initialise
    if ( SageInterface::isPointerType(variableType) )
    {
        // Set initialisation to NULL value
        SgAssignInitializer* assignInitializer = 
            SageBuilder::buildAssignInitializer(SageBuilder::buildIntVal(0)
                , variableType);
        result = SageBuilder::buildVariableDeclaration(newName, variableType
            , assignInitializer, scopeStatement);
    }
    else
    {
        result = SageBuilder::buildVariableDeclaration(newName, variableType
            , oldVariable->get_initializer(), scopeStatement);
    }
    return result;
}

SgVariableDeclaration*
ASTModifying::createTSPointerForInitializedName
(SgInitializedName* initializedName, SgName nameNewVariable)
{
    // Get scope
    SgStatement* statement = 
        SageInterface::getEnclosingStatement(initializedName);
    SgScopeStatement* scopStatement = statement->get_scope();
    // Create the type for the new pointer
    SgType* newType = NULL;
    SgType* variableType = initializedName->get_type(); 
    SgType* baseType = variableType->findBaseType();
    /*SgClassType* classType = isSgClassType(baseType);
    // is it a class?
    if ( classType != NULL)
        newType = SageBuilder::buildPointerType(classType);
    else*/
    newType = SageBuilder::buildPointerType(baseType);
    // Build new Variable
    SgVariableDeclaration* newVariableDeclaration 
        = SageBuilder::buildVariableDeclaration (nameNewVariable, newType
            , NULL, scopStatement);
    return newVariableDeclaration;
}

void
ASTModifying::replaceDeclaration
(SgVariableDeclaration* oldDeclaration, SgVariableDeclaration* newDeclaration)
{
    SageInterface::replaceStatement(oldDeclaration, newDeclaration, true);
}

void
ASTModifying::replaceReference
(SgVarRefExp* reference, SgVarRefExp* newReference)
{
    SageInterface::replaceExpression(reference, newReference, true);
}

void
ASTModifying::addExpressionBeforeReference
(SgVarRefExp* reference, SgStatement* newExpression)
{
    SgStatement* statement = SageInterface::getEnclosingStatement (reference);
    SageInterface::insertStatementBefore (statement, newExpression);
}
