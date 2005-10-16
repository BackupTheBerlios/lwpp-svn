// Copyright (C) 1999 - 2002 David Forstenlechner

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_XCONTROLBUTTON_3E1AA79B02D8_INCLUDED
#define _INC_XCONTROLBUTTON_3E1AA79B02D8_INCLUDED

#include "XControl.h"

class XPanel;

class XControlButton : 
public XControl  
  
{
public:
	virtual void Initialize(const char* name, XPanel* owner);

	static void OnButtonPressed(LWXPanelID owner, unsigned long cid);

	XControlButton();

};

#endif /* _INC_XCONTROLBUTTON_3E1AA79B02D8_INCLUDED */
