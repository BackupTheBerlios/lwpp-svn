// ontrolBoolChoice.h: interface for the ControlBoolChoice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ONTROLBOOLCHOICE_H__34E657A7_7023_47C5_892B_FAAF3D5C9ECE__INCLUDED_)
#define AFX_ONTROLBOOLCHOICE_H__34E657A7_7023_47C5_892B_FAAF3D5C9ECE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ControlChoices.h"

class ControlBoolChoice : public ControlChoices  
{
public:
	ControlBoolChoice();
	virtual ~ControlBoolChoice();
   virtual bool RegisterWithPanel(LWPanelID pan);
};

#endif // !defined(AFX_ONTROLBOOLCHOICE_H__34E657A7_7023_47C5_892B_FAAF3D5C9ECE__INCLUDED_)
