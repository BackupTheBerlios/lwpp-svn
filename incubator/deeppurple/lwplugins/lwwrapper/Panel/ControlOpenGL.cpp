#include "StdAfx.h"
#include "Controlopengl.h"

ControlOpenGL::ControlOpenGL(void) :
Width(100),
Height(100)
{
}

ControlOpenGL::~ControlOpenGL(void)
{
}

void ControlOpenGL::Initialize(const char *name, Panel *SetOwner, int width, int height)
{
   Width=width;
   Height=height;
   Control::Initialize(name,SetOwner);
}

bool ControlOpenGL::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locdesc;
   locdesc.type=LWT_AREA;
   locdesc.area.width=Width;
   locdesc.area.height=Height;
   this->ThisControl=Lightwave3D::panf->addControl(this->Owner->pan, "OpenGLControl", &locdesc, const_cast<char *>(Name.c_str()));
   return true;
}

