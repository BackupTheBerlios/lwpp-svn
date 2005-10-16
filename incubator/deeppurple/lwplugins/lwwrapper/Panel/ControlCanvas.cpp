#include "StdAfx.h"
#include "controlcanvas.h"

ControlCanvas::ControlCanvas(void) :
Width(256),
Height(256)
{
}

ControlCanvas::~ControlCanvas(void)
{
}

bool ControlCanvas::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locdesc;
   locdesc.type=LWT_AREA;
   locdesc.area.width=Width;
   locdesc.area.height=Height;
   this->ThisControl=Lightwave3D::panf->addControl(this->Owner->pan, "CanvasControl", &locdesc, const_cast<char *>(Name.c_str()));
   LWValue Locival    = { LWT_INTEGER };
	Locival.ptr.ptr = DrawCallback;
   this->ThisControl->set( ThisControl, CTL_USERDRAW, &Locival );
	Locival.ptr.ptr = this;              
   this->ThisControl->set( ThisControl, CTL_USERDATA, &Locival );
   return true;
}

void ControlCanvas::DrawCallback( LWControl *control, void *UserData, int drawmode )
{
   ControlCanvas *locControl=static_cast<ControlCanvas *>(UserData);
   locControl->Draw();
}

void ControlCanvas::Draw()
{
   int i=0;
}

void ControlCanvas::Initialize(const char *name, Panel *SetOwner, int width, int height)
{
   Width=width;
   Height=height;
   Control::Initialize(name,SetOwner);
}

void ControlCanvas::DrawPoint(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    Lightwave3D::panf->drawFuncs->drawRGBPixel(this->Owner->pan,r,g,b,x+mXPos+4,y+mYPos+6);
}