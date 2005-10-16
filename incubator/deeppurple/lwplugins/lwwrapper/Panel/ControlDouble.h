// ControlDouble.h: interface for the ControlDouble class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ControlDouble_H__D7E31386_D386_43ED_86D2_527A7805B157__INCLUDED_)
#define AFX_ControlDouble_H__D7E31386_D386_43ED_86D2_527A7805B157__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#include "Control.h"

class ControlDouble : public Control
{
public:
	ControlDouble();
	virtual ~ControlDouble();
   double GetValue(void);
   void SetValue(double Angle);
   bool RegisterWithPanel(LWPanelID pan);
};

#endif
