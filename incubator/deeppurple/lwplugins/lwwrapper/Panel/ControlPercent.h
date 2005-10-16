// ControlPercent.h: interface for the ControlPercent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTROLPERCENT_H__F4913AB0_2EAD_4FCF_931E_79C11DA9BD01__INCLUDED_)
#define AFX_CONTROLPERCENT_H__F4913AB0_2EAD_4FCF_931E_79C11DA9BD01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ControlDouble.h"

class ControlPercent : public ControlDouble  
{
public:
	ControlPercent();
	virtual ~ControlPercent();
   virtual bool RegisterWithPanel(LWPanelID pan);
   virtual void Initialize(const char *name, Panel* SetOwner, int min=0, int max=100);
   int Min,Max;
};

#endif // !defined(AFX_CONTROLPERCENT_H__F4913AB0_2EAD_4FCF_931E_79C11DA9BD01__INCLUDED_)
