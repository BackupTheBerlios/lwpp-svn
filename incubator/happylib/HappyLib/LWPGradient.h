
#ifndef _LWPGRADIENT_H
#define _LWPGRADIENT_H

#include "LWPPlugin.h"
#include "LWPSerialization.h"
#include "HLPoint.h"

#define LWID_SERIAL_GRAD LWID_('G','R','A','D')
class LWPControl;
class LWPPanel;

// [FIXME] put the gradient interface stuff into LWPIGradientControl : public LWPIAreaControl
//         in a different file and everything.

//// [DOCUMENTME] ////

class LWPGradientKey
{
public:
	LWPGradientKey * prev;
	LWPGradientKey * next;
	double frac;
	Color col;
	double val;
	LWPGradientKey() : prev(0), next(0), frac(0), col(Color::Black()), val(0) {}
	LWPGradientKey(LWPGradientKey const * k) : prev(0), next(0), frac(k->frac), col(k->col), val(k->val) {}
	LWError load(LWPLoadState const *);
	LWError save(LWPSaveState const *) const;
	int xOffset(int w) const { return int(frac * (w - 1) + 0.5); }
	void setXOffset(int, int);
	void draw(LWPPanel, LWRasterID, int, int, int, int, int, int) const;
};

class LWPGradient
{
public:
	LWPGradientKey * head;
	LWPGradientKey * tail;
	LWPGradientKey * sel;
	enum GradType { GRADTYPE_COLOR, GRADTYPE_VALUE } type;	// [FIXME] support both types
	int dragging;
	// [NOTE] head and tail are persistent
	LWPGradient() ;
	LWPGradient(LWPGradient const &);
	~LWPGradient();
	LWPGradient & operator = (LWPGradient const &);
	void remove(LWPGradientKey *);
	void append(LWPGradientKey *);
	void insertAfter(LWPGradientKey * before, LWPGradientKey * after);
	void addNewKey(double);
	void select(int, int, int, int, int);
	int keyCount() const;
	Color evaluateColor(double) const;
	LWError load(LWPLoadState const *);
	LWError save(LWPSaveState const *) const;
	void draw(LWPPanel, int, int, int, int) const;
	void event(LWPPanel, int, int, int, int, int, int, int[3], int);
	XCALL_(static void) Draw(LWPControl *, void *, DrMode);
	XCALL_(static void) Event(LWPControl *, void *);

	// serialization class
	class SerializationLWPGradient : public LWPSerialization::Serialization
	{
	public:
		SerializationLWPGradient(char const * c, void * v)
			: LWPSerialization::Serialization(c, v) {}
		virtual void load(LWPLoadState const * lState)
		{
			if (lState->readU4() != LWID_SERIAL_GRAD) return;
			((LWPGradient*)data)->load(lState);
		}
		virtual void save(LWPSaveState const * sState) const
		{
			sState->writeU4(LWID_SERIAL_GRAD);
			((LWPGradient*)data)->save(sState);
		}
		virtual void copy(LWPSerialization::Serialization const & from)
		{
			*(LWPGradient*)data = *(LWPGradient*)from.data;
		}
	};

	operator LWPSerialization::Serialization ()
	{
		return SerializationLWPGradient("", this);
	}
};

#endif
