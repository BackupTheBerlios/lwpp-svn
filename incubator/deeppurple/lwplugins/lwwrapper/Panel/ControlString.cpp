#include "StdAfx.h"
#include "controlstring.h"

ControlString::ControlString(void) :
Width(50),
IsReadOnly(false)
{
}

ControlString::~ControlString(void)
{
}

bool ControlString::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc Locdesc;
   Locdesc.type=LWT_STRING;
   Locdesc.string.width=Width;
   if (this->IsReadOnly)
      ThisControl=Lightwave3D::panf->addControl (pan, "InfoControl", &Locdesc, const_cast<char *>(Name.c_str()));
   else
      ThisControl=Lightwave3D::panf->addControl (pan, "EditControl", &Locdesc, const_cast<char *>(Name.c_str()));
   return true;
}

bool ControlString::GetValue (string &LName)
{
   LWValue locsval = { LWT_STRING };
   char sBuffer[1000];
   locsval.str.buf=sBuffer;
   locsval.str.bufLen=1000;
   ThisControl->get(ThisControl, CTL_VALUE, &locsval);
   LName=sBuffer;
   return true;
}

bool ControlString::SetValue (string &newString)
{
   LWValue locsval = { LWT_STRING };
   locsval.str.buf=const_cast<char *>(newString.c_str());
   locsval.str.bufLen=static_cast<int>( newString.length() );
   ThisControl->set(ThisControl, CTL_VALUE, &locsval);
   return true;
}

void ControlString::Initialize(const char *name, Panel *SetOwner, int width, bool isreadonly)
{
   this->Width=width;
   this->IsReadOnly=isreadonly;
   Control::Initialize(name,SetOwner);
}