#include "ASTTemporalData.h"

ASTTemporalData::ASTTemporalData()
{
    oldNewVariables = new std::map<std::string, const NewVariable*>();
	oldInitializers = new std::map<std::string, const SgInitializer*>();
    oldSymbol = new std::map<std::string, const SgSymbol*>();
    initVariables = new std::map<const SgScopeStatement*
        , std::map<std::string,bool>* >();
}

ASTTemporalData::~ASTTemporalData()
{
    if ( oldNewVariables )
    {
        delete oldNewVariables;
        oldNewVariables = NULL;
    }
    if ( oldInitializers )
    {
        delete oldInitializers;
        oldInitializers = NULL;
    }
    if ( oldSymbol )
    {
        delete oldSymbol;
        oldSymbol = NULL;
    }
    if ( initVariables )
    {
        //TODO: delete sub maps
        delete initVariables;
        initVariables = NULL;
    }
}

void 
ASTTemporalData::insertOldNewVariables
(std::string name, const NewVariable* newVariable)
{
    oldNewVariables->insert( 
        std::pair<std::string, const NewVariable*>(name, newVariable));
}

void
ASTTemporalData::insertOldInitializers
(std::string name, const SgInitializer* initializer)
{
    oldInitializers->insert(
        std::pair<std::string, const SgInitializer*>(name, initializer));
}

void
ASTTemporalData::insertOldSymbol
(std::string name, const SgSymbol* symbol)
{
    oldSymbol->insert(std::pair<std::string, const SgSymbol*> (name, symbol));
}

void
ASTTemporalData::insertInitVariables
(const SgScopeStatement* scope, std::string name, bool value)
{
    if ( !( initVariables->count(scope) > 0) )
    {
        // The map does not has the scope. Create one
        std::map<std::string, bool>* subInitVariables = 
            new std::map<std::string, bool>();       
        initVariables->insert(std::pair<const SgScopeStatement*,
            std::map<std::string, bool>* > (scope, subInitVariables));
    }
    std::map<const SgScopeStatement*, std::map<std::string, bool>* >::iterator 
        iter = initVariables->find(scope);
    ((*iter).second)->insert(std::pair<std::string, bool> (name, value));
}

void
ASTTemporalData::insertInitVariables
(SgVarRefExp* reference, bool value)
{
    const SgScopeStatement* scope = 
        ASTQuerying::getScopeOfVariableReference(reference);
    SgName name = ASTQuerying::getNameOfVariableReference(reference);
    insertInitVariables(scope, name, value);
}

void
ASTTemporalData::insertInitStaticPointers
(std::string name, const SgInitializer* initializer)
{
    initStaticPointers->insert(std::pair<std::string, const SgInitializer*>(name
        , initializer));
}

const NewVariable* 
ASTTemporalData::getOldNewVariables
(const std::string& name)
const
{
    if (! isOldNewVariable( name ) )
        return NULL;
    std::map<std::string, const NewVariable*>::const_iterator it = 
        oldNewVariables->find(name);
    return (*it).second;
}

const SgInitializer* 
ASTTemporalData::getOldInitializers
(const std::string& name)
const
{
    if (! isOldInitializers( name ) )
        return NULL;
    std::map<std::string, const SgInitializer*>::const_iterator it = 
        oldInitializers->find(name);
    return (*it).second;
}

const SgSymbol*
ASTTemporalData::getOldSymbol
(const std::string& name)
const
{
    if (! isOldSymbol( name ) )
        return NULL;
    std::map<std::string, const SgSymbol*>::const_iterator it = 
        oldSymbol->find(name);
    return (*it).second;
}

const SgInitializer*
ASTTemporalData::getInitStaticPointers
(const std::string& name)
const
{

    if (! isInitStaticPointers( name ) )
        return NULL;
    std::map<std::string, const SgInitializer*>::const_iterator it =
        initStaticPointers->find(name);
    return (*it).second;
}
    
bool
ASTTemporalData::isOldNewVariable
(const std::string& name)
const
{
    if ( oldNewVariables->count ( name ) > 0)
        return true;
    return false;
}

bool
ASTTemporalData::isOldInitializers
(const std::string& name)
const
{
    if ( oldInitializers->count ( name ) > 0 )
        return true;
    return false;
}

bool
ASTTemporalData::isOldSymbol
(const std::string& name)
const
{
    if ( oldSymbol->count ( name ) > 0 )
        return true;
    return false;
}

bool
ASTTemporalData::isInitVariables
(const SgScopeStatement* scope, const std::string& name)
const
{
    if ( initVariables->count ( scope ) > 0 )
    {
        std::map<const SgScopeStatement*, std::map<std::string, bool>* 
            >::iterator iter = initVariables->find(scope);
        if ( ((*iter).second)->count ( name ) > 0 )
            return true;
    }
    return false;
}

bool
ASTTemporalData::isInitVariables
(SgVarRefExp* reference)
const
{
    SgScopeStatement* scope = 
        ASTQuerying::getScopeOfVariableReference(reference);
    SgName name = ASTQuerying::getNameOfVariableReference(reference);
    return isInitVariables(scope, name);
}

bool 
ASTTemporalData::isInitStaticPointers
(const std::string& name)
const
{
    if ( initStaticPointers->count( name ) > 0 )
        return true;
    return false;
}

bool
ASTTemporalData::hasBeenVariableRefactored
(SgVarRefExp* reference)
const
{
    std::string name = ASTQuerying::getNameOfVariableReference(reference);
    return isOldNewVariable(name);
}

// This function determines if the variable name of the current reference is 
// an already updated name. This could happen if other referece has been updated
// in the same scope.
// TODO Maybe I should change the name of this function
bool
ASTTemporalData::hasBeenVariableRefactoredInScope
(SgVarRefExp* reference)
const
{
    std::string name = ASTQuerying::getNameOfVariableReference(reference);
    std::map<std::string, const NewVariable*>::const_iterator iter = 
        oldNewVariables->begin();
    std::map<std::string, const NewVariable*>::const_iterator end = 
        oldNewVariables->end();
    for(; iter != end; ++iter)
    {
        const NewVariable* newVariable = (*iter).second;
        if ( name.compare(newVariable->getName()) == 0 )
            return true;
    }
    return false;
}

bool
ASTTemporalData::hasVariableNewName
(SgVarRefExp* reference)
const
{
    std::string name = ASTQuerying::getNameOfVariableReference(reference);
    if ( isOldNewVariable( name ) )
    {
        const NewVariable* newVariable = getOldNewVariables( name );
        std::string newName = newVariable->getName();
        if ( newName.compare(name) != 0 )
            return true;
    }
    return false;
}

const NewVariable*
ASTTemporalData::getNewVariableOfInitializedName
(const SgInitializedName* initializedName)
const
{
    SgName name = ASTQuerying::getNameOfInitializedName(initializedName);
    if ( isOldNewVariable( name ) )
        return getOldNewVariables(name);
    return NULL;
}

const NewVariable* 
ASTTemporalData::getNewVariableOfVariableReference
(const SgVarRefExp* varRefExp)
const
{
    SgName name = ASTQuerying::getNameOfVariableReference(varRefExp);
    if ( isOldNewVariable( name ) )
        return getOldNewVariables(name);
    return NULL;
}

const SgSymbol*
ASTTemporalData::getOldSymbolOfInitializedName
(const SgInitializedName* initializedName)
const
{
    SgName name = ASTQuerying::getNameOfInitializedName(initializedName);
    if ( isOldSymbol( name ) )
        return getOldSymbol(name);
    return NULL;
}

const SgInitializer*
ASTTemporalData::getOldInitializerOfVariableReference
(const SgVarRefExp* varRefExp)
const
{
    SgName name = ASTQuerying::getNameOfVariableReference(varRefExp);
    if ( isOldInitializers( name ) )
        return getOldInitializers( name );
    std::cout << "constructor null" << std::endl;
    return NULL;
}
