
#ifndef _LWPPANEL_H
#define _LWPPANEL_H


//// wrapper for LWValue [DOCUMENTME] ////

class LWPValue
{
protected:
	mutable LWValue lwvalue;
public:
	operator LWValue * () const { return &lwvalue; }
};

class LWPValString : public LWPValue
{
public:
	LWPValString(char * buf, int bufLen)
	{
		lwvalue.type = LWT_STRING;
		lwvalue.str.buf = buf;
		lwvalue.str.bufLen = bufLen;
	}
};

class LWPValPtr : public LWPValue
{
public:
	LWPValPtr(void * value)
	{
		lwvalue.type = LWT_INTEGER;
		lwvalue.intv.value = (int)value;
		lwvalue.intv.defVal = (int)value;
	}
};

class LWPValInt : public LWPValue
{
public:
	LWPValInt()		// [FIXME] make default constructor for the other types too
	{
		lwvalue.type = LWT_INTEGER;
	}
	LWPValInt(int value)
	{
		lwvalue.type = LWT_INTEGER;
		lwvalue.intv.value = value;
		lwvalue.intv.defVal = value;
	}
	LWPValInt(int value, int defVal)
	{
		lwvalue.type = LWT_INTEGER;
		lwvalue.intv.value = value;
		lwvalue.intv.defVal = defVal;
	}
	int value() { return lwvalue.intv.value; }	// [FIXME] make this for the other types too
};

class LWPValFloat : public LWPValue
{
public:
	LWPValFloat(double value)
	{
		lwvalue.type = LWT_FLOAT;
		lwvalue.flt.value = value;
		lwvalue.flt.defVal = value;
	}
	LWPValFloat(double value, double defVal)
	{
		lwvalue.type = LWT_FLOAT;
		lwvalue.flt.value = value;
		lwvalue.flt.defVal = defVal;
	}
};

class LWPValIVector : public LWPValue
{
public:
	LWPValIVector()
	{
		lwvalue.type = LWT_VINT;
	}
	LWPValIVector(int val[3])
	{
		lwvalue.type = LWT_VINT;
		memcpy(&lwvalue.ivec.val, &val, sizeof(int[3]));
		lwvalue.ivec.defVal = val[0];
	}
	LWPValIVector(int val[3], int defVal)
	{
		lwvalue.type = LWT_VINT;
		memcpy(&lwvalue.ivec.val, &val, sizeof(int[3]));
		lwvalue.ivec.defVal = defVal;
	}
	void value(int val[3]) { memcpy(val, lwvalue.ivec.val, 3*sizeof(int)); }
};

class LWPValFVector : public LWPValue
{
public:
	LWPValFVector(double val[3])
	{
		lwvalue.type = LWT_VFLOAT;
		memcpy(&lwvalue.fvec.val, &val, sizeof(int[3]));
		lwvalue.fvec.defVal = val[0];
	}
	LWPValFVector(double val[3], double defVal)
	{
		lwvalue.type = LWT_VFLOAT;
		memcpy(&lwvalue.fvec.val, &val, sizeof(int[3]));
		lwvalue.fvec.defVal = defVal;
	}
};

class LWPValCustom : public LWPValue
{
public:
	LWPValCustom(int val[4])
	{
		lwvalue.type = LWT_CUSTOM;
		memcpy(&lwvalue.cust.val, &val, sizeof(int[4]));
	}
};


//// wrapper for LWPanControlDesc [DOCUMENTME] ////

class LWPControlDesc
{
protected:
	mutable LWPanControlDesc desc;
public:
	operator LWPanControlDesc * () const { return &desc; }
};

class LWPStringDesc : public LWPControlDesc
{
public:
	LWPStringDesc(int width)
	{
		desc.type = LWT_STRING;
		desc.string.width = width;
	}
};

class LWPChoiceDesc : public LWPControlDesc
{
public:
	LWPChoiceDesc(char const ** items, int vertical)
	{
		desc.type = LWT_CHOICE;
		desc.choice.items = items;
		desc.choice.vertical = vertical;
	}
};

class LWPTextDesc : public LWPControlDesc
{
public:
	LWPTextDesc(char const ** text)
	{
		desc.type = LWT_TEXT;
		desc.text.text = text;
	}
};

class LWPRangeDesc : public LWPControlDesc
{
public:
	LWPRangeDesc(int width, int min, int max)
	{
		desc.type = LWT_RANGE;
		desc.range.width = width;
		desc.range.min = min;
		desc.range.max = max;
	}
};

class LWPAreaDesc : public LWPControlDesc
{
public:
	LWPAreaDesc(int width, int height)
	{
		desc.type = LWT_AREA;
		desc.area.width = width;
		desc.area.height = height;
	}
};

class LWPLWItemDesc : public LWPControlDesc
{
public:
	LWPLWItemDesc(int itemType, char ** list, int count, int width)
	{
		desc.type = LWT_LWITEM;
		desc.lwitem.global = LWPPlugin::globalFunc;
		desc.lwitem.itemType = itemType;
		desc.lwitem.list = list;
		desc.lwitem.count = count;
		desc.lwitem.width = width;
	}
};

class LWPPopupDesc : public LWPControlDesc
{
public:
	LWPPopupDesc(int width,
		int (*countFn)(void *),
		char * (*nameFn)(void *, int))
	{
		desc.type = LWT_POPUP;
		desc.popup.width = width;
		desc.popup.countFn = countFn;
		desc.popup.nameFn = nameFn;
	}
};

class LWPListBoxDesc : public LWPControlDesc
{
public:
	LWPListBoxDesc(int width, int visItems, int top,
		int (*countFn)(void *),
		char * (*nameFn)(void *, int))
	{
		desc.type = LWT_POPUP;
		desc.listbox.width = width;			// width in pixels
		desc.listbox.visItems = visItems;	// height in items
		desc.listbox.top = top;				// top visible item
		desc.listbox.countFn = countFn;
		desc.listbox.nameFn = nameFn;
	}
};

class LWPXPanDesc : public LWPControlDesc
{
public:
	LWPXPanDesc(int width, int height, void * xpan)
	{
		desc.type = LWT_XPANEL;
		desc.xpanel.width = width;
		desc.xpanel.height = height;
		desc.xpanel.xpan = xpan;
	}
};

class LWPMultiListBoxDesc : public LWPControlDesc
{
public:
	LWPMultiListBoxDesc(int width, int visItems, int top,
		int (*countFn)(void *),
		char * (*nameFn)(void *, int, int col),
		int (*colWidth)(void *, int col))
	{
		desc.type = LWT_MLIST;
		desc.multiList.width = width;		// width in pixels
		desc.multiList.visItems = visItems;	// height in items
		desc.multiList.top = top;			// top visible item
		desc.multiList.countFn = countFn;
		desc.multiList.nameFn = nameFn;
		desc.multiList.colWidth = colWidth;	// pixel width of column col, up to 8 columns terminate with 0
	}
};

class LWPTreeDesc : public LWPControlDesc
{
public:
	LWPTreeDesc(int width, int height,		
		int (*countFn)(void *data, void *node),
		void *(*leafFn)(void *data, void *node, int i),
		char *(*infoFn)(void *data, void *node, int *nodeFlags),
		void (*moveFn)(void *data, void *node, void *parentNode, int i))
	{
		desc.type = LWT_TREE;
		desc.tree.width = width;
		desc.tree.height = height;
		desc.tree.countFn = countFn;	// return number of leafs under given node, NULL node is root
		desc.tree.leafFn = leafFn;		// return leaf i of node, NULL node is root
		desc.tree.infoFn = infoFn;		// return "name" for node, fill 0 flags, or save non-zero flags
		desc.tree.moveFn = moveFn;		// NULL prevents moves, or called when user moves node, with new parent, position
	}
};


//// wraper for LWPanel [DOCUMENTME] ////

class LWPPanel
{
protected:
	mutable LWPanelID panel;
public:
	operator LWPanelID () const { return panel; }
	LWPPanel() : panel(0) {}
	LWPPanel(LWPanelID panel) : panel(panel) {}
	~LWPPanel() {}
	// low level wraps
	int open(int flags) { return LWPPlugin::panelFuncs->open(panel, flags); }
		// see flag bits  PANF_ etc.
	int handle(int eventBits) { return LWPPlugin::panelFuncs->handle(panel, eventBits); }
		// process input manually after non-blocking open, use EVNT_ bits to process async. or synchronously
	void draw(DrMode md) { LWPPlugin::panelFuncs->draw(panel, md); }
	void close() { LWPPlugin::panelFuncs->close(panel); }
	void destroy() { LWPPlugin::panelFuncs->destroy(panel); panel = 0; }
	void get(pTag tag, void * value) const { LWPPlugin::panelFuncs->get(panel, tag, value); }
	void set(pTag tag, void * value) { LWPPlugin::panelFuncs->set(panel, tag, value); }
	LWControl * addControl(char *type, ControlDesc *data, char *label) { return LWPPlugin::panelFuncs->addControl(panel, type, data, label); }
	LWControl * nextControl(LWControlID ctl) { return LWPPlugin::panelFuncs->nextControl(panel, ctl); }
	DrawFuncs * drawFuncs() { return LWPPlugin::panelFuncs->drawFuncs; }
	// medium level wraps [FIXME] more functions needed like the following:
	int getInt(pTag tag) const { int i; get(tag, &i); return i; }
	void setData(void * data) { set(PAN_USERDATA, data); }
	void setClose(XCALL_(void)(*event)(LWPanelID, void *)) { set(PAN_USERCLOSE, event); }
	void setKeys(XCALL_(void)(*event)(LWPanelID, void *, LWDualKey)) { set(PAN_USERKEYS, event); }
	void setDraw(XCALL_(void)(*event)(LWPanelID, DrMode)) { set(PAN_USERDRAW, event); }
	void setX(int x) { set(PAN_X, &x); }
	void setY(int y) { set(PAN_Y, &y); }
	void setW(int w) { set(PAN_W, &w); }
	void setH(int h) { set(PAN_H, &h); }
	// high level wraps [FIXME] more functions needed like the following:
	int getResult() const { return getInt(PAN_RESULT); }
	int getX() const { return getInt(PAN_X); }
	int getY() const { return getInt(PAN_Y); }
	int getW() const { return getInt(PAN_W); }
	int getH() const { return getInt(PAN_H); }
	void setWH(int w, int h) { setW(w); setH(h); }
	void setXY(int x, int y) { setX(x); setY(y); }
	int good() const { return !!panel; }
	int init(char const * title, void * data)
	{
		if (panel) destroy();
		if (LWPPlugin::panelFuncs)
		{
			panel = LWPPlugin::panelFuncs->create((char*)title, data);
			LWPPlugin::panelFuncs->globalFun = LWPPlugin::globalFunc;
		}
		return good();
	}
	void center()
	{
		setXY((drawFuncs()->dispMetrics()->width - getW()) / 2, (drawFuncs()->dispMetrics()->height - getH()) / 2);
	}
	// [FIXME] need more opening functions like these:
	void openBlocking()
	{
		open(PANF_BLOCKING);
	}
};


//// wrapper for LWControl [DOCUMENTME] ////

class LWPControl
{
public:
	static LWPControl * create(LWPPanel panel, char const * type, LWPControlDesc & desc, char const * label)
	{
		return (LWPControl*)LWPPlugin::panelFuncs->addControl(panel, (char*)type, desc, (char*)label);
	}
    void draw(DrMode md) { ((LWControl*)this)->draw(((LWControl*)this), md); }
	// low level wraps
    void get(cTag tag, LWPValue & value) const { ((LWControl*)this)->get(((LWControl*)this), tag, value); }
    void set(cTag tag, LWPValue & value) { ((LWControl*)this)->set(((LWControl*)this), tag, value); }
	// medium level wraps [FIXME] more functions needed like the following:
	int getInt(cTag tag) const { LWPValInt i; get(tag, i); return i.value(); }
	int getInt() const { return getInt(CTL_VALUE); }
	void getIvec(int val[3]) const { LWPValIVector i; get(CTL_VALUE, i); i.value(val); }
	void setX(int x) { set(CTL_X, LWPValInt(x)); }
	void setY(int y) { set(CTL_Y, LWPValInt(y)); }
	void setData(void * data) { set(CTL_USERDATA, LWPValPtr(data)); }
	void setDraw(XCALL_(void) (*draw)(LWPControl *, void *, DrMode)) { set(CTL_USERDRAW, LWPValPtr(draw)); }
	void setEvent(XCALL_(void) (*event)(LWPControl *, void *)) { set(CTL_USEREVENT, LWPValPtr(event)); }
	// high level wraps [FIXME] more functions needed like the following:
	int getX() const { return getInt(CTL_X); }
	int getY() const { return getInt(CTL_Y); }
	int getHotX() const { return getInt(CTL_HOTX); }
	int getHotY() const { return getInt(CTL_HOTY); }
	int getHotW() const { return getInt(CTL_HOTW); }
	int getHotH() const { return getInt(CTL_HOTH); }
	int getMouseX() const { return getInt(CTL_MOUSEX); }
	int getMouseY() const { return getInt(CTL_MOUSEY); }
	LWPPanel getPanel() const { return (LWPanelID)getInt(CTL_PANEL); }
	void setXY(int x, int y) { setX(x); setY(y); }
};

#endif
