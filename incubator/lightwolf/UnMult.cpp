/*! @file
 *
 * @brief
 * UNMULT.C -- LightWave Plugins ImageFilterClass
 * UnMult source code for ImageFilterClass.
 * 
 * Ported from LScript Arnie Cachelin's LScript
 *
 * @author Michael Wolf
 * @todo fill in remaining methods
 */

#include "UnMult.h"

/*!
 * Initialize variables.
 */
UnMultiply::UnMultiply(GlobalFunc *g, void *context, LWError *err) : 
lwpp::ImageFilterXPanelHandler(g, context, err),
magicnumber(0x01234567)
{
	threshold	 = 0.0;
	useThresh	 = false;
	useBGColour	 = true;
	useRGBColour = false;
	flare2Alpha	 = false;
	VSET(bgRGB, 0.0);
	dostream d;
	d << "Construct :" << this << "\n";
}

/*!
 * Process
 * Just negative right now...
 */
void UnMultiply::Process(const LWFilterAccess *fa)
{
   LWFVector out;
   float *r, *g, *b, *a;
   int x, y;

   /* fire up the monitor */

   MON_INIT( fa->monitor, fa->height / 8 );

   for ( y = 0; y < fa->height; y++ ) {

      /* get each scanline */

      r = fa->getLine( LWBUF_RED, y );
      g = fa->getLine( LWBUF_GREEN, y );
      b = fa->getLine( LWBUF_BLUE, y );
      a = fa->getLine( LWBUF_ALPHA, y );

      for ( x = 0; x < fa->width; x++ ) {

         /* change each pixel value c to 1.0 - c (leave the alpha alone) */

         out[ 0 ] = 1.0f - r[ x ];
         out[ 1 ] = 1.0f - g[ x ];
         out[ 2 ] = 1.0f - b[ x ];

         /* set the new value */

         fa->setRGB( x, y, out );
         fa->setAlpha( x, y, a[ x ] );
      }

      /* once every 8 lines, step the monitor and check for abort */

      if (( y & 7 ) == 7 )
         if ( MON_STEP( fa->monitor )) return;
   }

   MON_DONE( fa->monitor );

}

/*
 *	UnMultPanel
 */
enum controlIDs {CH_CALP = 0x8001, CH_THRS, CH_CRGB, CH_UNMU, CH_UBGC, CH_FL2A};

LWXPanelID UnMultiply::XPanel()
{
  static LWXPanelControl con[] =
  {
	{CH_UNMU, "Unmultiply", "iBoolean"},
	{CH_UBGC, "Background Colour", "color"},
	{CH_FL2A, "Flare 2 Alpha", "iBoolean"},
	{CH_CALP, "Clip Alpha at Threshold", "iBoolean"},
	{CH_CRGB, "Clip RGB at Threshold", "iBoolean"},
	{CH_THRS, "Threshold", "float"},
	0
  };

  static LWXPanelDataDesc  def[] =
  {
	{CH_UNMU, "Unmultiply", "integer"},
	{CH_UBGC, "Background Colour", "color"},
	{CH_FL2A, "Flare 2 Alpha", "iBoolean"},
	{CH_CALP, "Clip Alpha at Threshold", "integer"},
	{CH_CRGB, "Clip RGB at Threshold", "integer"},
	{CH_THRS, "Threshold", "float"},
	0
  };

  // Hints
  static LWXPanelHint hint[] = 
  {
	XpLABEL( 0, "Alphachannel tools " __DATE__),
	XpENABLE_(CH_CALP),
	  XpH(CH_THRS),
	  XpH(CH_CRGB),
	XpEND,
	XpENABLE_(CH_UNMU),
	  XpH(CH_UBGC),
	XpEND,
	XpDIVADD(CH_UBGC),
	XpDIVADD(CH_FL2A),
	XpEND
  };

  LWXPanelFuncs	  *pan_func = NULL;
  LWXPanelID	  pan = NULL;

  pan_func = ( LWXPanelFuncs * )(*global) (LWXPANELFUNCS_GLOBAL, GFUSE_TRANSIENT);

  pan = (*pan_func->create) (LWXP_VIEW, con);
  if (!pan) return NULL;
  (*pan_func->hint)		(pan, 0, hint );
  //(*pan_func->describe) (pan, def, lwpp::WrapXPanelImageFilter<UnMultiply>::XGet, lwpp::WrapXPanelImageFilter<UnMultiply>::XSet);
  (*pan_func->viewInst) (pan, this);
  (*pan_func->setData)	(pan, 0, this);
  return pan;
}


void *UnMultiply::XGet(unsigned long vid )
{
  void *result = NULL;
  static double c[3];

  switch ( vid ) {
	 case CH_CALP:
		result = &useThresh;
		break;

	 case CH_THRS:
		result = &threshold;
		break;

	 case CH_CRGB:
		result = &clipRGB;
		break;

	 case CH_UNMU:
		result = &useBGColour;
		break;

	 case CH_UBGC:
		c[0] = bgRGB[0];
		c[1] = bgRGB[1];
		c[2] = bgRGB[2];
		result = c;
		break;

	  case CH_FL2A:
		result = &flare2Alpha;
		break;

	 default:
		break;
  }
  
  return result;
}


LWXPRefreshCode
UnMultiply::XSet(unsigned long vid, void *value )
{
  double *d = (double *)value;
  char *c = (char *)value;
  int *i = (int *)value;
  bool *b = (bool *)value;

  switch ( vid ) {
	case CH_CALP:
	  useThresh = *b;
	  break;

	case CH_THRS:
	  threshold = (float)(*d);
	  break;

	case CH_CRGB:
	  clipRGB = *b;
	  break;

	case CH_UNMU:
	  useBGColour = *b;
	  break;

  	case CH_UBGC:
	  bgRGB[0] = (float)d[0];
	  bgRGB[1] = (float)d[1];
	  bgRGB[2] = (float)d[2];
	  break;

	case CH_FL2A:
	  flare2Alpha = *b;
	  break;

	 default:
	  break;
  } 
  return LWXPRC_DFLT;
}