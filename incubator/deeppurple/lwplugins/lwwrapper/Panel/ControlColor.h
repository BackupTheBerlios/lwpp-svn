// ControlColor.h: interface for the ControlColor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLCOLOR_H__9434D5F5_BDBA_40E0_92A2_DBC560463595__INCLUDED_)
#define AFX_CONTROLCOLOR_H__9434D5F5_BDBA_40E0_92A2_DBC560463595__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Control.h"

class ControlColor : public Control  
{
public:
	ControlColor();
	virtual ~ControlColor();
   bool RegisterWithPanel(LWPanelID pan);
};

#endif
