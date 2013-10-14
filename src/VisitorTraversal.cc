//
//  VisitorTraversal.cc
//  
//
//  Created by Pablo Torres Tramón on 7/13/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "VisitorTraversal.h"

VisitorTraversal::VisitorTraversal()
{
    temporalData = new ASTTemporalData();   
}

VisitorTraversal::~VisitorTraversal()
{
    if ( temporalData ) 
    {
        delete temporalData;
        temporalData = NULL;
    }
}

NewVariable*
VisitorTraversal::generateNewVariable
(SgName name, SgType* type, SgInitializedName* initializedName)
const
{
    NewVariable* newVariable = new NewVariable();
    newVariable->setName(name);
    newVariable->setInitializedName(initializedName);
    newVariable->setType(type);
    return newVariable;
}

void 
VisitorTraversal::storeInitializer(SgInitializedName* initializedName)
{
    const SgInitializer* initializer
        = ASTQuerying::getInitializerOfInitializedName(initializedName);
    //store constructor. Only store existing constructor. If constructor
    //is null, no value is stored
    if (initializer != NULL)
    {
        std::cout << "storing initialiser" << std::endl;
        SgName name = ASTQuerying::getNameOfInitializedName(initializedName);
        temporalData->insertOldInitializers(name, initializer);
    }
}

void
VisitorTraversal::storeNewVariable
(SgVariableDeclaration* newVariableDeclaration, SgName oldName)
{
    SgInitializedName* newInitializedName = 
        *(newVariableDeclaration->get_variables().begin());
    storeNewVariable(newInitializedName, oldName);
}

void
VisitorTraversal::storeNewVariable
(SgInitializedName* initializedName, SgName oldName)
{
    SgName newName = 
        ASTQuerying::getNameOfInitializedName(initializedName);
    SgType* newType = initializedName->get_type();
    // Store information about new variable
    NewVariable* newVar = generateNewVariable(newName, newType
        , initializedName);
    temporalData->insertOldNewVariables(oldName, newVar);
}

void
VisitorTraversal::storeSymbol
(SgInitializedName* intializedName, SgName name)
{
    SgSymbol* symbol = intializedName->get_symbol_from_symbol_table();
    temporalData->insertOldSymbol(name, symbol);   
}

void
VisitorTraversal::transformPOD
(SgInitializedName* initializedName)
{
    // store initialiser
    storeInitializer(initializedName);
    // Get variable name
    SgName name = ASTQuerying::getNameOfInitializedName(initializedName);
    // Set to TLS
    ASTModifying::setVariableToTLS(initializedName);
	// Add symbol to map. This is added to modify static 
    // reference if apply. In this case no new variable is added
    storeNewVariable(initializedName, name);
    storeSymbol(initializedName, name);
}

void
VisitorTraversal::transformClass
(SgInitializedName* initializedName, SgVariableDeclaration* declaration)
{
    // store constructor
    storeInitializer(initializedName);
    // Set new name
    SgName name = ASTQuerying::getNameOfInitializedName(initializedName);
    SgName newName = name + "_PTR";
    // Create pointer
    SgVariableDeclaration* newPointer = 
        ASTModifying::createTSPointerForInitializedName(initializedName
            , newName);
    // Set TLS and static
    ASTModifying::setVariableToTLS(newPointer);
    ASTModifying::setVariableToStatic(newPointer);
    // Replace declaration
    ASTModifying::replaceDeclaration(declaration, newPointer);
    // Store data
    storeNewVariable(newPointer, name);
    storeSymbol(initializedName, name);
}

void
VisitorTraversal::transformStaticInitialisation
(SgInitializedName* initializedName, SgVariableDeclaration* declaration)
{
    // Store initialiser
    storeInitializer(initializedName);
    // Retrieve previous definition
    const NewVariable* newVariable = 
        temporalData->getNewVariableOfInitializedName(initializedName);
    // Create new variable declaration from previous one
    SgVariableDeclaration* newDeclaration = 
        ASTModifying::createCopyVariableDeclaration(initializedName
            , newVariable);
    // Setear a TLS
    ASTModifying::setVariableToTLS(newDeclaration);
    // Update AST
    ASTModifying::replaceDeclaration(declaration, newDeclaration);
    /*if ( ASTQuerying::isPointerType(newVariable->getType()) )
    {
        //Add initialiser
        temporalData->insertInitStaticPointers(
            newVariable->getInitializedName()->get_qualified_name()
            , initializedName->get_initializer());
    }*/
}

void
VisitorTraversal::analyseDeclaration
(SgNode* node) 
{
    SgVariableDeclaration* variableDeclaration = isSgVariableDeclaration(node);
    // Get all name initializations        
    SgInitializedNamePtrList::iterator iter = 
        variableDeclaration->get_variables().begin();
    SgInitializedNamePtrList::iterator end = 
        variableDeclaration->get_variables().end();
    // For all names initialized in this declaration 
    for (; iter != end; ++iter)
    {
        SgInitializedName* initializedName = *iter;
        if ( ASTQuerying::isStaticVariableDeclaration(initializedName) )
        {
            // Get variable type
            SgType* variableType = initializedName->get_type();
            if ( ASTQuerying::isPodType(variableType) )
            { 
                std::cout << "INFO: Static POD variable changed to TLS '" 
                    << std::endl;
                transformPOD(initializedName);
            }
            else
            {
                std::cout << "INFO: Static nonPOD variable changed to TLS " 
                    << std::endl;
                transformClass(initializedName, variableDeclaration);
            }
		}
        else 
        {
            const SgSymbol* symbol = 
                temporalData->getOldSymbolOfInitializedName(initializedName);
            if ( ASTQuerying::isStaticVariableInitialisation(initializedName
                , symbol) )
            {
                std::cout << "INFO: Replaced static reference '" << std::endl;
                transformStaticInitialisation(initializedName
                    , variableDeclaration);
		    }
            else 
            {
                std::cout << "INFO: Other variable declaration '" << std::endl;
            }
        }      		
	}
}

void
VisitorTraversal::transformReference
(SgVarRefExp* reference)
{
    // Get new variable declaration
    const NewVariable* newVariable = 
        temporalData->getNewVariableOfVariableReference(reference);
    // Get constructor initialiser
    const SgInitializer* initializer = 
        temporalData->getOldInitializerOfVariableReference(reference);
    // Create new reference with pointer (new initialisation)
    SgVarRefExp* newReference = ASTModifying::createVariableReference(reference
        , newVariable);
    // Replace old reference
    ASTModifying::replaceReference(reference, newReference);
    //Initialise pointer if is not previously initilised
    if ( !temporalData->isInitVariables(newReference) )
    {
        std::cout << "initialise ref " << std::endl;
        // Create initialisation expression
        SgIfStmt* init = ASTModifying::createVariableIntialisation(newReference
            , initializer);
        // Add initialisation
        ASTModifying::addExpressionBeforeReference(newReference, init);
        // mark scope as initialised
        temporalData->insertInitVariables(newReference, true);
    }
    // Update reference operators such as Dot operator.
    if ( ASTQuerying::isVariableReferenceInDotExpression(newReference) )
    {
        ASTModifying::dotReferenceToArrowReference(newReference);
    }
}

void
VisitorTraversal::analyseReference(SgNode* node)
{
    SgVarRefExp* reference = isSgVarRefExp(node);
    SgVariableSymbol* variableSymbol = reference->get_symbol();
    SgName name = variableSymbol->get_name();
    std::cout << "INFO: variable reference '" << name << std::endl;
    // Has been this variable reference refactored previously in other scope
    if ( temporalData->hasBeenVariableRefactored(reference) )
    {
        if ( temporalData->hasVariableNewName(reference) ) 
        {
            std::cout << "INFO: Changing variable reference '" << std::endl;
            transformReference(reference);
        }
    }
    // Has been this variable reference refactored previously in this scope
    else if ( temporalData->hasBeenVariableRefactoredInScope(reference) )
    {
        /*if ( temporalData->hasVariableNewName(reference) ) 
        {
            if ()
            std::cout << "INFO: Updating reference '" << std::endl;
            ASTModifying::dotReferenceToArrowReference(reference);
        }*/
        SgVariableSymbol* variableSymbol = reference->get_symbol();
        SgName name = variableSymbol->get_name();
        std::cout << "INFO: In scope '" << name 
            << "', do nothing" << std::endl; 
    }
    else 
    {
        SgVariableSymbol* variableSymbol = reference->get_symbol();
        SgName name = variableSymbol->get_name();
        std::cout << "INFO: Other variable reference '" << name 
            << "', do nothing" << std::endl; 
    }
}

void 
VisitorTraversal::visit(SgNode* node) 
{
    // Test variable declaration
    if( ASTQuerying::isVariableDeclaration(node) )
    {
        std::cout << "INFO: Variable declaration found, analysing" << std::endl;
        analyseDeclaration(node);
	}
	else if ( ASTQuerying::isVariableReference(node) )
    {
        std::cout << "INFO: Variable reference found, analysing" << std::endl;
        analyseReference(node);
    }
    else if ( ASTQuerying::isMainFunction(node) )
    {
        std::cout << "Is the main function" << std::endl;
    }
    else
    {
        //std::cout << "Otra cosa" << std::endl;
    }
}

void 
VisitorTraversal::atTraversalEnd()
{
    std::cout << "End tranversal" << std::endl;
}
