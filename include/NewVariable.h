//
//  newVariable.h
//  
//
//  Created by Pablo Torres Tramón on 7/13/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef NEWVARIABLE_H
#define NEWVARIABLE_H

#include "rose.h"
#define GLOBAL_SCOPE_NAME "SgGlobal"

class NewVariable 
{
private:
	SgInitializedName* initializedName;
	SgName name;
	SgType* type;
    
public:
	SgInitializedName* getInitializedName
        () const;
	SgName getName
        () const;
	SgType* getType
        () const;
	void setInitializedName
        (SgInitializedName* initializedName);
	void setName
        (SgName name);
	void setType
        (SgType* type);
};


#endif
