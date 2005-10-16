// ControlText.h: interface for the ControlText class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CONTROLTEXT_INCLUDED
#define CONTROLTEXT_INCLUDED

#include "Control.h"

class ControlText : public Control  
{
public:
	ControlText();
	virtual ~ControlText();
   virtual void Initialize(const char *name, Panel *SetOwner, const char **StringList);
   virtual bool RegisterWithPanel(LWPanelID pan);
protected:
   const char **StringRow;
};

#endif // CONTROLTEXT_INCLUDED
