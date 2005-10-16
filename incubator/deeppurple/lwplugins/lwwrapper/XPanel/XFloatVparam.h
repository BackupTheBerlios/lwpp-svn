// Copyright (C) 1999 - 2002 David Forstenlechner

#ifndef _INC_XFLOATVPARAM_3E34046C009F_INCLUDED
#define _INC_XFLOATVPARAM_3E34046C009F_INCLUDED

#include "XControl.h"

class XPanel;

class XFloatVparam : 
public XControl  
  
{
public:
	XFloatVparam();

	~XFloatVparam();

	virtual void Initialize(const char* name, XPanel* owner);

};

#endif /* _INC_XFLOATVPARAM_3E34046C009F_INCLUDED */
