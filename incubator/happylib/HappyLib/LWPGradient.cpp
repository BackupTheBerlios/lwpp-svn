
#include "LWPGradient.h"
#include "LWPPanel.h"

// [DOCUMENTME]


//// LWPGradientKey implementation ////

LWError LWPGradientKey::load(LWPLoadState const * ls)
{
	frac = ls->readFP();
	col = ls->readColor();
	val = ls->readFP();
	return 0;
}

LWError LWPGradientKey::save(LWPSaveState const * ss) const
{
	ss->writeFP(frac);
	ss->writeColor(col);
	ss->writeFP(val);
	return 0;
}

void LWPGradientKey::setXOffset(int w, int off)
{
	if (!prev || !next) return;		// don't alter ends of gradient
	frac = Max(prev->frac, Min(next->frac, double(off) / double(w - 1)));
	frac = int(frac * 100.0 + 0.5) * 0.01;	// round to nearest hundredth
}

void LWPGradientKey::draw(LWPPanel panel, LWRasterID raster, int x, int y, int w, int h, int numerics, int diamond) const
{
	Colori c(col);
	unsigned char * ch = (unsigned char *)&c;
	int offx = xOffset(w);
	int sx = x + offx;
	int showUpColor = col.luminance() > 0.25 ? COLOR_BLACK : COLOR_WHITE;
	// diamond at top
	if (diamond)
	{
		LWPPlugin::rasterFuncs->drawRGBBox(raster, 192, 192, 192, sx, y, 1, 5);
		LWPPlugin::rasterFuncs->drawRGBBox(raster, 192, 192, 192, sx-2, y+2, 5, 1);
		LWPPlugin::rasterFuncs->drawRGBBox(raster, 255, 255, 255, sx-1, y+1, 3, 3);
	}
	else
	{
		LWPPlugin::rasterFuncs->drawRGBBox(raster, 255, 255, 255, sx, y, 1, 5);
		LWPPlugin::rasterFuncs->drawRGBBox(raster, 255, 255, 255, sx-2, y+2, 5, 1);
		LWPPlugin::rasterFuncs->drawRGBBox(raster, 192, 192, 192, sx-1, y+1, 3, 3);
	}
	// color blob at bottom
	LWPPlugin::rasterFuncs->drawBox(raster, showUpColor, sx-2, y+h-5, 5, 5);
	LWPPlugin::rasterFuncs->drawRGBBox(raster, ch[0], ch[1], ch[2], sx-1, y+h-4, 3, 3);
	// numerics
	if (numerics)
	{
		char num[42];
		sprintf(num, "%i", int(frac * 100 + 0.5));
		int th = LWPPlugin::panelFuncs->drawFuncs->dispMetrics()->textHeight;
		int tw = LWPPlugin::panelFuncs->drawFuncs->textWidth(panel, num);
		int tx = Min(x+w-tw, Max(x, sx-tw/2));
		LWPPlugin::rasterFuncs->drawText(raster, num, showUpColor, tx, y+(h-th)/2);
	}
}


//// LWPGradient implementation ////

LWPGradient::LWPGradient() : head(0), tail(0), sel(0), type(GRADTYPE_COLOR), dragging(0)
{
	append(new LWPGradientKey);
	append(new LWPGradientKey);
	head->frac = 0.0;
	tail->frac = 1.0;
	head->col = Color::Black();
	tail->col = Color::White();
}

LWPGradient::LWPGradient(LWPGradient const & g) : head(0), tail(0), sel(0), type(g.type), dragging(0)
{
	for (LWPGradientKey * k = g.head; k; k = k->next) append(new LWPGradientKey(k));
}

LWPGradient::~LWPGradient()
{
	for (LWPGradientKey * k = head; k; k = head)
	{
		remove(k);
		delete k;
	}
}

LWPGradient & LWPGradient::operator = (LWPGradient const & g)
{
	if (&g == this) return *this;
	LWPGradient::~LWPGradient();
	head = tail = sel = 0;
	dragging = 0;
	type = g.type;
	for (LWPGradientKey * k = g.head; k; k = k->next) append(new LWPGradientKey(k));
	return *this;
}

void LWPGradient::remove(LWPGradientKey * k)
{
	if (k == head) head = k->next;
	if (k == tail) tail = k->prev;
	if (k->prev) k->prev->next = k->next;
	if (k->next) k->next->prev = k->prev;
	k->next = k->prev = 0;
	if (sel == k) sel = 0;
	dragging = 0;
}

void LWPGradient::append(LWPGradientKey * k)
{
	if (!head) head = tail = k;
	else
	{
		k->prev = tail;
		tail->next = k;
		tail = k;
	}
	dragging = 0;
}

void LWPGradient::insertAfter(LWPGradientKey * before, LWPGradientKey * k)
{
	k->next = before->next;
	k->next->prev = k;
	k->prev = before;
	k->prev->next = k;
	dragging = 0;
}

void LWPGradient::addNewKey(double frac)
{
	LWPGradientKey * k = new LWPGradientKey;
	k->frac = frac;
	k->col = evaluateColor(k->frac);
	LWPGradientKey * before;
	for (before = head; before->next->frac <= frac; before = before->next);
	insertAfter(before, k);
	sel = k;
}

void LWPGradient::select(int x, int y, int w, int h, int selx)
{
	int offx = selx - x;
	int closest = w;
	sel = 0;
	for (LWPGradientKey * k = head; k; k = k->next)
	{
		int dist = abs(k->xOffset(w) - offx);
		if ((dist < closest) || (dist == closest && offx > k->xOffset(w)))
		{
			closest = abs(k->xOffset(w) - offx);
			sel = k;
		}
	}
	if (closest > 5) sel = 0;
}

int LWPGradient::keyCount() const
{
	int c = 0;
	for (LWPGradientKey * k = head; k; k = k->next) c ++;
	return c;
}

Color LWPGradient::evaluateColor(double frac) const
{
	if (frac <= 0.0) return head->col;
	if (frac >= 1.0) return tail->col;
	for (LWPGradientKey * k = head; k->next; k = k->next)
	{
		if (k->frac <= frac && k->next->frac >= frac)
		{
			if (k->frac == k->next->frac) return k->col;
			double alpha = (frac - k->frac) / (k->next->frac - k->frac);
			Color col = pow(Lerp(pow(k->col, 2.2), pow(k->next->col, 2.2), alpha), 1.0/2.2);
			col.clamp(0.0, 1.0);	// bloody floating point precision
			return col;
		}
	}
	HLASSERT(0, "Bad gradient evaluation");
	return head->col;
}

LWError LWPGradient::load(LWPLoadState const * ls)
{
	LWPGradient::~LWPGradient();
	head = tail = sel = 0;
	dragging = 0;
	type = (GradType)ls->readI4();
	LWPGradientKey k;
	for (int c = ls->readI4(); c; c --)
	{
		k.load(ls);
		append(new LWPGradientKey(&k));
	}
	return 0;
}

LWError LWPGradient::save(LWPSaveState const * ss) const
{
	ss->writeI4(type);
	ss->writeI4(keyCount());
	for (LWPGradientKey * k = head; k; k = k->next) k->save(ss);
	return 0;
}

void LWPGradient::draw(LWPPanel panel, int x, int y, int w, int h) const
{
	LWRasterID raster = LWPPlugin::rasterFuncs->create(w, h, 0);

	// borders
	LWPPlugin::rasterFuncs->drawRGBBox(raster, 96, 96, 96, 0, 0, w, 2);
	LWPPlugin::rasterFuncs->drawRGBBox(raster, 96, 96, 96, 0, h-2, w, 2);

	// render gradient
	LWPGradientKey * k = head;
	Color col;
	for (int t = 0; t < w; t ++)
	{
		while (k->next && (k->next->xOffset(w) <= t || k->next->xOffset(w) == k->xOffset(w))) k = k->next;
		if (k->next)
		{
			double alpha = double(t - k->xOffset(w)) / double(k->next->xOffset(w) - k->xOffset(w));
			col = pow(Lerp(pow(k->col, 2.2), pow(k->next->col, 2.2), alpha), 1.0/2.2);
			col.clamp(0.0, 1.0);	// bloody floating point precision
		}
		else col = k->col;
		Colori c(col);
		unsigned char * ch = (unsigned char *)&c;
		LWPPlugin::rasterFuncs->drawRGBBox(raster, ch[0], ch[1], ch[2], t, 2, 1, h-4);
	}

	// draw sliders
	for (k = head; k; k = k->next) k->draw(panel, raster, 0, 0, w, h, k == sel, k == sel && dragging);
	
	// blit panel
	LWPPlugin::rasterFuncs->blitPanel(raster, 0, 0, panel, x, y, w, h);

	LWPPlugin::rasterFuncs->destroy(raster);
}

void HandleColor(Color & col)
{
	LWColorPickLocal local;
	local.result  = 0;
	local.title   = "Color";
	local.red	  = (float)col[0];
	local.green   = (float)col[1];
	local.blue	  = (float)col[2];
	local.data	  = 0;
	local.hotFunc = 0;

	int result = LWPPlugin::colorPickerFunc(LWCOLORPICK_VERSION, &local);
	if (result == AFUNC_OK && local.result > 0)
	{
		col[0] = local.red;
		col[1] = local.green;
		col[2] = local.blue;
	}
}

void LWPGradient::event(LWPPanel panel, int x, int y, int w, int h, int mx, int my, int md[3], int kq)
{
	int redraw = 0;
	if (!md[2])	// hasn't dragged yet, so select
	{
		LWPGradientKey * prevSel = sel;
		select(x, y, w, h, mx);
		redraw = 1;
		dragging = 0;
		if (sel)	// a key was selected
		{
			if (md[1] < h*0.25)			// selected drag gadget
			{
				if (kq && sel != head && sel != tail)	// qualifiers held down, so delete the key
				{
					LWPGradientKey * k = sel;
					remove(k);
					delete k;
				}
				else dragging = 1;
			}
			else if (md[1] > h*0.70)	// selected color gadget
			{
				if (kq && prevSel)	// qualifiers held down, so paste key
				{
					sel->col = prevSel->col;
				}
				else HandleColor(sel->col);
			}
			else	// just clicked in the middle of the key
			{
				if (sel == prevSel) sel = 0;	// unselect
			}
		}
		else		// no key selected, so create a new key
		{
			addNewKey(double(md[0]) / double(w - 1));
			if (kq && prevSel) sel->col = prevSel->col;	// copy key if qualifier held down
		}
	}
	else	// is dragging
	{
		if (sel && dragging)	// we're dragging a selected key
		{
			sel->setXOffset(w, md[0]); redraw = 1;
		}
	}
	if (redraw) draw(panel, x, y, w, h);
}

XCALL_(void) LWPGradient::Draw(LWPControl * ctrl, void * data, DrMode mode)
{
	LWPGradient * grad = (LWPGradient *)data;
	grad->draw(ctrl->getPanel(), ctrl->getHotX(), ctrl->getHotY(), ctrl->getHotW(), ctrl->getHotH());
}

XCALL_(void) LWPGradient::Event(LWPControl * ctrl, void * data)
{
	// mouse drag coordinates [FIXME] wrap somehow??
	int md[3];
	ctrl->getIvec(md);

	LWPGradient * grad = (LWPGradient *)data;
	grad->event(ctrl->getPanel(), ctrl->getHotX(), ctrl->getHotY(), ctrl->getHotW(), ctrl->getHotH(),
		ctrl->getMouseX(), ctrl->getMouseY(), md, ctrl->getPanel().getInt(PAN_QUALIFIERS));
}

