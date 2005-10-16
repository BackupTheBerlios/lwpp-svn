#include "StdAfx.h"
#include "panel.h"

Panel::Panel(const char *name, int INFlags) :
pan(NULL),
Flags(INFlags),
PanelName(name),
ThisWindow(NULL),
IsDragging(false),
Closing(false)
{
   if (!CreatePanel(PanelName))
      return;
}

Panel::~Panel(void) 
{
   if (pan && ( Flags & PANF_BLOCKING ) )
      Lightwave3D::panf->destroy(pan);
}

bool Panel::IsClosing(void)
{
   return Closing;
}

int Panel::Open()
{
   return this->ShowPanel();
}

void Panel::Close()
{
   if (pan)
      Lightwave3D::panf->close(pan);
}

void Panel::Redraw()
{
    if ( this->pan )
        Lightwave3D::panf->draw( this->pan, DR_REFRESH );
}

bool Panel::CreatePanel(std::string &PanelName)
{
   this->pan=Lightwave3D::panf->create(const_cast<char *>(PanelName.c_str()),NULL);
   if (!this->pan)
      return false;

   Lightwave3D::panf->set(pan,PAN_USERDATA,this);

   Lightwave3D::panf->set(pan,PAN_USERDRAW, PanelDraw_Callback );

   Lightwave3D::panf->set(pan,PAN_MOUSEBUTTON, PanelMouse_Callback );

   Lightwave3D::panf->set(pan,PAN_MOUSEMOVE, PanelMouseMove_Callback );

   Lightwave3D::panf->set(pan,PAN_USERKEYS, PanelKeyDown_Callback );

   return true;
}

void Panel::PanelKeyDown_Callback( LWPanelID panel, void *userdata, unsigned short key )
{
   Panel *locPanel=static_cast<Panel *>(userdata);
   locPanel->OnKeyDown(key);
}

void Panel::OnKeyDown(unsigned short key)
{

}

int Panel::ShowPanel()
{
   if (pan)
   {
      if (Flags & PANF_BLOCKING)
      {
         int result=Lightwave3D::panf->open(pan,Flags);
         ThisWindow=GetActiveWindow();//DispInfo.window;      
         return result;
      }
      else
      {
         Lightwave3D::panf->set(pan,PAN_USERCLOSE,PanelClosed_Callback);
         int result=Lightwave3D::panf->open(pan,Flags);
         ThisWindow=GetActiveWindow();//DispInfo.window;      
         return result;
      }
   }
   else
      return 0;
}

//void Panel::OnValueChanged(Control   * ChangedControl)
//{
//}

void Panel::Update( Observable* theChangedSubject )
{
}

void Panel::OnPanelClosed(void)
{
}

void Panel::PanelClosed_Callback( LWPanelID panel, void *UserData )
{
   Panel *locPanel=static_cast<Panel *>(UserData);
   locPanel->Closing=true;
   locPanel->OnPanelClosed();
   delete locPanel;
}

void Panel::OnDraw(int drawmode)
{
}

void Panel::OnMouseEvent(int event, int x, int y)
{
   int debug=1;
}

void Panel::PanelDraw_Callback( LWPanelID panel, void *userdata, int drawmode )
{
   Panel *locPanel=static_cast<Panel *>(userdata);
   locPanel->OnDraw(drawmode);
}

void Panel::PanelMouse_Callback( LWPanelID panel, void *userdata, int qualifiers, int x, int y ) 
{
   Panel *locPanel=static_cast<Panel *>(userdata);
   locPanel->OnMouseEvent(qualifiers, x, y);
}

void Panel::PanelMouseMove_Callback( LWPanelID panel, void *userdata, int qualifiers, int x, int y ) 
{
   Panel *locPanel=static_cast<Panel *>(userdata);
   locPanel->OnMouseEvent(qualifiers, x, y);
}


void Panel::SetPanWidth(int widht)
{
   Lightwave3D::panf->set(pan,PAN_W,&widht);
}

void Panel::SetPanHeight(int height)
{
   Lightwave3D::panf->set(pan,PAN_H,&height);
}

void Panel::GetPanelPosition( int &x, int &y)
{
   Lightwave3D::panf->get(pan,PAN_X,&x);   
   Lightwave3D::panf->get(pan,PAN_Y,&y);   
}

string Panel::PanelClassName = "Panel";



LWPanelID Panel::GetPanelID()
{
   return this->pan;
}

