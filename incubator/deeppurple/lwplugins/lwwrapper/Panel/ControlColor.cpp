// ControlColor.cpp: implementation of the ControlColor class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ControlColor.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ControlColor::ControlColor()
{

}

ControlColor::~ControlColor()
{

}

bool ControlColor::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locDesc;
   locDesc.type=LWT_VINT;

   this->ThisControl=Lightwave3D::panf->addControl(pan,"MiniRGBControl",&locDesc,const_cast<char *>(Name.c_str()));
   if (this->ThisControl)    
      return true;
   else
      return false;
}
