// ControlCustPopUp.h: interface for the ControlCustPopUp class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CONTROLCUSTPOPUP_H
#define CONTROLCUSTPOPUP_H

#include "ControlChoices.h"

class ControlCustPopUp : public ControlChoices  
{
public:
	ControlCustPopUp();
	virtual ~ControlCustPopUp();
   virtual bool RegisterWithPanel(LWPanelID pan);
   virtual void Initialize(const char *name, Panel *SetOwner, int width=100);

   static int Count_Callback( void *userdata );
   static const char *Name_Callback( void *userdata, int index );

   virtual int OnItemCount();
   virtual const char *OnGetEntries(int index);

protected:
   int Width;
   int numEntries;
   vector<string> Entries;
};

#endif
