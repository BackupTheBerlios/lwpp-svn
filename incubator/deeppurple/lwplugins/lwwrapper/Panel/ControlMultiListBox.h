// ControlMultiListBox.h: interface for the ControlMultiListBox class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CONTROLMULTILISTBOX_INCLUDED
#define CONTROLMULTILISTBOX_INCLUDED

#include "ControlInteger.h"

class ControlMultiListBox : public ControlInteger
{
public:
	ControlMultiListBox();
	virtual ~ControlMultiListBox();
   virtual bool RegisterWithPanel(LWPanelID pan);
   virtual void Initialize(const char *name, Panel *SetOwner, int width=100, int numrows=5);

   virtual int SetCount();
   virtual const char *SetName(int index, int column );
   virtual int SetColwidth( int index );

   static int Count_Callback( void *userdata );
   static const char *Name_Callback( void *userdata, int index, int column );
   static int Colwidth_Callback( void *userdata, int index );

   int Width;
   int NumRows;

};

#endif // CONTROLMULTILISTBOX_INCLUDED
