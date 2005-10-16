// Master.h: interface for the Master class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MASTER_INCLUDED
#define MASTER_INCLUDED

#include "HandlerImpl.h"
#include "Master_Adaptor.h"

class Master : 
public HandlerImpl  
{
public:
   Master() {}
   virtual ~Master() {}

   virtual double Event ( const LWMasterAccess *acc) { return 0.0; }

   virtual unsigned int Flags ( void ) { return 0; }
};

#endif // MASTER_INCLUDED
