#ifndef ASTMODIFYING_H
#define ASTMODIFYING_H

#include "rose.h"
#include "ASTQuerying.h"
#include "ASTTemporalData.h"

class ASTModifying
{
public:
    static void setVariableToTLS
        (SgVariableDeclaration* variableDeclaration);
    static void setVariableToTLS
        (SgInitializedName* initializedName);
    static void setVariableToStatic
        (SgVariableDeclaration* variableDeclaration);
    static SgIfStmt* createVariableIntialisation
        (SgVarRefExp*, const SgInitializer*);
    static void dotReferenceToArrowReference
        (SgVarRefExp*);
    static SgVarRefExp* createVariableReference
        (SgVarRefExp*, const NewVariable*);
    static SgVariableDeclaration* createCopyVariableDeclaration
        (SgInitializedName*, const NewVariable*);
    static SgVariableDeclaration* createTSPointerForInitializedName
        (SgInitializedName*, SgName);
    static void replaceDeclaration
        (SgVariableDeclaration*, SgVariableDeclaration*);
    static void replaceReference
        (SgVarRefExp* reference, SgVarRefExp* newReference);
    static void addExpressionBeforeReference
        (SgVarRefExp*, SgStatement*);
};

#endif
