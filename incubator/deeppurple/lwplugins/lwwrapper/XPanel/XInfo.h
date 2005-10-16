#ifndef XPANEL_XINFO_H_HEADER_INCLUDED_C127722B
#define XPANEL_XINFO_H_HEADER_INCLUDED_C127722B

#include "XControl.h"

class XInfo : public XControl
{
 public:
   XInfo();

	virtual void Initialize(const char * name, XPanel * owner);
};



#endif /* XPANEL_XINFO_H_HEADER_INCLUDED_C127722B */
