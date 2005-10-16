#pragma once
#include "controlcanvas.h"

class ControlOpenGL :
   public ControlCanvas
{
public:
   ControlOpenGL(void);
   virtual ~ControlOpenGL(void);

   virtual void Initialize(const char *name, Panel *SetOwner, int widht, int height);

   bool RegisterWithPanel(LWPanelID pan);

protected:
   int Width;
   int Height;
};
