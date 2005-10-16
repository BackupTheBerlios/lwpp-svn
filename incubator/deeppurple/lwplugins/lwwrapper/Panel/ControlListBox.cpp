// ControlListBox.cpp: implementation of the ControlListBox class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ControlListBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ControlListBox::ControlListBox() :
Width(50),
Height(5)
{

}

ControlListBox::~ControlListBox()
{

}

bool ControlListBox::RegisterWithPanel(LWPanelID pan)
{

   LWPanControlDesc locdesc;
   locdesc.type=LWT_POPUP;
   locdesc.listbox.width=Width;
   locdesc.listbox.nameFn=Name_Callback;
   locdesc.listbox.countFn=Count_Callback;
   locdesc.listbox.visItems=Height;

   ThisControl=Lightwave3D::panf->addControl (pan, "ListBoxControl", &locdesc, const_cast<char *>(Name.c_str()));

   LWValue  LocVal= { LWT_INTEGER };

      LocVal.ptr.ptr=this;
      ThisControl->set(ThisControl,CTL_USERDATA,&LocVal);

   return true;
}

char *debugStr2[]={"one","two"};

int ControlListBox::SetCount()
{
   return 2;
}

char *ControlListBox::SetName( int index )
{
   if (index==0)
      return debugStr2[0];

   if (index==1)
      return debugStr2[1];

   return NULL;
}

int ControlListBox::Count_Callback( void *userdata )
{
   if (userdata==NULL)
      return 0;
   else
   {
      ControlListBox *theControl=static_cast<ControlListBox *>(userdata);
      return theControl->SetCount();
   }
}

char *ControlListBox::Name_Callback( void *userdata, int index )
{
   if (userdata==NULL)
      return 0;
   else
   {
      ControlListBox *theControl=static_cast<ControlListBox *>(userdata);
      return theControl->SetName(index);
   }
}
