#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lwpp_imagefilter.h"
#include <lwmath.h>


//! The actual class implementing the filter
class UnMultiply 
: public lwpp::ImageFilterXPanelHandler
{
	LWFVector bgRGB; // User defined background colour
	
	bool	  useRGBColour;
	bool	  useBGColour;
	bool	  clipRGB;
	bool	  useThresh;
	bool	  flare2Alpha;
	float	  threshold;

	public:
	  UnMultiply(GlobalFunc *g, void *context, LWError *err);
	  ~UnMultiply() {;}
	  void Process(const LWFilterAccess *fa);
	  LWXPanelID XPanel();
	  void *  XGet(unsigned long vid );
	  LWXPRefreshCode XSet(unsigned long vid, void *value );
};

