// ontrolBoolChoice.cpp: implementation of the ControlBoolChoice class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ControlBoolChoice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ControlBoolChoice::ControlBoolChoice()
{
}

ControlBoolChoice::~ControlBoolChoice()
{
}

bool ControlBoolChoice::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locdesc;
   locdesc.type=LWT_CHOICE;
   locdesc.choice.vertical=MX_HCHOICE;
   locdesc.choice.items=const_cast<const char **>(Choices);
   ThisControl=Lightwave3D::panf->addControl (pan, "ChoiceControl", &locdesc, const_cast<char *>(Name.c_str()));
   return true;
}