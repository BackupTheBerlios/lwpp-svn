// ControlListBox.h: interface for the ControlListBox class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CONTROLLISTBOX_INCLUDED
#define CONTROLLISTBOX_INCLUDED

#include "Control.h"

class ControlListBox : public Control  
{
public:
	ControlListBox();
	virtual ~ControlListBox();
   virtual bool RegisterWithPanel(LWPanelID pan);

   virtual int SetCount();
   virtual char *SetName(int index );

   static int Count_Callback( void *userdata );
   static char *Name_Callback( void *userdata, int index );

   int Width;
   int Height;
};

#endif // CONTROLLISTBOX_INCLUDED
