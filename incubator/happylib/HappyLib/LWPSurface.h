
#ifndef _LWPSURFACE_H
#define _LWPSURFACE_H

#include <LWPPlugin.h>

class LWPSurface
{
public:
	LWSurfaceID id;
	LWPSurface() : id(0) {}
	LWPSurface(LWSurfaceID id) : id(id) {}
	LWPSurface next() const { return LWPPlugin::surfaceFuncs->next(id); }
	char const * name() const { return LWPPlugin::surfaceFuncs->name(id); }
	char const * sceneObject() const { return LWPPlugin::surfaceFuncs->sceneObject(id); }
	int getInt(char const * channel) const { return LWPPlugin::surfaceFuncs->getInt(id, channel); }
	double const * getDouble(char const * channel) const { return LWPPlugin::surfaceFuncs->getFlt(id, channel); }
	LWEnvelopeID getEnvelope(char const * channel) const { return LWPPlugin::surfaceFuncs->getEnv(id, channel); }
	LWTextureID getTexture(char const * channel) const { return LWPPlugin::surfaceFuncs->getTex(id, channel); }
	LWImageID getImage(char const * channel) const { return LWPPlugin::surfaceFuncs->getImg(id, channel); }
	double getDoubleE(char const * channel, LWTime time) const
	{
		LWEnvelopeID e = getEnvelope(channel);
		if (!e) return *getDouble(channel);
		return LWPPlugin::envelopeFuncs->evaluate(e, time);
	}
	double getDoubleET(char const * channel, LWMicropolID mp, LWTime time) const
	{
		double d = getDoubleE(channel, time);
		LWTextureID t = getTexture(channel);
		if (!t) return d;
		mp->txVal = d;
		LWPPlugin::textureFuncs->evaluate(t, mp, &d);
		return d;
	}
	Color color() const
	{
		Color c;
		c.set(getDouble(SURF_COLR));
		return c;
	}
	Color colorET(LWTime time, LWMicropolID mp) const
	{
		Color c;
		LWEnvelopeID e = getEnvelope("BaseColor.R");
		if (e)
		{
			c[0] = LWPPlugin::envelopeFuncs->evaluate(e, time);
			e = getEnvelope("BaseColor.G");
			c[1] = LWPPlugin::envelopeFuncs->evaluate(e, time);
			e = getEnvelope("BaseColor.B");
			c[2] = LWPPlugin::envelopeFuncs->evaluate(e, time);
		}
		else c.set(getDouble(SURF_COLR));
		LWTextureID t = getTexture(SURF_COLR);
		if (!t) return c;
		mp->txVal = c[0] * 0.2989 + c[1] * 0.5866 + c[2] * 0.1144;
		LWPPlugin::textureFuncs->evaluate(t, mp, c);
		return c;
	}
	Color colorETMAP(LWTime time, LWMicropolID mp) const
	{
		// get base color
		Color c = colorET(time, mp);
		// check for vertices
		if (!mp->verts[0]) return c;
		// check vertex coloring weight
		double weight = vertexColoringE(time);
		if (!weight) return c;
		// check for map name
		char const * colorMap = getColorVMap();
		if (!colorMap) return c;
		// check for mesh
		LWMeshInfo * meshInfo = LWPRender::objectInfo->meshInfo(mp->objID, 1);
		if (!meshInfo) return c;	// null object
		void * map = meshInfo->pntVLookup(meshInfo, LWVMAP_RGB, colorMap);
		// check for map
		if (!map) map = meshInfo->pntVLookup(meshInfo, LWVMAP_RGBA, colorMap);
		if (!map) return c;			// no map found
		int channels = meshInfo->pntVSelect(meshInfo, map);
		if (channels != 3 && channels != 4) return c;	// weird map
		// compute interpolated color
		Color cMap = Color::Black();
		double alphaMap = 0.0;
		float val[4];
		Colorf & col = *(Colorf*)val;
		for (int i = 0; i < 4; i ++)
		{
			int mapped = meshInfo->pntVPGet(meshInfo, mp->verts[i], mp->polygon, col);
			if (!mapped) mapped = meshInfo->pntVGet(meshInfo, mp->verts[i], col);
			if (!mapped) continue;
			cMap += col * mp->weights[i];
			alphaMap += channels == 4 ? val[3] * mp->weights[i] : mp->weights[i];
		}
		return Lerp(c, c * cMap, weight * alphaMap);
	}
	double luminosity() const { return *getDouble(SURF_LUMI); }
	double luminosityET(LWTime time, LWMicropolID mp) const { return getDoubleET(SURF_LUMI, mp, time); }
	double diffuse() const { return *getDouble(SURF_DIFF); }
	double diffuseET(LWTime time, LWMicropolID mp) const { return getDoubleET(SURF_DIFF, mp, time); }
	double specularity() const { return *getDouble(SURF_SPEC); }
	double specularityET(LWTime time, LWMicropolID mp) const { return getDoubleET(SURF_SPEC, mp, time); }
	double reflectivity() const { return *getDouble(SURF_REFL); }
	double reflectivityET(LWTime time, LWMicropolID mp) const { return getDoubleET(SURF_REFL, mp, time); }
	double transparency() const { return *getDouble(SURF_TRAN); }
	double transparencyET(LWTime time, LWMicropolID mp) const { return getDoubleET(SURF_TRAN, mp, time); }
	double translucency() const { return *getDouble(SURF_TRNL); }
	double translucencyET(LWTime time, LWMicropolID mp) const { return getDoubleET(SURF_TRNL, mp, time); }
	double eta() const { return *getDouble(SURF_RIND); }
	double etaET(LWTime time, LWMicropolID mp) const { return getDoubleET(SURF_RIND, mp, time); }
	double bump() const { return *getDouble(SURF_BUMP); }
	Vector bumpET(LWTime time, LWMicropolID mp) const
	{
		double amount;
		LWEnvelopeID e = getEnvelope(SURF_BUMP);
		if (e) amount = LWPPlugin::envelopeFuncs->evaluate(e, time);
		else amount = *getDouble(SURF_BUMP);
		LWTextureID t = getTexture(SURF_BUMP);
		if (!t || !amount) return Vector::Origin();
		mp->txVal = 0.0;
		Vector v = Vector::Origin();
		LWPPlugin::textureFuncs->evaluate(t, mp, v);
		return v * amount;
	}
	double glossiness() const { return *getDouble(SURF_GLOS); }
	double glossinessET(LWTime time, LWMicropolID mp) const { return getDoubleET(SURF_GLOS, mp, time); }
	double specialBuffer1() const { return *getDouble(SURF_BUF1); }
	double specialBuffer2() const { return *getDouble(SURF_BUF2); }
	double specialBuffer3() const { return *getDouble(SURF_BUF3); }
	double specialBuffer4() const { return *getDouble(SURF_BUF4); }
	double diffuseSharpness() const { return *getDouble(SURF_SHRP); }
	double diffuseSharpnessE(LWTime time) const { return getDoubleE(SURF_SHRP, time); }
	double smoothingAngle() const { return *getDouble(SURF_SMAN); }
	double reflectionSeamAngle() const { return *getDouble(SURF_RSAN); }
	double reflectionSeamAngleE(LWTime time) const { return getDoubleE(SURF_RSAN, time); }
	double refractionSeamAngle() const { return *getDouble(SURF_TSAN); }
	double refractionSeamAngleE(LWTime time) const { return getDoubleE(SURF_TSAN, time); }
#ifdef LW7
	double reflectionBlurring() const { return *getDouble(SURF_RBLR); }
	double reflectionBlurringE(LWTime time) const { return getDoubleE(SURF_RBLR, time); }
	double refractionBlurring() const { return *getDouble(SURF_TBLR); }
	double refractionBlurringE(LWTime time) const { return getDoubleE(SURF_TBLR, time); }
#endif
	double colorFilter() const { return *getDouble(SURF_CLRF); }
	double colorFilterE(LWTime time) const { return getDoubleE(SURF_CLRF, time); }
	double colorHighlights() const { return *getDouble(SURF_CLRH); }
	double colorHighlightsE(LWTime time) const { return getDoubleE(SURF_CLRH, time); }
	double additiveTransparency() const { return *getDouble(SURF_ADTR); }
	double additiveTransparencyE(LWTime time) const { return getDoubleE(SURF_ADTR, time); }
	int alphaType() const { return getInt(SURF_ALPH); }
	double alpha() const
	{
		int alphaOptions = getInt(SURF_ALPH);
		if (alphaOptions == 0) return -1.0;	// unaffected
		if (alphaOptions == 1) return *getDouble(SURF_AVAL);
		if (alphaOptions == 2) return 1.0 - transparency();
		return 0.0;	// shadow density [FIXME]
	}
	double alphaET(LWTime time, LWMicropolID mp) const
	{
		int alphaOptions = getInt(SURF_ALPH);
		if (alphaOptions == 0) return -1.0;	// unaffected
		if (alphaOptions == 1) return *getDouble(SURF_AVAL);
		if (alphaOptions == 2) return 1.0 - transparencyET(time, mp);
		return 0.0;	// shadow density [FIXME]
	}
	/* [FIXME] add as needed
#define SURF_GVAL  "GlowValue"E
#define SURF_LCOL  "LineColor"
#define SURF_LSIZ  "LineSize"
*/
	int useReflectionImage() const { return (getInt(SURF_RFOP) & 2); }
	int useRefractionImage() const { return (getInt(SURF_TROP) & 2); }
	int traceReflection() const { return (getInt(SURF_RFOP) & 1); }
	int traceRefraction() const { return (getInt(SURF_TROP) & 1); }
	int doubleSided() const { return getInt(SURF_SIDE) == 3; }
/*
#define SURF_GLOW  "Glow"E
#define SURF_LINE  "RenderOutlines"
*/
	LWImageID reflectionImage() const { return getImage(SURF_RIMG); }
	LWImageID refractionImage() const { return getImage(SURF_TIMG); }
	double vertexColoring() const { return *getDouble(SURF_VCOL); }
	double vertexColoringE(LWTime time) const { return getDoubleE(SURF_VCOL, time); }
	// other functions
	int isOpaque() const { return !*getDouble(SURF_TRAN) && !getEnvelope(SURF_TRAN) && !getTexture(SURF_TRAN); }
	char const * getColorVMap() const { return LWPPlugin::surfaceFuncs->getColorVMap(id); }
	void setColorVMap(char const * vmapName, int type) { LWPPlugin::surfaceFuncs->setColorVMap(id, vmapName, type); }
	LWChanGroupID chanGroup() const { return LWPPlugin::surfaceFuncs->chanGrp(id); }
	operator LWSurfaceID () { return id; }
	operator LWSurfaceID const () const { return id; }
	static LWPSurface create(char const * objName, char const * surfName)
	{ return LWPPlugin::surfaceFuncs->create(objName, surfName); }
	static LWPSurface first()
	{
		if (!LWPPlugin::surfaceFuncs) return 0;
		return LWPPlugin::surfaceFuncs->first();
	}
	LWPSurface * byName(char const * surfName, char const * objName)
	{
		if (!LWPPlugin::surfaceFuncs) return 0;
		return (LWPSurface *)LWPPlugin::surfaceFuncs->byName(surfName, objName);
	}
	LWPSurface * byObject(char const * surfName)
	{
		if (!LWPPlugin::surfaceFuncs) return 0;
		return (LWPSurface *)LWPPlugin::surfaceFuncs->byObject(surfName);
	}
};

#endif
