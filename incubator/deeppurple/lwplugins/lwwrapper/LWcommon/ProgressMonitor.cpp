// ProgressMonitor.cpp: implementation of the ProgressMonitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProgressMonitor.h"
#include "../../lwinclude/lwmonitor.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ProgressMonitor::ProgressMonitor(char *title, unsigned int flags) :
theMon(NULL)
{
   theMon=Lightwave3D::lmonf->create();
   if (theMon)
   {
      Lightwave3D::lmonf->setup(theMon, title, flags, NULL);
   }
}

ProgressMonitor::~ProgressMonitor()
{
   if(theMon)
      Lightwave3D::lmonf->destroy(theMon);
}

void ProgressMonitor::Init(int numSteps,const char *initialMessage)
{
   if(theMon)
      Lightwave3D::lmonf->init(theMon,numSteps,initialMessage);
}

int ProgressMonitor::IncMon(int num,const char *message)
{
   if(theMon)
      return Lightwave3D::lmonf->step(theMon, num, message);
   else
      return 0;
}

void ProgressMonitor::Done(void)
{
   if(theMon)
      Lightwave3D::lmonf->done(theMon);
}