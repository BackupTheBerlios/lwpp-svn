
#include <LWPShader.h>
#include <LWPTextureUtils.h>
#include "HLTrig.h"

class Blotch : public LWPShader
{
private:
	// members for precomputes, etc.
	double			r2, piOverR;
	LWPChanGroup	cGroup;
public:
	// user-modifiable members
	Color			color;
	Point			center;
	double			radius;
	double			softness;
	LWPVParm		vColor;
	LWPVParm		vCenter;
	LWPVParm		vRadius;
	LWPVParm		vSoftness;
public:
	// shader stuff
	ShaderStuff(Blotch, "Blotch Demo");
	Blotch(LWSurfaceID);
	// statics
	XCALL_(static int) Interface(long, GlobalFunc *, void *, void *);
	// virtuals
	char const * descLn() const;
	unsigned int flags() const;
	LWError newTime(LWFrame, LWTime);
	void evaluate(LWPShaderAccess *);
};

RegisterPlugin<Blotch> blotch(Blotch::Interface);


//// Blotch implementation ////

Blotch::Blotch(LWSurfaceID surface) : LWPShader(surface),
	// channel groups
	cGroup(surfaceFuncs->chanGrp(surface), name()),
	// members
	color(0.4, 0.0, 0.8),
	center(0.0, 0.0, 0.0),
	radius(1.0),
	softness(0.5),
	// vParms
	vColor(LWVP_COLOR, cGroup, "Color", color),
	vCenter(LWVP_FLOAT | LWVPF_VECTOR, cGroup, "Center", center),
	vRadius(LWVP_FLOAT, cGroup, "Radius", radius),
	vSoftness(LWVP_FLOAT, cGroup, "Softness", softness)
{
	// serialization (handles copy, load, and save for you)
	serialization.add("SCLR", color);
	serialization.add("SCNT", center);
	serialization.add("SRAD", radius);
	serialization.add("SSFT", softness);
	serialization.add("VCLR", vColor);
	serialization.add("VCNT", vCenter);
	serialization.add("VRAD", vRadius);
	serialization.add("VSFT", vSoftness);
}

char const * Blotch::descLn() const
{
	sprintf(desc, "Blotch  (%0.2f %.2f %.02f)", color[0], color[1], color[2]);	
	return desc;
}

unsigned int Blotch::flags() const
{
	return LWSHF_COLOR;
}

LWError Blotch::newTime(LWFrame f, LWTime t)
{
	color = vColor.evaluateColor(t);
	center = vCenter.evaluatePoint(t);
	radius = vRadius.evaluateFloat(t);
	softness = vSoftness.evaluateFloat(t);
	r2 = radius * radius;
	piOverR = Pi / radius;
	return 0;
}

void Blotch::evaluate(LWPShaderAccess * sa)
{
	double rad2 = sa->oPos || center;	// compute squared distance from center
	if (rad2 > r2) return;
	sa->color = Lerp(sa->color, color, pow(0.5 * (1.0 + cos(sqrt(rad2) * piOverR)), softness));
}


//// interface implementation ////
// [FIXME] At some point I'm going to wrap the xPanel stuff in some classes,
//         though I haven't decided on the best design for that yet.

enum { ID_COLOR = 0x8001, ID_CENTER, ID_RADIUS, ID_SOFTNESS, ID_SHOPEN, ID_SHADD };

void handle_color(LWXPanelID panel, int cid)
{
	LWColorPickLocal local;
	Blotch * inst;
	int result;

	inst = (Blotch*)LWPPlugin::xPanelFuncs->getData(panel, 0);

	local.result  = 0;
	local.title   = "Blotch Color";
	local.red	  = (float)inst->color[0];
	local.green   = (float)inst->color[1];
	local.blue	  = (float)inst->color[2];
	local.data	  = 0;
	local.hotFunc = 0;

	result = LWPPlugin::colorPickerFunc(LWCOLORPICK_VERSION, &local);
	if (result == AFUNC_OK && local.result > 0)
	{
		inst->color[0] = local.red;
		inst->color[1] = local.green;
		inst->color[2] = local.blue;
		inst->vColor.setVal(inst->color);
		LWPPlugin::instUpdate(LWSHADER_HCLASS, inst);
	}
}

void * ui_get(Blotch *inst, unsigned long vid)
{
	switch (vid)
	{
	case ID_COLOR:		return inst->vColor;
	case ID_CENTER:		return inst->vCenter;
	case ID_RADIUS:		return inst->vRadius;
	case ID_SOFTNESS:	return inst->vSoftness;
	default: ;
	}
	return 0;
}

int ui_set(Blotch * inst, unsigned long vid, void *)
{
	switch (vid)
	{
	case ID_COLOR:
	case ID_CENTER:
	case ID_RADIUS:
	case ID_SOFTNESS:
		return 1;
	default: ;
	}
	return 0;
}

void ui_chgnotify(LWXPanelID panel, unsigned long cid, unsigned long vid, int event)
{
	void * inst = LWPPlugin::xPanelFuncs->getData(panel, 0);
	if (event == LWXPEVENT_VALUE) LWPPlugin::instUpdate(LWSHADER_HCLASS, inst);
}

LWXPanelID get_panel(Blotch * inst)
{
	static LWXPanelControl xctl[] =
	{
		{ ID_COLOR,    "Color",       "color-env",    },
		{ ID_CENTER,   "Center",      "distance3-env" },
		{ ID_RADIUS,   "Radius",      "distance-env"  },
		{ ID_SOFTNESS, "Softness",    "float-env"     },
		{ 0 }
	};

	static LWXPanelDataDesc xdata[] =
	{
		{ ID_COLOR,    "Color",    "color-env",    },
		{ ID_CENTER,   "Center",   "distance3-env" },
		{ ID_RADIUS,   "Radius",   "distance-env"  },
		{ ID_SOFTNESS, "Softness", "float-env"     },
		{ 0 }
	};

	static LWXPanelHint xhint[] =
	{
		XpLABEL(0, "Blotch"),
			XpBUTNOTIFY(ID_COLOR, handle_color),
			XpCHGNOTIFY(ui_chgnotify),
			XpRANGE(ID_SOFTNESS, 0, 1, 1),
			XpEND
	};

	LWXPanelID panel;

	if (panel = LWPPlugin::xPanelFuncs->create(LWXP_VIEW, xctl))
	{
		LWPPlugin::xPanelFuncs->hint(panel, 0, xhint);
		LWPPlugin::xPanelFuncs->describe(panel, xdata, (LWXPanelGetFunc*)ui_get, (LWXPanelSetFunc*)ui_set);
		LWPPlugin::xPanelFuncs->viewInst(panel, inst);
		LWPPlugin::xPanelFuncs->setData(panel, 0, inst);
	}
	
	return panel;
}

XCALL_(int) Blotch::Interface(long version, GlobalFunc * global, void * vlocal, void * serverData)
{
	if (version != LWINTERFACE_VERSION) return AFUNC_BADVERSION;
	
	LWInterface * local = (LWInterface *)vlocal;
	
	local->panel   = get_panel((Blotch*)local->inst);
	local->options = 0;
	local->command = 0;
	
	return local->panel ? AFUNC_OK : AFUNC_BADGLOBAL;
}

/* [FIXME] handle tags (goes on both the hnadler and the interface)
ServerTagInfo ServerTags[] =
{
	{ "Demo: Blotch with Envelopes", SRVTAG_USERNAME },
	{ 0 }
};
*/
