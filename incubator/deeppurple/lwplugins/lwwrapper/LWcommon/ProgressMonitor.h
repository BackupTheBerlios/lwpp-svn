// ProgressMonitor.h: interface for the ProgressMonitor class.
//
//////////////////////////////////////////////////////////////////////

#ifndef PROGRESSMONITOR_INCLUDED
#define PROGRESSMONITOR_INCLUDED

class ProgressMonitor  
{
public:
	ProgressMonitor(char *title, unsigned int flags);
	virtual ~ProgressMonitor();
    void    Init(int numSteps,const char *initialMessage=NULL);
    int     IncMon(int num=1,const char *message=NULL);
    void    Done(void);
    LWLMonID theMon;
};

#endif // PROGRESSMONITOR_INCLUDED
