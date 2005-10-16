#include "StdAfx.h"
#include "ControlCustPopUp.h"

ControlCustPopUp::ControlCustPopUp() :
Width(100)
{
}

ControlCustPopUp::~ControlCustPopUp()
{

}

bool ControlCustPopUp::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locdesc;
   locdesc.type=LWT_POPUP;
   locdesc.popup.width=Width;
   locdesc.popup.countFn=Count_Callback;
   locdesc.popup.nameFn=Name_Callback;
   ThisControl=Lightwave3D::panf->addControl (pan, "CustomPopupControl", &locdesc, const_cast<char *>(Name.c_str()));
   return true;
}

void ControlCustPopUp::Initialize(const char *name, Panel *SetOwner, int width)
{
   Width=width;
   Control::Initialize(name,SetOwner);
}

int ControlCustPopUp::OnItemCount()
{
   return 0;
}

int ControlCustPopUp::Count_Callback( void *userdata )
{
   ControlCustPopUp *theControl=static_cast<ControlCustPopUp *>(userdata);

   while (!theControl->Entries.empty())  // use Entries.clear() instead?
      theControl->Entries.pop_back();

   return theControl->OnItemCount();
}

const char *ControlCustPopUp::OnGetEntries(int index)
{
   return NULL;
}

const char *ControlCustPopUp::Name_Callback( void *userdata, int index )
{
   ControlCustPopUp *theControl=static_cast<ControlCustPopUp *>(userdata);

   const char *buffer=theControl->OnGetEntries(index);
   if (buffer)
   {
      theControl->Entries.push_back(buffer);
      return (*theControl->Entries.rbegin()).c_str();
   }
   else 
      return NULL;
}
