// ControlText.cpp: implementation of the ControlText class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ControlText.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ControlText::ControlText() :
StringRow(NULL)
{}

ControlText::~ControlText()
{}

bool ControlText::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc Locdesc;
   Locdesc.type=LWT_TEXT;
   Locdesc.text.text=StringRow;
   ThisControl=Lightwave3D::panf->addControl (pan, "TextControl", &Locdesc, const_cast<char *>(Name.c_str()));

   return true;
}

void ControlText::Initialize(const char *name, Panel *SetOwner, const char **StringList)
{
   this->StringRow=StringList;
   Control::Initialize(name,SetOwner);
}
