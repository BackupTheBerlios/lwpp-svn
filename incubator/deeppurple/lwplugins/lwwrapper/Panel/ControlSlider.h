// Copyright (C) 1999 - 2002 David Forstenlechner

#if defined (_MSC_VER) && (_MSC_VER >= 1000)
#pragma once
#endif
#ifndef _INC_CONTROLSLIDER_3E2E5FBD007D_INCLUDED
#define _INC_CONTROLSLIDER_3E2E5FBD007D_INCLUDED

#include "Control.h"

class ControlSlider : 
public Control  
  
{
public:

   ControlSlider();
   virtual void Initialize(const char *name, Panel *SetOwner, int min, int max, int width, bool EntryField=false);

	int GetValue();
	void SetValue(int Val);

	virtual bool RegisterWithPanel(LWPanelID pan);

// CAUTION will not be updated
   int Width, Min, Max;

   bool EntryField;
};

#endif /* _INC_CONTROLSLIDER_3E2E5FBD007D_INCLUDED */
