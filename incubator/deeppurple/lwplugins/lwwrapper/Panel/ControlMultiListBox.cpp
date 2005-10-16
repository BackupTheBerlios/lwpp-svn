// ControlMultiListBox.cpp: implementation of the ControlMultiListBox class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ControlMultiListBox.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ControlMultiListBox::ControlMultiListBox() :
NumRows(5),
Width(200)
{

}

ControlMultiListBox::~ControlMultiListBox()
{

}

bool ControlMultiListBox::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc desc;
   desc.type=LWT_MLIST;
   desc.multiList.width=Width;
   desc.multiList.nameFn=Name_Callback;
   desc.multiList.countFn=Count_Callback;
   desc.multiList.colWidth=Colwidth_Callback;
   desc.multiList.visItems=NumRows;

   ThisControl=Lightwave3D::panf->addControl (pan, "MultiListBoxControl", &desc, const_cast<char *>(Name.c_str()));

   LWValue  LocVal= { LWT_INTEGER };

      LocVal.ptr.ptr=this;
      ThisControl->set(ThisControl,CTL_USERDATA,&LocVal);

   return true;
}

void ControlMultiListBox::Initialize(const char *name, Panel *SetOwner, int width, int numrows)
{
   Width=width;
   NumRows=numrows;
   Control::Initialize(name,SetOwner);
}


int ControlMultiListBox::SetCount()
{
   return 2;
}
 
char *debugStr3[]={"one","two"};

const char *ControlMultiListBox::SetName( int index, int column )
{
   if (index==-1)
      return debugStr3[0];

   if (index==0)
      return debugStr3[0];

   if (index==1)
      return debugStr3[1];

   return NULL;
}


int ControlMultiListBox::SetColwidth( int index )
{
   if (index<2)
      return Width/2;
   else 
      return 0;
}

////////////////////////////////////////////////////////////
// Callbacks

int ControlMultiListBox::Count_Callback( void *userdata )
{
   if (userdata==NULL)
      return 0;
   else
   {
      ControlMultiListBox *theControl=static_cast<ControlMultiListBox *>(userdata);
      return theControl->SetCount();
   }
   return 0;
}

const char *ControlMultiListBox::Name_Callback( void *userdata, int index, int column)
{
   if (userdata==NULL)
      return 0;
   else
   {
      ControlMultiListBox *theControl=static_cast<ControlMultiListBox *>(userdata);
      return theControl->SetName(index, column);
   }
   return 0;
}

int ControlMultiListBox::Colwidth_Callback( void *userdata, int index )
{
   if (userdata==NULL)
      return 0;
   else
   {
      ControlMultiListBox *theControl=static_cast<ControlMultiListBox *>(userdata);
      return theControl->SetColwidth(index);
   }
   return 20;
}