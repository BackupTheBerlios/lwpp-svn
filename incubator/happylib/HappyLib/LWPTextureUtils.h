
#ifndef _LWPTEXTUREUTILS_H
#define _LWPTEXTUREUTILS_H

#include "LWPPlugin.h"
#include "LWPSerialization.h"

#define LWID_SERIAL_ENVL LWID_('E','N','V','L')
#define LWID_SERIAL_VPRM LWID_('V','P','R','M')
#define LWID_SERIAL_TXSP LWID_('T','X','S','P')


//// LWPChanGroup is a wrapper for LWChanGroup [DOCUMENTME] ////

class LWPChanGroup	// [FIXME] lots of stuff still to wrap...
{
private:
	LWChanGroupID id;
public:
	LWPChanGroup() : id(0) {}
	LWPChanGroup(LWChanGroupID parent, const char * name)
	{
		if (!LWPPlugin::envelopeFuncs) { id = 0; return; }
		id = LWPPlugin::envelopeFuncs->createGroup(parent, name);
	}
	~LWPChanGroup()
	{
		if (id) LWPPlugin::envelopeFuncs->destroyGroup(id);
	}
	operator LWChanGroupID () { return id; }
};


//// LWPEnvelope is a wrapper for LWEnvelope [DOCUMENTME] ////

class LWPEnvelope	// [FIXME] lots of stuff still to wrap...
{
private:
	LWEnvelopeID id;
public:
	LWPEnvelope() : id(0) {}
	LWPEnvelope(LWPChanGroup group, const char * name, int type)
	{
		if (!LWPPlugin::envelopeFuncs) { id = 0; return; }
		id = LWPPlugin::envelopeFuncs->create(group, name, type);
	}
	LWPEnvelope(const char * name, int type)
	{
		if (!LWPPlugin::envelopeFuncs) { id = 0; return; }
		id = LWPPlugin::envelopeFuncs->create(0, name, type);
	}
	~LWPEnvelope()
	{
		if (id) LWPPlugin::envelopeFuncs->destroy(id);
	}
	LWPEnvelope(LWPEnvelope const & e)
	{
		if (!LWPPlugin::envelopeFuncs) { id = 0; return; }
		id = LWPPlugin::envelopeFuncs->create(0, "", LWET_FLOAT);
		LWPPlugin::envelopeFuncs->copy(id, e.id);
	}
	LWPEnvelope & operator = (LWPEnvelope const & e)
	{
		if (&e == this || !LWPPlugin::envelopeFuncs) return *this;
		// no need to destroy, just copy (vParmFuncs->copy is smart enough to handle it)
		LWPPlugin::envelopeFuncs->copy(id, e.id);
		return *this;
	}
	operator LWEnvelopeID () { return id; }
	LWError load(LWPLoadState const * ls)
	{
		return LWPPlugin::envelopeFuncs->load(id, &ls->lState);
	}
	LWError save(LWPSaveState const * ss) const
	{
		return LWPPlugin::envelopeFuncs->save(id, &ss->sState);
	}
	LWError load(const char * fileName)
	{
		LWLoadState * ls = LWPPlugin::fileIOFuncs->openLoad(fileName, LWIO_ASCII);
		if (!ls) return "Unable to open file for reading";
		LWError e = LWPPlugin::envelopeFuncs->load(id, ls);
		LWPPlugin::fileIOFuncs->closeLoad(ls);
		return e;
	}
	LWError save(const char * fileName) const
	{
		LWSaveState * ss = LWPPlugin::fileIOFuncs->openSave(fileName, LWIO_ASCII);
		if (!ss) return "Unable to open file for writing";
		LWError e = LWPPlugin::envelopeFuncs->save(id, ss);
		LWPPlugin::fileIOFuncs->closeSave(ss);
		return e;
	}
	double evaluate(LWTime t) const
	{
		return LWPPlugin::envelopeFuncs->evaluate(id, t);
	}
	void clear()
	{
		while (true)
		{
			LWEnvKeyframeID key = LWPPlugin::envelopeFuncs->nextKey(id, 0);
			if (!key) break;
			LWPPlugin::envelopeFuncs->destroyKey(id, key);
		}
	}
	void addLinearKey(double time, double value)
	{
		LWEnvKeyframeID key = LWPPlugin::envelopeFuncs->createKey(id, time, value);
		LWPPlugin::envelopeFuncs->keySet(id, key, LWKEY_SHAPE, (void*)3);
	}

/*
  int              (*edit)         ( LWChanGroupID group,
                                     LWEnvelopeID env, int flags, void *data);
  int              (*envAge)       ( LWEnvelopeID env );
  LWEnvKeyframeID  (*createKey)    ( LWEnvelopeID env,
                                     LWTime time, double value);
  void             (*destroyKey)   ( LWEnvelopeID env, LWEnvKeyframeID key);
  LWEnvKeyframeID  (*findKey)      ( LWEnvelopeID env, LWTime time);
  LWEnvKeyframeID  (*nextKey)      ( LWEnvelopeID env, LWEnvKeyframeID key);
  LWEnvKeyframeID  (*prevKey)      ( LWEnvelopeID env, LWEnvKeyframeID key);
  int              (*keySet)       ( LWEnvelopeID env, LWEnvKeyframeID key,
                                     LWKeyTag tag, void *value);
  int              (*keyGet)       ( LWEnvelopeID env,
                                     LWEnvKeyframeID key, LWKeyTag tag, void *value);
  int              (*setEnvEvent)  ( LWEnvelopeID env,
                                     LWEnvEventFunc  ev, void *data); 
  int              (*egSet)        ( LWEnvelopeID env,
                                     LWChanGroupID group, int tag, void *value);
  int              (*egGet)        ( LWEnvelopeID env,
                                     LWChanGroupID group, int tag, void *value); 
*/

	// serialization class
	class SerializationLWPEnvelope : public LWPSerialization::Serialization
	{
	public:
		SerializationLWPEnvelope(char const * c, void * v) : LWPSerialization::Serialization(c, v) {}
		virtual void load(LWPLoadState const * lState)
		{
			if (lState->readU4() != LWID_SERIAL_ENVL) return;
			((LWPEnvelope*)data)->load(lState);
		}
		virtual void save(LWPSaveState const * sState) const
		{
			sState->writeU4(LWID_SERIAL_ENVL);
			((LWPEnvelope*)data)->save(sState);
		}
		virtual void copy(LWPSerialization::Serialization const & from)
		{
			*(LWPEnvelope*)data = *(LWPEnvelope*)from.data;
		}
	};

	operator LWPSerialization::Serialization ()
	{
		return SerializationLWPEnvelope("", this);
	}
};


//// LWPMicroPolygon is a wrapper for LWMicopol [DOCUMENTME] ////

class LWPMicroPolygon
{
public:
	Point oPos;
	Point wPos;
	Vector oScl;
	Vector gNorm;
	Vector wNorm;
	Vector ray;
	double bumpHeight;
	double txVal;
	double spotSize;
	Point raySource;
	double rayLength;
	double cosine;
	Matrix3X3 oXfrm;
	Matrix3X3 wXfrm;
	LWItemID objID;
	LWItemID srfID;
	LWPntID verts[4];
	float weights[4];
	Pointf vertsWPos[4];
	int polNum;
	int oAxis;
	int wAxis;
	int context;
	LWIlluminateFunc * illuminate;
	LWRayTraceFunc * rayTrace;
	LWRayCastFunc * rayCast;
	LWRayShadeFunc * rayShade;
	void * userData;
	LWPolID polygon;

	LWPMicroPolygon() { LWPPlugin::vParmFuncs->initMP(*this); }
	~LWPMicroPolygon() {}
	operator LWMicropolID () { return (LWMicropolID)this; }
};


//// LWPVParm is a wrapper for LW's vParm objects [DOCUMENTME] ////

class LWPVParm	// [FIXME] lots of stuff still to wrap...
{
private:
	LWVParmID id;
	int envType;
	int texType;
public:
	LWPVParm() : id(0) {}
	template <class Obj>
	LWPVParm(int envType, LWChanGroupID group, char const * channelName, Obj iVal,
		int texType = LWVPDT_NOTXTR, LWTxtrContextID context = 0,
		LWVP_EventFunc * event = 0, void * userData = 0)
		: envType(envType), texType(texType)
	{
		if (!LWPPlugin::vParmFuncs) { id = 0; return; }
		id = LWPPlugin::vParmFuncs->create(envType, texType);
		LWPPlugin::vParmFuncs->setVal(id, (double*)&iVal);

		if (group)
		{
			LWPPlugin::vParmFuncs->setup(id, channelName, group, context,
				event, LWPPlugin::channelInfo->groupName(group), userData);
		}
		else	// anonymous
		{
			LWPPlugin::vParmFuncs->setup(id, channelName, 0, context, event, "", userData);
		}
	}
	~LWPVParm()
	{
		if (!LWPPlugin::vParmFuncs) return;
		if (id) LWPPlugin::vParmFuncs->destroy(id);
	}
	LWPVParm(LWPVParm const & v) : envType(v.envType), texType(v.texType)
	{
		if (!LWPPlugin::vParmFuncs) { id = 0; return; }
		id = LWPPlugin::vParmFuncs->create(0, 0);
		LWPPlugin::vParmFuncs->copy(id, v.id);
	}
	LWPVParm & operator = (LWPVParm const & v)
	{
		if (&v == this || !LWPPlugin::vParmFuncs) return *this;
		// no need to destroy, just copy (vParmFuncs->copy is smart enough to handle it)
		envType = v.envType; texType = v.texType;
		LWPPlugin::vParmFuncs->copy(id, v.id);
		return *this;
	}
	template <class Obj> void setVal(Obj iVal)
	{
		if (!LWPPlugin::vParmFuncs) return;
		LWPPlugin::vParmFuncs->setVal(id, (double*)&iVal);
	}
	operator LWVParmID () { return id; }

	double evaluateFloat(LWTime t) const
	{
		double tmp[4];
		LWPPlugin::vParmFuncs->getVal(id, t, 0, tmp);
		return tmp[0];
	}
	double evaluateFloat(LWTime t, LWPMicroPolygon & mp) const
	{
		double tmp[4];
		LWPPlugin::vParmFuncs->getVal(id, t, mp, tmp);
		return tmp[0];
	}
	Color evaluateColor(LWTime t) const
	{
		Color tmp;
		LWPPlugin::vParmFuncs->getVal(id, t, 0, tmp);
		return tmp;
	}
	Color evaluateColor(LWTime t, LWPMicroPolygon & mp) const
	{
		Color tmp;
		LWPPlugin::vParmFuncs->getVal(id, t, mp, tmp);
		return tmp;
	}
	Vector evaluateVector(LWTime t) const
	{
		Vector tmp;
		LWPPlugin::vParmFuncs->getVal(id, t, 0, tmp);	// [FIXME] take a micropolygon
		return tmp;
	}
	Point evaluatePoint(LWTime t) const
	{
		Point tmp;
		LWPPlugin::vParmFuncs->getVal(id, t, 0, tmp);	// [FIXME] take a micropolygon
		return tmp;
	}
	LWError load(LWPLoadState const * ls)
	{
		return LWPPlugin::vParmFuncs->load(id, &ls->lState);
	}
	LWError save(LWPSaveState const * ss) const
	{
		return LWPPlugin::vParmFuncs->save(id, &ss->sState);
	}

	// serialization class
	class SerializationLWPVParm : public LWPSerialization::Serialization
	{
	public:
		SerializationLWPVParm(char const * c, void * v) : LWPSerialization::Serialization(c, v) {}
		virtual void load(LWPLoadState const * lState)
		{
			if (lState->readU4() != LWID_SERIAL_VPRM) return;
			((LWPVParm*)data)->load(lState);
		}
		virtual void save(LWPSaveState const * sState) const
		{
			sState->writeU4(LWID_SERIAL_VPRM);
			((LWPVParm*)data)->save(sState);
		}
		virtual void copy(LWPSerialization::Serialization const & from)
		{
			*(LWPVParm*)data = *(LWPVParm*)from.data;
		}
	};

	operator LWPSerialization::Serialization ()
	{
		return SerializationLWPVParm("", this);
	}
};


//// LWPTextureSpace is a wrapper for LWTxtr [DOCUMENTME] ////

class LWPTextureSpace	// [FIXME] possibly add channel stuff
{
public:
	LWTextureID id;
	int returnType;
	const char * name;
	LWTxtrContextID gc;
	LWPTextureSpace() : returnType(TRT_PERCENT), name(0), gc(0)
	{
		if (!LWPPlugin::textureFuncs) { id = 0; return; }
		id = LWPPlugin::textureFuncs->create(returnType, "", 0, 0);
	}
	LWPTextureSpace(int returnType, const char * name, LWTxtrContextID gc) : returnType(returnType), name(name), gc(gc)
	{
		id = LWPPlugin::textureFuncs->create(returnType, name, gc, 0);
	}
	~LWPTextureSpace()
	{
		if (id) LWPPlugin::textureFuncs->destroy(id);
	}
	LWPTextureSpace(LWPTextureSpace const & t) : returnType(t.returnType), name(t.name), gc(t.gc)
	{
		if (!LWPPlugin::textureFuncs) { id = 0; return; }
		id = LWPPlugin::textureFuncs->create(returnType, "", 0, 0);
		LWPPlugin::textureFuncs->copy(id, t.id);
	}
	LWPTextureSpace & operator = (LWPTextureSpace const & t)
	{
		if (&t == this || !LWPPlugin::textureFuncs) return *this;
		returnType = t.returnType;
		name = t.name;
		gc = t.gc;
		if (id) LWPPlugin::textureFuncs->destroy(id);
		id = LWPPlugin::textureFuncs->create(returnType, name, gc, 0);
		LWPPlugin::textureFuncs->copy(id, t.id);
		return *this;
	}
	void newTime(LWFrame f, LWTime t)
	{
		LWPPlugin::textureFuncs->newtime(id, t, f);
	}
	void cleanUp()
	{
		LWPPlugin::textureFuncs->cleanup(id);
	}
	double evaluate(LWPMicroPolygon & mp, double * rv)
	{
		// [FIXME] have evaluateFloat, evaluateColor, etc. instead of passing in rv
		return LWPPlugin::textureFuncs->evaluate(id, mp, rv);
	}
	LWError load(LWPLoadState const * ls)
	{
		if (id) LWPPlugin::textureFuncs->destroy(id);
		id = LWPPlugin::textureFuncs->create(returnType, name, gc, 0);
		LWPPlugin::textureFuncs->load(id, &ls->lState);
		return 0;
	}
	LWError save(LWPSaveState const * ss) const
	{
		LWPPlugin::textureFuncs->save(id, &ss->sState);
		return 0;
	}

	// serialization class
	class SerializationLWPTextureSpace : public LWPSerialization::Serialization
	{
	public:
		SerializationLWPTextureSpace(char const * c, void * v) : LWPSerialization::Serialization(c, v) {}
		virtual void load(LWPLoadState const * lState)
		{
			if (lState->readU4() != LWID_SERIAL_TXSP) return;
			((LWPTextureSpace*)data)->load(lState);
		}
		virtual void save(LWPSaveState const * sState) const
		{
			sState->writeU4(LWID_SERIAL_TXSP);
			((LWPTextureSpace*)data)->save(sState);
		}
		virtual void copy(LWPSerialization::Serialization const & from)
		{
			*(LWPTextureSpace*)data = *(LWPTextureSpace*)from.data;
		}
	};

	operator LWPSerialization::Serialization ()
	{
		return SerializationLWPTextureSpace("", this);
	}
};


/* [DOCUMENTME]
// LWVParm envelope types
#define LWVP_FLOAT        0x10
#define LWVP_PERCENT      0x20
#define LWVP_DIST         0x30
#define LWVP_ANGLE        0x40
#define LWVP_COLOR        0x51
#define LWVPF_VECTOR      0x01

// Texture data types
#define LWVPDT_NOTXTR      -1
#define LWVPDT_VECTOR       0
#define LWVPDT_COLOR        1
#define LWVPDT_PERCENT      2
#define LWVPDT_SCALAR       3
#define LWVPDT_DISPLACEMENT 4

// state flags
#define LWVPSF_ENV   (1<<0) // create (set) or has (get) envelope
#define LWVPSF_TEX   (1<<1) // create (set) or has (get) texture
*/


#endif

