// ControlTab.cpp: implementation of the ControlTab class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ControlTab.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ControlTab::ControlTab() :
NumTabs(0)
{

}

ControlTab::~ControlTab()
{

}

bool ControlTab::RegisterWithPanel(LWPanelID pan)
{
   LWPanControlDesc locdesc;
   locdesc.type=LWT_CHOICE;
   locdesc.choice.vertical=MX_HCHOICE;
   locdesc.choice.items=const_cast<const char **>(Choices);

   NumTabs = 0;
   while (Choices[NumTabs])
   {
        vector<Control*> myvec;

        mTabControlVectors.push_back(myvec);
        NumTabs++;
   }

   ThisControl=Lightwave3D::panf->addControl (pan, "TabChoiceControl", &locdesc, const_cast<char *>(Name.c_str()));
   return true;
}

void ControlTab::addControlToTab(int tabnr, Control *control)
{
    if(tabnr == 0)
    {
        control->UnHide();
    }
    else
    {
        control->Hide();
    }
     
    mTabControlVectors[tabnr].push_back(control);
}

void ControlTab::notifyObservers()
{
    int index = GetValue();
    int curindex = 0;
    for ( vector< vector<Control*> >::iterator Itor = mTabControlVectors.begin();
          Itor != mTabControlVectors.end();
          Itor ++)
    {
        for (vector<Control*>::iterator innerItor= (*Itor).begin();
             innerItor != (*Itor).end();
             innerItor ++)
         {
             if(curindex == index)
             {
                 (*innerItor)->UnHide();                   
             }
             else
             {
                 (*innerItor)->Hide();                 
             }

         }
         curindex++;
    }

    ControlChoices::notifyObservers();
}