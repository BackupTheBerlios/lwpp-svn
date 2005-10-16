// ControlTab.h: interface for the ControlTab class.
//
//////////////////////////////////////////////////////////////////////

#ifndef CONTROLTAB_H
#define CONTROLTAB_H

#include <vector>

#include "ControlChoices.h"

class ControlTab : public ControlChoices  
{
public:
	ControlTab();
	virtual ~ControlTab();
    virtual bool RegisterWithPanel(LWPanelID pan);

    void addControlToTab(int tabnr, Control *control);

    /// overloaded to provide automatic hiding/unhiding
    virtual void notifyObservers(); 
private:
    int NumTabs;
    vector< vector<Control*> > mTabControlVectors;
};

#endif
