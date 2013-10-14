#ifndef ASTTEMPORALDATA_H
#define ASTTEMPORALDATA_H

#include <string>
#include <map>

#include "rose.h"
#include "NewVariable.h"
#include "ASTQuerying.h"

class ASTTemporalData
{
private:
    std::map<std::string, const NewVariable*>* oldNewVariables;
	std::map<std::string, const SgInitializer*>* oldInitializers;//constr or value
    std::map<std::string, const SgSymbol*>* oldSymbol;
    std::map<const SgScopeStatement*, std::map<std::string,bool>* >* initVariables;
    std::map<std::string, const SgInitializer*>* initStaticPointers;

public:
    ASTTemporalData();
    virtual ~ASTTemporalData();

    void insertOldNewVariables
        (std::string, const NewVariable*);
    void insertOldInitializers
        (std::string, const SgInitializer*);
    void insertOldSymbol
        (std::string, const SgSymbol*);
    void insertInitVariables
        (const SgScopeStatement* scope, std::string, bool);
    void insertInitVariables
        (SgVarRefExp*, bool);
    void insertInitStaticPointers
        (std::string, const SgInitializer*);

    const NewVariable* getOldNewVariables
        (const std::string&) const;
    const SgInitializer* getOldInitializers
        (const std::string&) const;
    const SgSymbol* getOldSymbol
        (const std::string&) const;
    const SgInitializer* getInitStaticPointers
        (const std::string&) const;
    
    bool isOldNewVariable
        (const std::string&) const;
    bool isOldInitializers
        (const std::string&) const;
    bool isOldSymbol
        (const std::string&) const;
    bool isInitVariables
        (const SgScopeStatement*, const std::string&) const;
    bool isInitVariables
        (SgVarRefExp*) const;
    bool isInitStaticPointers
        (const std::string&) const;

    bool hasBeenVariableRefactored
        (SgVarRefExp* reference) const;
    bool hasBeenVariableRefactoredInScope
        (SgVarRefExp* reference) const;
    bool hasVariableNewName
        (SgVarRefExp* reference) const;

    const NewVariable* getNewVariableOfInitializedName
        (const SgInitializedName*) const;
    const NewVariable* getNewVariableOfVariableReference
        (const SgVarRefExp*) const;
    const SgSymbol* getOldSymbolOfInitializedName
        (const SgInitializedName*) const;
    const SgInitializer* getOldInitializerOfVariableReference
        (const SgVarRefExp*) const;
};

#endif
