#ifndef ASTQUERYING_H
#define ASTQUERYING_H

#include <string>
#include <map>

#include "rose.h"
#include "NewVariable.h"

class ASTQuerying
{
public:
    static bool isThereDefinedSymbol
        (const SgStatement*, const NewVariable*);
    static bool isStaticVariableReference
        (const SgVarRefExp*);
    static bool isStaticVariableInitialisation
        (const SgInitializedName*, const SgSymbol*);
    static bool isStaticVariableDeclaration
        (const SgInitializedName*);
    static bool isVariableDeclaration
        (const SgNode*);
    static bool isVariableReference
        (const SgNode*);
    static bool isPodType
        (SgType*);
    static bool isPointerType
        (SgType*);
    static bool isVariableReferenceInDotExpression
        (SgVarRefExp* reference);
    static bool isMainFunction
        (SgNode*);
    
    static std::string getNameOfVariableReference
        (const SgVarRefExp*);
    static const SgInitializer* getInitializerOfInitializedName
        (const SgInitializedName*);
    static SgName getNameOfInitializedName
        (const SgInitializedName* variable);
    static SgScopeStatement* getScopeOfVariableReference
        (SgVarRefExp* reference);
};

#endif

