#pragma once
#include "control.h"

//##ModelId=3E16FCD6039E
class ControlCanvas : 
public Control  
{
public:
   ControlCanvas(void);
   virtual ~ControlCanvas(void);
   virtual void Initialize(const char *name, Panel *SetOwner, int widht, int height);
   bool RegisterWithPanel(LWPanelID pan);
   static void DrawCallback( LWControl *control, void *UserData, int drawmode );
   virtual void Draw();
   virtual void DrawPoint(int x, int y, unsigned char r, unsigned char g, unsigned char b);
   int Width;
   int Height;
};
