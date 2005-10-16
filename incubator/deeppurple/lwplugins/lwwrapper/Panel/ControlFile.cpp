#include "StdAfx.h"
#include "controlfile.h"

ControlFile::ControlFile(void) :
Width(200)
{
}

ControlFile::~ControlFile(void)
{
}

bool ControlFile::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locDesc;
   locDesc.type=LWT_STRING;
   locDesc.area.width=Width;
   locDesc.area.height=0;   

   this->ThisControl=Lightwave3D::panf->addControl(pan,"FileControl",&locDesc,const_cast<char *>(Name.c_str()));
   if (this->ThisControl)    
      return true;
   else
      return false;
}

void ControlFile::GetValue(std::string &FileName)
{
   char sBuffer[255];

   LWValue LocSval = { LWT_STRING };
   LocSval.str.buf=sBuffer;
   LocSval.str.bufLen=255;
   ThisControl->get(ThisControl, CTL_VALUE, &LocSval);

   FileName=sBuffer;
}

void ControlFile::Initialize(const char *name, Panel *SetOwner, int width)
{
   Width=width;
   Control::Initialize(name, SetOwner);
}