//
//  NewVariable.cc
//  
//
//  Created by Pablo Torres Tramón on 7/13/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "NewVariable.h"

SgInitializedName* 
NewVariable::getInitializedName()
const
{
	return initializedName;
}

SgName
NewVariable::getName()
const
{
	return name;
}

SgType* 
NewVariable::getType()
const
{
	return type;
}

void 
NewVariable::setInitializedName
(SgInitializedName* initializedName) 
{
	this->initializedName = initializedName;
}

void
NewVariable::setName
(SgName name)
{
	this->name = name;
}

void
NewVariable::setType
(SgType* type)
{
	this->type = type;
}

