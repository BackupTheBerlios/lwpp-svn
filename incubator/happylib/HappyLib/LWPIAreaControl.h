
#ifndef _LWPIAREACONTROL_H
#define _LWPIAREACONTROL_H

#include "LWPPlugin.h"
#include "LWPPanel.h"
#include "HLPoint.h"


//// double-buffered drag area control ////

// [FIXME] move everything into the class body or make inline (can't inline the statics)

class LWPIAreaControl
{
public:
	LWPControl * ctrl;
	int dragging;
	LWPIAreaControl() ;
	virtual ~LWPIAreaControl();
	virtual void draw(LWRasterID raster, int, int) const;
	virtual void event(LWPanel, int, int, int, int, int[3], int);
	virtual int mouseDown(int, int, int);
	virtual int mouseDrag(int[3], int);

	XCALL_(static void) Draw(LWPControl *, void *, DrMode);
	XCALL_(static void) Event(LWPControl *, void *);
};

LWPIAreaControl::LWPIAreaControl() : ctrl(0), dragging(0)
{
}

LWPIAreaControl::~LWPIAreaControl()
{
}

void LWPIAreaControl::draw(LWRasterID raster, int w, int h) const
{
	// borders [FIXME]
	LWPPlugin::rasterFuncs->drawRGBBox(raster, 96, 96, 96, 0, 0, w, 2);
	LWPPlugin::rasterFuncs->drawRGBBox(raster, 96, 96, 96, 0, h-2, w, 2);

	/*
	Color col;
	Colori c(col);
	unsigned char * ch = (unsigned char *)&c;
	LWPPlugin::rasterFuncs->drawRGBBox(raster, ch[0], ch[1], ch[2], t, 2, 1, h-4);
	*/

	// draw sliders
	//for (k = head; k; k = k->next) k->draw(panel, raster, 0, 0, w, h, k == sel, k == sel && dragging);
}

void LWPIAreaControl::event(LWPanel panel, int w, int h, int mx, int my, int md[3], int kq)
{
	int redraw = 0;
	if (!md[2])	// hasn't dragged yet
	{
		dragging = 0;
		redraw = mouseDown(mx, my, kq);	// [NOTE] mouseDown may set dragging = 1
	}
	else		// is dragging
	{
		redraw = mouseDrag(md, kq);
	}
	if (redraw) Draw(ctrl, this, DR_REFRESH);	// [FIXME]
}

int LWPIAreaControl::mouseDown(int mx, int my, int kq)
{
	// [FIXME]
	return 1;
}

int LWPIAreaControl::mouseDrag(int md[3], int kq)
{
	// [FIXME]
	return 1;
}

XCALL_(void) LWPIAreaControl::Draw(LWPControl * ctrl, void * data, DrMode mode)
{
	LWPIAreaControl * area = (LWPIAreaControl *)data;
	LWPanel panel = ctrl->getPanel();

	// control dimensions
	int x = ctrl->getHotX();
	int y = ctrl->getHotY();
	int w = ctrl->getHotW();
	int h = ctrl->getHotH();

	LWRasterID raster = LWPPlugin::rasterFuncs->create(w, h, 0);
	area->draw(raster, w, h);
	LWPPlugin::rasterFuncs->blitPanel(raster, 0, 0, panel, x, y, w, h);
	LWPPlugin::rasterFuncs->destroy(raster);
}

XCALL_(void) LWPIAreaControl::Event(LWPControl * ctrl, void * data)
{
	LWPIAreaControl * area = (LWPIAreaControl *)data;
	LWPanel panel = ctrl->getPanel();

	// control dimensions
	int x = ctrl->getHotX();
	int y = ctrl->getHotY();
	int w = ctrl->getHotW();
	int h = ctrl->getHotH();

	// mouse click coordinates
	int mx = ctrl->getMouseX();
	int my = ctrl->getMouseY();

	// mouse drag coordinates
	int md[3];
	ctrl->getIvec(md);

	// keyboard qualifiers
	int kq = LWPPanel(panel).getInt(PAN_QUALIFIERS);

	area->event(panel, w, h, mx-x, my-y, md, kq);
}

#endif
