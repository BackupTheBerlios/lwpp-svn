
#ifndef _LWPSERIALIZATION_H
#define _LWPSERIALIZATION_H

#include "LWPIO.h"


//// Serialization classes [DOCUMENTME] ////

class LWPSerialization	// [FIXME] more Serialization types to add
{
public:
	class Serialization
	{
	public:
		LWBlockIdent blockID;
		void * data;
		Serialization() {}
		Serialization(char const *, void *);
		Serialization(Serialization const &);
		Serialization const & operator = (Serialization const &);
		virtual void load(LWPLoadState const *) {}
		virtual void save(LWPSaveState const *) const {}
		virtual void copy(Serialization const &) {}
	};
	class SerializationDouble : public Serialization
	{
	public:
		SerializationDouble(char const * c, void * v) : Serialization(c, v) {}
		virtual void load(LWPLoadState const *);
		virtual void save(LWPSaveState const *) const;
		virtual void copy(Serialization const &);
	};
	class SerializationFloat : public Serialization
	{
	public:
		SerializationFloat(char const * c, void * v) : Serialization(c, v) {}
		virtual void load(LWPLoadState const *);
		virtual void save(LWPSaveState const *) const;
		virtual void copy(Serialization const &);
	};
	class SerializationInt : public Serialization
	{
	public:
		SerializationInt(char const * c, void * v) : Serialization(c, v) {}
		virtual void load(LWPLoadState const *);
		virtual void save(LWPSaveState const *) const;
		virtual void copy(Serialization const &);
	};
	class SerializationString : public Serialization
	{
	public:
		SerializationString(char const * c, void * v) : Serialization(c, v) {}
		virtual void load(LWPLoadState const *);
		virtual void save(LWPSaveState const *) const;
		virtual void copy(Serialization const &);
	};
	class SerializationPoint : public Serialization
	{
	public:
		SerializationPoint(char const * c, void * v) : Serialization(c, v) {}
		virtual void load(LWPLoadState const *);
		virtual void save(LWPSaveState const *) const;
		virtual void copy(Serialization const &);
	};
	class SerializationColor : public Serialization
	{
	public:
		SerializationColor(char const * c, void * v) : Serialization(c, v) {}
		virtual void load(LWPLoadState const *);
		virtual void save(LWPSaveState const *) const;
		virtual void copy(Serialization const &);
	};
	class SerializationItem : public Serialization
	{
	public:
		SerializationItem(char const * c, void * v) : Serialization(c, v) {}
		virtual void load(LWPLoadState const *);
		virtual void save(LWPSaveState const *) const;
		virtual void copy(Serialization const &);
	};

	// member data
	Queue<Serialization> serialize;
	Queue<Serialization> deprecate;
	Array<LWBlockIdent> blockList;

	// functions
	void clear() { serialize.clear(); }
	void add(char const *, Serialization);				// for external serializations
	void add(char const *, double &);
	void add(char const *, float &);
	void add(char const *, int &);
	void add(char const *, String &);
	void add(char const *, Point &);
	void add(char const *, Color &);
	void add(char const *, void * &);
	void addDeprecated(char const *, Serialization);	// for external serializations
	void addDeprecated(char const *, double &);
	void addDeprecated(char const *, float &);
	void addDeprecated(char const *, int &);
	void addDeprecated(char const *, String &);
	void addDeprecated(char const *, Point &);
	void addDeprecated(char const *, Color &);
	void addDeprecated(char const *, void * &);
	LWError load(const LWPLoadState *);
	LWError save(const LWPSaveState *) const;
	LWError copy(LWPSerialization const &);
	Serialization * findSerialization(LWID id);
	static unsigned int Ascii2Bin(char const *);
};

#endif
