
#ifndef _LWPSURFACEOPTIONS_H
#define _LWPSURFACEOPTIONS_H

#include "LWPShader.h"


#define LWID_SERIAL_SFOP LWID_('S','F','O','P')
#define LWID_SERIAL_SVOP LWID_('S','V','O','P')


//// [DOCUMENTME] ////

class LWPSurfaceData
{
public:
	Color color;
	double luminous;
	double diffuse;
	double specular;
	double mirror;
	double transparency;
	double eta;
	double glossiness;
	double translucency;
	double colorHL;
	double colorFL;
	double addTransparency;
	double difSharpness;
	LWPSurfaceData()
		: color(Color::Black()), luminous(0), diffuse(1),
		specular(0), mirror(0),
		transparency(0), eta(1),
		glossiness(0.4),
		translucency(0), colorHL(0), colorFL(0),
		addTransparency(0), difSharpness(1)
	{}
	LWPSurfaceData(LWPShaderAccess const * sa)
		: color(sa->color), luminous(sa->luminous), diffuse(sa->diffuse),
		specular(sa->specular), mirror(sa->mirror),
		transparency(sa->transparency), eta(sa->eta),
		glossiness(sa->roughness == 0.0 ? 0.0 : 1.0/sa->roughness),
		translucency(sa->translucency), colorHL(sa->colorHL), colorFL(sa->colorFL),
		addTransparency(sa->addTransparency), difSharpness(sa->difSharpness)
	{}

	void load(const LWPLoadState * s)
	{
		color = s->readColor();
		luminous = s->readFP();
		diffuse = s->readFP();
		specular = s->readFP();
		mirror = s->readFP();
		transparency = s->readFP();
		eta = s->readFP();
		glossiness = s->readFP();
		translucency = s->readFP();
		colorHL = s->readFP();
		colorFL = s->readFP();
		addTransparency = s->readFP();
		difSharpness = s->readFP();
	}
	void save(const LWPSaveState * s) const
	{
		s->writeColor(color);
		s->writeFP(luminous);
		s->writeFP(diffuse);
		s->writeFP(specular);
		s->writeFP(mirror);
		s->writeFP(transparency);
		s->writeFP(eta);
		s->writeFP(glossiness);
		s->writeFP(translucency);
		s->writeFP(colorHL);
		s->writeFP(colorFL);
		s->writeFP(addTransparency);
		s->writeFP(difSharpness);
	}
};

class LWPSurfaceOptions
{
public:
	LWPSurfaceData surface;
	int doColor;
	int doLuminous;
	int doDiffuse;
	int doSpecular;
	int doMirror;
	int doTransparency;
	int doEta;
	int doGlossiness;
	int doTranslucency;
	int doColorHL;
	int doColorFL;
	int doAddTransparency;
	int doDifSharpness;

	LWPSurfaceOptions()
		: doColor(0), doLuminous(0), doDiffuse(0),
		doSpecular(0), doMirror(0),
		doTransparency(0), doEta(0),
		doGlossiness(0),
		doTranslucency(0), doColorHL(0), doColorFL(0),
		doAddTransparency(0), doDifSharpness(0)
	{}

	void load(const LWPLoadState * s)
	{
		doColor = s->readI1();
		doLuminous = s->readI1();
		doDiffuse = s->readI1();
		doSpecular = s->readI1();
		doMirror = s->readI1();
		doTransparency = s->readI1();
		doEta = s->readI1();
		doGlossiness = s->readI1();
		doTranslucency = s->readI1();
		doColorHL = s->readI1();
		doColorFL = s->readI1();
		doAddTransparency = s->readI1();
		doDifSharpness = s->readI1();
		surface.load(s);
	}
	void save(const LWPSaveState * s) const
	{
		s->writeI1(doColor);
		s->writeI1(doLuminous);
		s->writeI1(doDiffuse);
		s->writeI1(doSpecular);
		s->writeI1(doMirror);
		s->writeI1(doTransparency);
		s->writeI1(doEta);
		s->writeI1(doGlossiness);
		s->writeI1(doTranslucency);
		s->writeI1(doColorHL);
		s->writeI1(doColorFL);
		s->writeI1(doAddTransparency);
		s->writeI1(doDifSharpness);
		surface.save(s);
	}
	void alphaMix(LWPShaderAccess * sa, double alpha) const
	{
		if (doColor) sa->color = Lerp(sa->color, surface.color, alpha);
		if (doLuminous) sa->luminous = Lerp(sa->luminous, surface.luminous, alpha);
		if (doDiffuse) sa->diffuse = Lerp(sa->diffuse, surface.diffuse, alpha);
		if (doSpecular) sa->specular = Lerp(sa->specular, surface.specular, alpha);
		if (doMirror) sa->mirror = Lerp(sa->mirror, surface.mirror, alpha);
		if (doTransparency) sa->transparency = Lerp(sa->transparency, surface.transparency, alpha);
		if (doEta) sa->eta = Lerp(sa->eta, surface.eta, alpha);
		if (doGlossiness) sa->roughness = Lerp(sa->roughness, surface.glossiness == 0.0 ? 0.0 : 1.0/surface.glossiness, alpha);
		if (doTranslucency) sa->translucency = Lerp(sa->translucency, surface.translucency, alpha);
		if (doColorHL) sa->colorHL = Lerp(sa->colorHL, surface.colorHL, alpha);
		if (doColorFL) sa->colorFL = Lerp(sa->colorFL, surface.colorFL, alpha);
		if (doAddTransparency) sa->addTransparency = Lerp(sa->addTransparency, surface.addTransparency, alpha);
		if (doDifSharpness) sa->difSharpness = Lerp(sa->difSharpness, surface.difSharpness, alpha);
	}


	// serialization class
	class SerializationLWPSurfaceOptions : public LWPSerialization::Serialization
	{
	public:
		SerializationLWPSurfaceOptions(char const * c, void * v) : LWPSerialization::Serialization(c, v) {}
		virtual void load(LWPLoadState const * lState)
		{
			if (lState->readU4() != LWID_SERIAL_SFOP) return;
			((LWPSurfaceOptions*)data)->load(lState);
		}
		virtual void save(LWPSaveState const * sState) const
		{
			sState->writeU4(LWID_SERIAL_SFOP);
			((LWPSurfaceOptions*)data)->save(sState);
		}
		virtual void copy(LWPSerialization::Serialization const & from)
		{
			*(LWPSurfaceOptions*)data = *(LWPSurfaceOptions*)from.data;
		}
	};

	operator LWPSerialization::Serialization ()
	{
		return SerializationLWPSurfaceOptions("", this);
	}
};


//// [DOCUMENTME] ////

class LWPVSurfaceData
{
public:
	LWPVParm color;
	LWPVParm luminous;
	LWPVParm diffuse;
	LWPVParm specular;
	LWPVParm glossiness;
	LWPVParm mirror;
	LWPVParm transparency;
	LWPVParm eta;
	LWPVParm translucency;
	LWPVParm colorHL;
	LWPVParm colorFL;
	LWPVParm addTransparency;
	LWPVParm difSharpness;
	LWPVSurfaceData() {}
	LWPVSurfaceData(LWChanGroupID group, int texType = LWVPDT_NOTXTR, LWTxtrContextID context = 0)
		: color(LWVP_COLOR, group, "Color", Color::Black(), texType, context),
		luminous(LWVP_PERCENT, group, "Luminosity", 0.0, texType, context),
		diffuse(LWVP_PERCENT, group, "Diffuse", 1.0, texType, context),
		specular(LWVP_PERCENT, group, "Specularity", 0.0, texType, context),
		glossiness(LWVP_PERCENT, group, "Glossiness", 0.4, texType, context),
		mirror(LWVP_PERCENT, group, "Reflectivity", 0.0, texType, context),
		transparency(LWVP_PERCENT, group, "Transparency", 0.0, texType, context),
		eta(LWVP_FLOAT, group, "Refraction Index", 1.0, texType, context),
		translucency(LWVP_PERCENT, group, "Translucency", 0.0, texType, context),
		colorHL(LWVP_PERCENT, group, "Color Highlights", 0.0, texType, context),
		colorFL(LWVP_PERCENT, group, "Color Filter", 0.0, texType, context),
		addTransparency(LWVP_PERCENT, group, "Additive Transparency", 0.0, texType, context),
		difSharpness(LWVP_PERCENT, group, "Diffuse Sharpness", 1.0, texType, context)
	{}

	void load(const LWPLoadState * s)
	{
		color.load(s);
		luminous.load(s);
		diffuse.load(s);
		specular.load(s);
		mirror.load(s);
		transparency.load(s);
		eta.load(s);
		glossiness.load(s);
		translucency.load(s);
		colorHL.load(s);
		colorFL.load(s);
		addTransparency.load(s);
		difSharpness.load(s);
	}
	void save(const LWPSaveState * s) const
	{
		color.save(s);
		luminous.save(s);
		diffuse.save(s);
		specular.save(s);
		mirror.save(s);
		transparency.save(s);
		eta.save(s);
		glossiness.save(s);
		translucency.save(s);
		colorHL.save(s);
		colorFL.save(s);
		addTransparency.save(s);
		difSharpness.save(s);
	}
};

class LWPVSurfaceOptions
{
public:
	LWPVSurfaceData surface;
	int doColor;
	int doLuminous;
	int doDiffuse;
	int doSpecular;
	int doMirror;
	int doTransparency;
	int doEta;
	int doGlossiness;
	int doTranslucency;
	int doColorHL;
	int doColorFL;
	int doAddTransparency;
	int doDifSharpness;

	LWPVSurfaceOptions() {}
	LWPVSurfaceOptions(LWChanGroupID group, int texType = LWVPDT_NOTXTR, LWTxtrContextID context = 0)
		: surface(group, texType, context),
		doColor(0), doLuminous(0), doDiffuse(0),
		doSpecular(0), doMirror(0),
		doTransparency(0), doEta(0),
		doGlossiness(0),
		doTranslucency(0), doColorHL(0), doColorFL(0),
		doAddTransparency(0), doDifSharpness(0)
	{}

	void load(const LWPLoadState * s)
	{
		doColor = s->readI1();
		doLuminous = s->readI1();
		doDiffuse = s->readI1();
		doSpecular = s->readI1();
		doMirror = s->readI1();
		doTransparency = s->readI1();
		doEta = s->readI1();
		doGlossiness = s->readI1();
		doTranslucency = s->readI1();
		doColorHL = s->readI1();
		doColorFL = s->readI1();
		doAddTransparency = s->readI1();
		doDifSharpness = s->readI1();
		surface.load(s);
	}
	void save(const LWPSaveState * s) const
	{
		s->writeI1(doColor);
		s->writeI1(doLuminous);
		s->writeI1(doDiffuse);
		s->writeI1(doSpecular);
		s->writeI1(doMirror);
		s->writeI1(doTransparency);
		s->writeI1(doEta);
		s->writeI1(doGlossiness);
		s->writeI1(doTranslucency);
		s->writeI1(doColorHL);
		s->writeI1(doColorFL);
		s->writeI1(doAddTransparency);
		s->writeI1(doDifSharpness);
		surface.save(s);
	}
	void alphaMix(LWPShaderAccess * sa, double alpha) const
	{
		LWTime t = LWPRender::timeInfo->time;
		if (doColor) sa->color = Lerp(sa->color, surface.color.evaluateColor(t), alpha);
		if (doLuminous) sa->luminous = Lerp(sa->luminous, surface.luminous.evaluateFloat(t), alpha);
		if (doDiffuse) sa->diffuse = Lerp(sa->diffuse, surface.diffuse.evaluateFloat(t), alpha);
		if (doSpecular) sa->specular = Lerp(sa->specular, surface.specular.evaluateFloat(t), alpha);
		if (doGlossiness) sa->roughness = Lerp(sa->roughness, surface.glossiness.evaluateFloat(t) == 0.0 ? 0.0 : 1.0/surface.glossiness.evaluateFloat(t), alpha);
		if (doMirror) sa->mirror = Lerp(sa->mirror, surface.mirror.evaluateFloat(t), alpha);
		if (doTransparency) sa->transparency = Lerp(sa->transparency, surface.transparency.evaluateFloat(t), alpha);
		if (doEta) sa->eta = Lerp(sa->eta, surface.eta.evaluateFloat(t), alpha);
		if (doTranslucency) sa->translucency = Lerp(sa->translucency, surface.translucency.evaluateFloat(t), alpha);
		if (doColorHL) sa->colorHL = Lerp(sa->colorHL, surface.colorHL.evaluateFloat(t), alpha);
		if (doColorFL) sa->colorFL = Lerp(sa->colorFL, surface.colorFL.evaluateFloat(t), alpha);
		if (doAddTransparency) sa->addTransparency = Lerp(sa->addTransparency, surface.addTransparency.evaluateFloat(t), alpha);
		if (doDifSharpness) sa->difSharpness = Lerp(sa->difSharpness, surface.difSharpness.evaluateFloat(t), alpha);
	}


	// serialization class
	class SerializationLWPVSurfaceOptions : public LWPSerialization::Serialization
	{
	public:
		SerializationLWPVSurfaceOptions(char const * c, void * v) : LWPSerialization::Serialization(c, v) {}
		virtual void load(LWPLoadState const * lState)
		{
			if (lState->readU4() != LWID_SERIAL_SFOP) return;
			((LWPVSurfaceOptions*)data)->load(lState);
		}
		virtual void save(LWPSaveState const * sState) const
		{
			sState->writeU4(LWID_SERIAL_SFOP);
			((LWPVSurfaceOptions*)data)->save(sState);
		}
		virtual void copy(LWPSerialization::Serialization const & from)
		{
			*(LWPVSurfaceOptions*)data = *(LWPVSurfaceOptions*)from.data;
		}
	};

	operator LWPSerialization::Serialization ()
	{
		return SerializationLWPVSurfaceOptions("", this);
	}
};

#endif

