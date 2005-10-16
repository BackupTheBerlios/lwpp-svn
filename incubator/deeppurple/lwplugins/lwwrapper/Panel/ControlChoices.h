// ControlChoices.h: interface for the ControlChoices class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CONTROLCHOICES_INCLUDED
#define CONTROLCHOICES_INCLUDED

#include "Control.h"

class ControlChoices : public Control  
{
public:
	ControlChoices();
	virtual ~ControlChoices();
   virtual void Initialize(const char *name, Panel *SetOwner, char **ChoiceStrings);
   int GetValue(void);
   void SetValue(int Val);
protected:
   char **Choices;
};

#endif // CONTROLCHOICES_INCLUDED
