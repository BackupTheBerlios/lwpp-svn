#include "StdAfx.h"
#include "Control.h"

Control::Control(void) :
ThisControl(NULL),
Name(""),
Owner(NULL),
mXPos(0),
mYPos(0)
{
}

void Control::Initialize(const char *name, Panel *SetOwner)
{
   Name=name;
   Owner=SetOwner;
   RegisterWithPanel(Owner->pan);

   LWValue  LocVal= { LWT_INTEGER };

   if(ThisControl)
   {
      LocVal.ptr.ptr=this;
      ThisControl->set(ThisControl,CTL_USERDATA,&LocVal);

      LocVal.ptr.ptr=NotifyCallback;
      ThisControl->set(ThisControl,CTL_USEREVENT,&LocVal);
   }
}

Control::~Control(void)
{
}
/*
void Control::DoNotifyOwner(bool NotifyOwner)
{ 
   LWValue  LocVal= { LWT_INTEGER };

   if(NotifyOwner)
   {
      LocVal.ptr.ptr=this;
      ThisControl->set(ThisControl,CTL_USERDATA,&LocVal);

      LocVal.ptr.ptr=NotifyCallback;
      ThisControl->set(ThisControl,CTL_USEREVENT,&LocVal);
   }
   else
   {
      LocVal.ptr.ptr=NULL;
      ThisControl->set(ThisControl,CTL_USERDATA,&LocVal);

      LocVal.ptr.ptr=NULL;
      ThisControl->set(ThisControl,CTL_USEREVENT,&LocVal);
   }
}
*/

void Control::NotifyCallback(LWControl *cnt, void *UserData)
{
   Control *locControl=static_cast<Control *>(UserData);
   locControl->notifyObservers();
}



void Control::MoveControl(int x, int y)
{
   LWValue  LocVal= { LWT_INTEGER };
   LocVal.intv.value=x;
   ThisControl->set(ThisControl,CTL_X,&LocVal);
   LocVal.intv.value=y;
   ThisControl->set(ThisControl,CTL_Y,&LocVal);

   mXPos = x;
   mYPos = y;
}

void Control::SetPositionWithoutCaption(int x, int y)
{   
    int textpixels = 0;

    if (this->Owner)
    {
        textpixels = Lightwave3D::panf->drawFuncs->textWidth( this->Owner, const_cast<char*>(this->Name.c_str()) );
    }

    LWValue  LocVal= { LWT_INTEGER };
    LocVal.intv.value=x - textpixels - 6; //edited by Elmar
    ThisControl->set(ThisControl,CTL_X,&LocVal);
    LocVal.intv.value=y;
    ThisControl->set(ThisControl,CTL_Y,&LocVal);

}

void Control::ChangeSizeWithoutCaption(int x, int y)
{
    int textpixels = 0;

    if (this->Owner)
    {
        textpixels = Lightwave3D::panf->drawFuncs->textWidth( this->Owner, const_cast<char*>(this->Name.c_str()) );
    }

    LWValue  LocVal= { LWT_INTEGER };
    LocVal.intv.value=x + textpixels + 6;//edited by Elmar
    ThisControl->set(ThisControl,CTL_W,&LocVal);
    LocVal.intv.value=y;
    ThisControl->set(ThisControl,CTL_H,&LocVal);
}


void Control::GetControlSize(int &w, int &h)
{
   LWValue  LocVal= { LWT_INTEGER };
   ThisControl->get(ThisControl,CTL_W,&LocVal);
   w=LocVal.intv.value;

   ThisControl->get(ThisControl,CTL_H,&LocVal); 
   h=LocVal.intv.value;
}

void Control::GetActiveControlSize(int &w, int &h)
{
   LWValue  LocVal= { LWT_INTEGER };
   ThisControl->get(ThisControl,CTL_HOTW,&LocVal);
   w=LocVal.intv.value;

   ThisControl->get(ThisControl,CTL_HOTH,&LocVal); 
   h=LocVal.intv.value;
}


bool Control::ChangeSize(int x, int y)
{
   LWValue  LocVal= { LWT_INTEGER };
   LocVal.intv.value=x;
   ThisControl->set(ThisControl,CTL_W,&LocVal);
   LocVal.intv.value=y;
   ThisControl->set(ThisControl,CTL_H,&LocVal);
   return true;
}

// Shows Control Ghosted
void Control::GhostControl()
{
   GHOST_CON( this->ThisControl );
}

// Shows the Control 
void Control::UnGhostControl()
{
UNGHOST_CON( this->ThisControl );
}

// Redraws Control in curent state
void Control::Refresh()
{
   REDRAW_CON( this->ThisControl );
}

int  Control::GetLabelWidth()
{
   LWValue  LocVal= { LWT_INTEGER };
   this->ThisControl->get(this->ThisControl, CTL_LABELWIDTH, &LocVal);
   return LocVal.intv.value;
}

// Hides the Control
void Control::Hide()
{
this->ThisControl->draw(this->ThisControl,DR_ERASE);
}

// Shows the Control 
void Control::UnHide()
{
   
   this->ThisControl->draw(this->ThisControl,DR_RENDER);
}

void Control::GetControlPosition(int &x, int &y)
{
   LWValue  LocVal= { LWT_INTEGER };

   this->ThisControl->get(ThisControl, CTL_X, &LocVal);
   x=LocVal.intv.value;

   this->ThisControl->get(ThisControl, CTL_Y, &LocVal);
   y=LocVal.intv.value;
}

void Control::GetActiveControlPosition(int &x, int &y)
{
   LWValue  LocVal= { LWT_INTEGER };

   this->ThisControl->get(ThisControl, CTL_HOTX, &LocVal);
   x=LocVal.intv.value;

   this->ThisControl->get(ThisControl, CTL_HOTY, &LocVal);
   y=LocVal.intv.value;
}


bool Control::IsInside(int x, int y)
{
   int Lx,Ly,Rx,Ry;
   LWValue  LocVal= { LWT_INTEGER };

   this->ThisControl->get(ThisControl, CTL_HOTX, &LocVal);
   Lx=LocVal.intv.value;

   this->ThisControl->get(ThisControl, CTL_HOTY, &LocVal);
   Ly=LocVal.intv.value;

   this->ThisControl->get(ThisControl, CTL_HOTW, &LocVal);
   Rx=Lx+LocVal.intv.value;

   this->ThisControl->get(ThisControl, CTL_HOTH, &LocVal);
   Ry=Ly+LocVal.intv.value;

   if (x>Lx && y>Ly && x<Rx && y<Ry)
      return true;
   else 
      return false;

}

void Control::SetRange(int min,int max)
{
   int Min=min;
   int Max=max;

   LWValue locival    = { LWT_INTEGER };
   locival.intv.value=Min;
   this->ThisControl->set(this->ThisControl,CTL_RANGEMIN, &locival);
   locival.intv.value=Max;
   this->ThisControl->set(this->ThisControl,CTL_RANGEMAX, &locival);
}
