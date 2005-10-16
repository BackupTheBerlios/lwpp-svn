#ifndef INTERFACE_H_HEADER_INCLUDED_C0CACDA7
#define INTERFACE_H_HEADER_INCLUDED_C0CACDA7
#include "../Panel/Panel.h"

class Interface : public Panel
{
public:
	Interface( const char *name="Default", int INFlags=0 );

    virtual void SetInstance(LWInstance inst)=0;
};

#endif /* INTERFACE_H_HEADER_INCLUDED_C0CACDA7 */
