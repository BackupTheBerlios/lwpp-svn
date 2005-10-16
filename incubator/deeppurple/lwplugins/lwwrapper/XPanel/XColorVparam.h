// XColorVparam.h: interface for the XColorVparam class.
//
//////////////////////////////////////////////////////////////////////

#ifndef XCOLORVPARAM_INCLUDED
#define XCOLORVPARAM_INCLUDED


#include "XControl.h"

class XColorVparam : 
public XControl  
{
public:
	XColorVparam();
	virtual ~XColorVparam();
	virtual void Initialize(const char* name, XPanel* owner);
};

#endif // XCOLORVPARAM_INCLUDED
