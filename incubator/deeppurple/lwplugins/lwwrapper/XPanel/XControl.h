#ifndef _INC_XCONTROL_3E1AA55A01D4_INCLUDED
#define _INC_XCONTROL_3E1AA55A01D4_INCLUDED

#include "XPanel.h"

class XControl 
{
public:
	unsigned long ID;

	string Name;

	string Type;

	string DataType;

	XControl();

	virtual ~XControl();

	virtual void Initialize(const char * name, XPanel * owner);

protected:
	XPanel * Owner;

};

#endif /* _INC_XCONTROL_3E1AA55A01D4_INCLUDED */
