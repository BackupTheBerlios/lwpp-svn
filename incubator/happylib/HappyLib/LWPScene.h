
#ifndef _LWPSCENE_H
#define _LWPSCENE_H

#include <LWPRender.h>


//// [DOCUMENTME]

class LWPItem
{
public:
	LWItemID id;
	LWPItem() : id(0) {}
	LWPItem(LWItemID id) : id(id) {}
	LWPItem next() const { return LWPRender::itemInfo->next(id); }
	LWPItem firstChild() const { return LWPRender::itemInfo->firstChild(id); }
	LWPItem nextChild(LWItemID child) const { return LWPRender::itemInfo->nextChild(id, child); }
	LWPItem parent() const { return LWPRender::itemInfo->parent(id); }
	LWPItem target() const { return LWPRender::itemInfo->target(id); }
	LWPItem goal() const { return LWPRender::itemInfo->goal(id); }
	LWItemType type() const { return LWPRender::itemInfo->type(id); }
	char const * name() const { return LWPRender::itemInfo->name(id); }
	template<class ThatClass> void param(LWItemParam p, LWTime t, TripleT<double, ThatClass> & v) const { LWPRender::itemInfo->param(id, p, t, v); }
	template<class ThatClass> unsigned int limits(LWItemParam p, TripleT<double, ThatClass> & min, Vector & max) const { return LWPRender::itemInfo->limits(id, p, min, max); }
	char const * getTag(int t) const { return LWPRender::itemInfo->getTag(id, t); }
	void setTag(int t, char const * ts) { LWPRender::itemInfo->setTag(id, t, ts); }
	LWChanGroupID chanGroup() const { return LWPRender::itemInfo->chanGroup(id); }
	LWChannelID findChannel(String const & groupName, String const & channelName) const
	{
		LWChanGroupID mainGroup = chanGroup();
		LWChanGroupID groupID = mainGroup;
		if (groupName.length())
		{
			for (groupID = LWPPlugin::channelInfo->nextGroup(mainGroup, 0);
				groupID; groupID = LWPPlugin::channelInfo->nextGroup(mainGroup, groupID))
				if (!stricmp(LWPPlugin::channelInfo->groupName(groupID), groupName)) break;
		}
		if (!groupID) return 0;
		LWChannelID channelID;
		for (channelID = LWPPlugin::channelInfo->nextChannel(groupID, 0);
			channelID; channelID = LWPPlugin::channelInfo->nextChannel(groupID, channelID))
			if (!stricmp(LWPPlugin::channelInfo->channelName(channelID), channelName)) break;
		return channelID;
	}
	char const * server(char const * s, int t) const { return LWPRender::itemInfo->server(id, s, t); }
	unsigned int serverFlags(char const * s, int t) const { return LWPRender::itemInfo->serverFlags(id, s, t); }
	void controller(LWItemParam p, int type[3]) const { LWPRender::itemInfo->controller(id, p, type); }
	Point position(LWTime t = LWPRender::timeInfo->time) const	// position before parenting
	{
		Point v;
		param(LWIP_POSITION, t, v);
		return v;
	}
	Point wPosition(LWTime t = LWPRender::timeInfo->time) const	// position after parenting
	{
#ifdef LW6
		// [NOTE] work-around for LW wPostition bug
		if (type() == LWI_LIGHT)
		{
			for (int i = 0; i < LWPRender::LightPosQ.count(); i ++)
				if (LWPRender::LightPosQ[i].id == id) return LWPRender::LightPosQ[i].pos;
			return Pointf::Origin();
		}
#endif
		Point v;
		param(LWIP_W_POSITION, t, v);
		return v;
	}
	Vector rotation(LWTime t = LWPRender::timeInfo->time) const	// rotation relative to parent
	{
		Vector v;
		param(LWIP_ROTATION, t, v);
		return v;
	}
	Vector scale(LWTime t = LWPRender::timeInfo->time) const	// scale relative to parent
	{
		Vector v;
		param(LWIP_SCALING, t, v);
		return v;
	}
	Point pivot(LWTime t = LWPRender::timeInfo->time) const		// position of pivot
	{
		Point v;
		param(LWIP_PIVOT, t, v);
		return v;
	}
	Vector pivotRot(LWTime t = LWPRender::timeInfo->time) const	// pivot rotation
	{
		Vector v;
		param(LWIP_PIVOT_ROT, t, v);
		return v;
	}
	Vector right(LWTime t = LWPRender::timeInfo->time) const	// right w/ scale after parenting
	{
		Vector v;
		param(LWIP_RIGHT, t, v);
		return v;
	}
	Vector up(LWTime t = LWPRender::timeInfo->time) const		// up w/ scale after parenting
	{
		Vector v;
		param(LWIP_UP, t, v);
		return v;
	}
	Vector forward(LWTime t = LWPRender::timeInfo->time) const	// forward w/ scale after parenting
	{
		Vector v;
		param(LWIP_FORWARD, t, v);
		return v;
	}
	Vector wRight(LWTime t = LWPRender::timeInfo->time) const	// world right in item coords
	{
		Vector v;
		param(LWIP_W_RIGHT, t, v);
		return v;
	}
	Vector wUp(LWTime t = LWPRender::timeInfo->time) const		// world up in item coords
	{
		Vector v;
		param(LWIP_W_UP, t, v);
		return v;
	}
	Vector wForward(LWTime t = LWPRender::timeInfo->time) const	// world forward in item coords
	{
		Vector v;
		param(LWIP_W_FORWARD, t, v);
		return v;
	}
	Matrix4X4 wTransform(LWTime t = LWPRender::timeInfo->time) const
	{
		return Matrix4X4(right(t), up(t), forward(t), wPosition(t));
	}
	Matrix4X4 wTransformPivoted(LWTime t = LWPRender::timeInfo->time) const	// wTransform plus pivot displacement
	{
		return wTransform(t) * Matrix4X4(Vector::XHat(), Vector::YHat(), Vector::ZHat(), -pivot(t));
	}
	operator LWItemID () { return id; }
	int operator == (LWItemID i) const { return id == i; }
	static LWPItem first(LWItemType type, LWItemID id = LWITEM_NULL)
	{
		if (!LWPRender::itemInfo) return LWITEM_NULL;
		return LWPRender::itemInfo->first(type, id);
	}
	static LWPItem firstSelected(LWItemType type)
	{
		LWItemID const * items = LWPRender::interfaceInfo->selItems;
		LWItemID item;
		while ((item = *(items++)) != LWITEM_NULL) if (LWPRender::itemInfo->type(item) == type) break;
		return item;
	}
};


//// [DOCUMENTME]

class LWPObject : public LWPItem
{
public:
	LWPObject() {}
	LWPObject(LWItemID id) : LWPItem(id) {}
	LWPObject(LWPItem id) : LWPItem(id) {}
	LWPObject next() const { return (LWPObject)LWPItem::next(); }
	char const * filename() const { return LWPRender::objectInfo->filename(id); }
	int numPoints() const { return LWPRender::objectInfo->numPoints(id); }
	int numPolygons() const { return LWPRender::objectInfo->numPolygons(id); }
	unsigned int shadowOpts() const { return LWPRender::objectInfo->shadowOpts(id); }
	double dissolve(LWTime t) const { return LWPRender::objectInfo->dissolve(id, t); }
	LWMeshInfoID meshInfo(int frozen) const { return LWPRender::objectInfo->meshInfo(id, frozen); }
	unsigned int flags() const { return LWPRender::objectInfo->flags(id); }
	double fog(LWTime t) const { return LWPRender::objectInfo->fog(id, t); }
	LWTextureID dispMap() const { return LWPRender::objectInfo->dispMap(id); }
	LWTextureID clipMap() const { return LWPRender::objectInfo->clipMap(id); }
	static LWPObject first() { return (LWPObject)LWPItem::first(LWI_OBJECT); }
	static LWPObject firstSelected() { return (LWPObject)LWPItem::firstSelected(LWI_OBJECT); }
};


//// [DOCUMENTME]

class LWPLight : public LWPItem
{
public:
	LWPLight() {}
	LWPLight(LWItemID id) : LWPItem(id) {}
	LWPLight(LWPItem id) : LWPItem(id) {}
	LWPLight next() const { return (LWPLight)LWPItem::next(); }
	int type() const { return LWPRender::lightInfo->type(id); }
	void color(LWTime t, Color & color) const { LWPRender::lightInfo->color(id, t, color); }
	Color color(LWTime t = LWPRender::timeInfo->time) const { Color col; color(t, col); return col; }
	int shadowType() const { return LWPRender::lightInfo->shadowType(id); }
#ifdef LW7
	void coneAngles(double & radius, double & edge, LWTime t = LWPRender::timeInfo->time) const
	{ LWPRender::lightInfo->coneAngles(id, t, &radius, &edge); }
	double range(LWTime t = LWPRender::timeInfo->time) const { return LWPRender::lightInfo->range(id, t); }
	int falloff() const { return LWPRender::lightInfo->falloff(id); }
	double falloff(double d, LWTime t = LWPRender::timeInfo->time) const
	{
		int type = falloff();
		if (type == LWLFALL_OFF) return 1.0;
		double r = range(t);
		switch (type)
		{
		case LWLFALL_LINEAR:		return Max(0.0, 1.0 - d / r);
		case LWLFALL_INV_DIST:		return r / d;
		case LWLFALL_INV_DIST_2:	return Sq(r / d);
		};
		return 1.0;
	}
	int quality() const { return LWPRender::lightInfo->quality(id); }
	Color shadowColor(LWTime t = LWPRender::timeInfo->time) const { Color color; LWPRender::lightInfo->shadowColor(id, t, color); return color; }
#else
	void coneAngles(double & radius, double & edge) const { LWPRender::lightInfo->coneAngles(id, &radius, &edge); }
	double range() const { return LWPRender::lightInfo->range(id); }
#endif
	unsigned int flags() const { return LWPRender::lightInfo->flags(id); }
	unsigned int hasDiffuse() const { return !(flags() & LWLFL_NO_DIFFUSE); }
	unsigned int hasSpecular() const { return !(flags() & LWLFL_NO_SPECULAR); }
	int isSource(LWItemID source, Point const & pos, Vector const & dir, LWTime t = LWPRender::timeInfo->time) const
	{
		if ((LWPRender::sceneInfo->renderOpts & LWROPT_SHADOWTRACE) && shadowType() == LWLSHAD_RAYTRACE) switch (type())
		{
			case LWLIGHT_LINEAR:	// [FIXME] this can be fixen in L7
				// raySource is somewhere on the light's line.
				// Each access of the light's position returns a random position on the line.
			case LWLIGHT_AREA:		// [FIXME] this can be fixen in L7
				// raySource is somewhere on the light's area
				// Each access of the light's position returns a random position on the area.
			case LWLIGHT_POINT:	// same as LWLIGHT_SPOT
			case LWLIGHT_SPOT:
				// it's sufficient that dir points from the light
				if ((wPosition(t) >> pos) * dir > 0.9999) return 1;
				break;
			case LWLIGHT_DISTANT:
				// it's sufficient that dir points parallel to the light's direction
				if (~forward(t) * dir > 0.9999) return 1;
				break;
		}
		return 0;
	}
	static LWPLight first() { return (LWPLight)LWPItem::first(LWI_LIGHT); }
	static LWPLight firstSelected() { return (LWPLight)LWPItem::firstSelected(LWI_LIGHT); }
	static void ambient(LWTime t, Color & color) { LWPRender::lightInfo->ambient(t, color); }
	static Color ambient(LWTime t = LWPRender::timeInfo->time) { Color col; ambient(t, col); return col; }
};


//// [DOCUMENTME]

class LWPCamera : public LWPItem
{
public:
	LWPCamera() {}
	LWPCamera(LWItemID id) : LWPItem(id) {}
	LWPCamera(LWPItem id) : LWPItem(id) {}
	LWPCamera next() const { return (LWPCamera)LWPItem::next(); }
	void resolution(int & w, int & h) const { LWPRender::cameraInfo->resolution(id, &w, &h); }
	int width() const { int w, h; resolution(w, h); return w; }
	int height() const { int w, h; resolution(w, h); return h; }
	double pixelAspect(LWTime t) const { return LWPRender::cameraInfo->pixelAspect(id, t); }
	double zoomFactor(LWTime t) const { return LWPRender::cameraInfo->zoomFactor(id, t); }
	double focalLength(LWTime t) const { return LWPRender::cameraInfo->focalLength(id, t); }
	double focalDistance(LWTime t) const { return LWPRender::cameraInfo->focalDistance(id, t); }
	double fStop(LWTime t) const { return LWPRender::cameraInfo->fStop(id, t); }
	double blurLength(LWTime t) const { return LWPRender::cameraInfo->blurLength(id, t); }
	void fovAngles(LWTime t, double & horizontal, double & vertical) const
	{
		LWPRender::cameraInfo->fovAngles(id, t, &horizontal, &vertical);
	}
	int isSource(LWItemID source, Point const & pos, Vector const & dir) const
	{
		return (id == source);
	}
	Matrix4X4 w2sMatrix(LWTime t) const
	{
		double const zoom = zoomFactor(t);
		Matrix4X4 transW2S;
		transW2S(0,0) = right()[0];
		transW2S(0,1) = right()[1];
		transW2S(0,2) = right()[2];
		transW2S(0,3) = -(right() * (Vector)wPosition());
		transW2S(1,0) = up()[0];
		transW2S(1,1) = up()[1];
		transW2S(1,2) = up()[2];
		transW2S(1,3) = -(up() * (Vector)wPosition());
		transW2S(2,0) = forward()[0];
		transW2S(2,1) = forward()[1];
		transW2S(2,2) = forward()[2];
		transW2S(2,3) = -(forward() * (Vector)wPosition()) - zoom;
		transW2S(3,0) = forward()[0] / zoom;
		transW2S(3,1) = forward()[1] / zoom;
		transW2S(3,2) = forward()[2] / zoom;
		transW2S(3,3) = 1.0 + transW2S(2,3) / zoom;
		return transW2S;
	}
	int W2S(LWTime t, Point & pos, int zBuffer)	// [DOCUMENTME]
	{
		double const zoom = zoomFactor(t);
		double const halfHeight = height() * 0.5;
		double const heightScale = halfHeight;
		double const halfWidth = width() * 0.5;
		double const widthScale = heightScale / pixelAspect(t);
		double const widthExtent = pixelAspect(t) * width() / height();
		Point tpt;
		// transform point to frustum coords
		if (zBuffer)
		{
			double const iw = 1.0 / LWPRender::TransW2S.wTrans(pos, tpt);
			tpt[0] *= iw;
			tpt[1] *= iw;
			tpt[2] += zoom;
		}
		else tpt = LWPRender::TransW2S * pos;
		pos[0] = halfWidth + tpt[0] * widthScale;
		pos[1] = halfHeight - tpt[1] * heightScale;
		pos[2] = tpt[2];
		// is the point behind the camera or outside the image rectangle?
		if (tpt[2] <= 0 || fabs(tpt[1]) > 1 || fabs(tpt[0]) > widthExtent) return 0;
		return 1;
	}
	void S2W(LWTime t, Point & pos, int zBuffer)	// [DOCUMENTME]
	{
		double const zoom = zoomFactor(t);
		double const halfHeight = height() * 0.5;
		double const heightScale = halfHeight;
		double const halfWidth = width() * 0.5;
		double const widthScale = heightScale / pixelAspect(t);
		Point const tpt((pos[0] - halfWidth) / widthScale, (halfHeight - pos[1]) / heightScale, pos[2]);
		// transform point from frustum coords
		if (zBuffer) { }		// [FIXME] Fix z values
		pos = wPosition() + right() * tpt[0] + up() * tpt[1] + forward() * (tpt[2] + zoom);
	}
	Vector pixelDir(LWTime t, double sx, double sy)
	{
		double const zoom = zoomFactor(t);
		double const halfHeight = height() * 0.5;
		double const heightScale = halfHeight;
		double const halfWidth = width() * 0.5;
		double const widthScale = heightScale / pixelAspect(t);
		return ~(right(t) * ((sx + 0.5 - halfWidth) / widthScale)
			+ up(t) * ((halfHeight - sy - 0.5) / heightScale) + forward(t) * zoom);
	}
	static LWPCamera first() { return (LWPCamera)LWPItem::first(LWI_CAMERA); }
	static LWPCamera firstSelected() { return (LWPCamera)LWPItem::firstSelected(LWI_CAMERA); }
	static LWPCamera renderCamera(LWTime t = LWPRender::timeInfo->time) { return LWPRender::sceneInfo->renderCamera(t); }
};


//// [DOCUMENTME]

class LWPBone : public LWPItem
{
public:
	LWPBone() {}
	LWPBone(LWItemID id) : LWPItem(id) {}
	LWPBone(LWPItem id) : LWPItem(id) {}
	LWPBone next() const { return (LWPBone)LWPItem::next(); }
	unsigned int flags() const { return LWPRender::boneInfo->flags(id); }
	void restParam(LWItemParam p, Triple & v) const { LWPRender::boneInfo->restParam(id, p, v); }
	double restLength() const { return LWPRender::boneInfo->restLength(id); }
	void limits(double & inner, double & outer) const { LWPRender::boneInfo->limits(id, &inner, &outer); }
	char const * weightMap() const { return LWPRender::boneInfo->weightMap(id); }
	static LWPBone first(LWItemID id) { return (LWPBone)LWPItem::first(LWI_BONE, id); }
	static LWPBone firstSelected() { return (LWPBone)LWPItem::firstSelected(LWI_BONE); }
};


#endif
