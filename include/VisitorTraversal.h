//
//  VisitorTraversal.h
//  
//
//  Created by Pablo Torres Tramón on 7/13/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef VISITORTRAVERSAL_H
#define VISITORTRAVERSAL_H

#include <string>
#include <map>

#include "rose.h"
#include "NewVariable.h"
#include "ASTQuerying.h"
#include "ASTModifying.h"
#include "ASTTemporalData.h"

#define GLOBAL_SCOPE_NAME "SgGlobal"

class VisitorTraversal : public SgSimpleProcessing {    
private:
    ASTTemporalData* temporalData;

public:
	VisitorTraversal();
	virtual ~VisitorTraversal();

    NewVariable* generateNewVariable
        (SgName, SgType*, SgInitializedName*) const;
    void storeInitializer
        (SgInitializedName*);
    void storeNewVariable
        (SgVariableDeclaration*, SgName);
    void storeNewVariable
        (SgInitializedName*, SgName);
    void storeSymbol
        (SgInitializedName*, SgName);
    void transformPOD
        (SgInitializedName*);
    void transformClass
        (SgInitializedName*, SgVariableDeclaration*);
    void transformStaticInitialisation
        (SgInitializedName*, SgVariableDeclaration*);
    void analyseDeclaration
        (SgNode*);
    void transformReference
        (SgVarRefExp* reference);
    void analyseReference
        (SgNode*);
	virtual void visit
        (SgNode*);
	virtual void atTraversalEnd
        ();
};


#endif
