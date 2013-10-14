#include "ASTQuerying.h"

bool
ASTQuerying::isThereDefinedSymbol
(const SgStatement* statement, const NewVariable* newVariable) 
{
    SgScopeStatement* scopStatement = statement->get_scope();
    if ( scopStatement->symbol_exists(newVariable->getInitializedName()
        ->search_for_symbol_from_symbol_table()) )
        return true;
    return false;
}

bool
ASTQuerying::isStaticVariableReference
(const SgVarRefExp *reference) 
{
    SgInitializedName* variable = reference->get_symbol()->get_declaration();
    return isStaticVariableDeclaration(variable); 
}

bool
ASTQuerying::isStaticVariableInitialisation
(const SgInitializedName* variable, const SgSymbol* originalSymbol)
{
    // Determine if the variables declaration is a static
    // initialization
    if ( !variable->search_for_symbol_from_symbol_table() ) 
        return false;
    SgName name = 
        variable->search_for_symbol_from_symbol_table()->get_name();
    
    // Search in original declaration scope the symbol
    SgScopeStatement* scope = variable->get_scope();
    SgSymbol* symbol = scope->lookup_symbol(name);
    // compare current symbol against given.
    if ( originalSymbol == symbol )
    {
        // It is the same variable. The variable is a static initialisation
        return true;
    }
    return false;
}


bool
ASTQuerying::isStaticVariableDeclaration
(const SgInitializedName* variable)
{
    SgStorageModifier tmpModifier = 
        variable->get_declaration()->get_declarationModifier()
            .get_storageModifier();
    if ( tmpModifier.isStatic() )
    {
        return true;
    }
    return false;
}

bool
ASTQuerying::isVariableDeclaration
(const SgNode* node)
{
    //Transform to SgVariableDeclaration* and test if is null
    const SgVariableDeclaration* variableDeclaration 
        = isSgVariableDeclaration(node);
    if ( variableDeclaration != NULL )
        return true;
    return false;
}

bool
ASTQuerying::isVariableReference
(const SgNode *node)
{
    const SgVarRefExp* variableReference = isSgVarRefExp(node);
    if ( variableReference != NULL )
        return true;
    return false;
}

bool
ASTQuerying::isPodType
(SgType* type)
{
    //POD type: int, unsigned, long, float, double, char, *ptr
    bool ret = false;
    if ( SageInterface::isScalarType(type) || 
            SageInterface::isPointerType(type) )
        ret = true;
    return ret;
}

bool
ASTQuerying::isPointerType
(SgType* type)
{
    return SageInterface::isPointerType(type);
}

bool
ASTQuerying::isVariableReferenceInDotExpression
(SgVarRefExp* reference)
{
    SgDotExp* dotNode = isSgDotExp(reference->get_parent());
    if ( dotNode != NULL )
        return true;
    return false;
}

bool
ASTQuerying::isMainFunction
(SgNode* node)
{
    return SageInterface::isMain(node);
}

std::string
ASTQuerying::getNameOfVariableReference
(const SgVarRefExp* reference) 
{
    return reference->get_symbol()->get_name();
}

const SgInitializer* 
ASTQuerying::getInitializerOfInitializedName
(const SgInitializedName* initializedName)
{
    return initializedName->get_initializer();
}

SgName
ASTQuerying::getNameOfInitializedName
(const SgInitializedName* variable)
{
    //TODO make exception
    if ( !variable->search_for_symbol_from_symbol_table() ) return "";
    SgName name = 
        variable->search_for_symbol_from_symbol_table()->get_name();
    return name;
}

SgScopeStatement*
ASTQuerying::getScopeOfVariableReference
(SgVarRefExp* reference)
{
    // Determine the statement of the reference
    SgScopeStatement* scopeStatement = SageInterface::getScope(reference);
    return scopeStatement;
}
