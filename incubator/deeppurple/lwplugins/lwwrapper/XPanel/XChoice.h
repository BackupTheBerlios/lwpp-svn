// Copyright (C) 1999 - 2002 David Forstenlechner

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_XCHOICE_3E37B6BE00A2_INCLUDED
#define _INC_XCHOICE_3E37B6BE00A2_INCLUDED

#include "XControl.h"

class XPanel;

class XChoice : 
public XControl 
  
{
public:
	XChoice();
	virtual void Initialize(const char* name, XPanel* owner,const char *choices[]);

};

#endif /* _INC_XCHOICE_3E37B6BE00A2_INCLUDED */
