
#include "LWPSerialization.h"


//// Serialization id tags ////

#define LWID_SERIAL_DOUB LWID_('D','O','U','B')
#define LWID_SERIAL_FLOA LWID_('F','L','O','A')
#define LWID_SERIAL_INTV LWID_('I','N','T','V')
#define LWID_SERIAL_STRN LWID_('S','T','R','N')
#define LWID_SERIAL_PONT LWID_('P','O','N','T')
#define LWID_SERIAL_COLR LWID_('C','O','L','R')
#define LWID_SERIAL_ITEM LWID_('I','T','E','M')


//// Serialization implementations ////

LWPSerialization::Serialization::Serialization(char const * id, void * data) : data(data)
{
	blockID.id = Ascii2Bin(id);
	blockID.token = id;
}

LWPSerialization::Serialization::Serialization(Serialization const & s)
{
	// copy whole thing, including vptr
	memcpy(this, &s, sizeof(*this));
}

LWPSerialization::Serialization const & LWPSerialization::Serialization::operator = (Serialization const & s)
{
	// copy whole thing, including vptr
	if (&s == this) return *this;
	memcpy(this, &s, sizeof(*this));
	return *this;
}

void LWPSerialization::SerializationDouble::load(LWPLoadState const * lState)
{
	if (lState->readU4() != LWID_SERIAL_DOUB) return;
	*(double*)data = lState->readFP();
}

void LWPSerialization::SerializationDouble::save(LWPSaveState const * sState) const
{
	sState->writeU4(LWID_SERIAL_DOUB);
	sState->writeFP(*(double*)data);
}

void LWPSerialization::SerializationDouble::copy(Serialization const & from)
{
	*(double*)data = *(double*)from.data;
}

void LWPSerialization::SerializationFloat::load(LWPLoadState const * lState)
{
	if (lState->readU4() != LWID_SERIAL_FLOA) return;
	*(float*)data = lState->readFP();
}

void LWPSerialization::SerializationFloat::save(LWPSaveState const * sState) const
{
	sState->writeU4(LWID_SERIAL_FLOA);
	sState->writeFP(*(float*)data);
}

void LWPSerialization::SerializationFloat::copy(Serialization const & from)
{
	*(float*)data = *(float*)from.data;
}

void LWPSerialization::SerializationInt::load(LWPLoadState const * lState)
{
	if (lState->readU4() != LWID_SERIAL_INTV) return;
	*(int*)data = lState->readI4();
}

void LWPSerialization::SerializationInt::save(LWPSaveState const * sState) const
{
	sState->writeU4(LWID_SERIAL_INTV);
	sState->writeI4(*(int*)data);
}

void LWPSerialization::SerializationInt::copy(Serialization const & from)
{
	*(int*)data = *(int*)from.data;
}

void LWPSerialization::SerializationString::load(LWPLoadState const * lState)
{
	if (lState->readU4() != LWID_SERIAL_STRN) return;
	*(String*)data = lState->readStr();
}

void LWPSerialization::SerializationString::save(LWPSaveState const * sState) const
{
	sState->writeU4(LWID_SERIAL_STRN);
	sState->writeStr(*(String*)data);
}

void LWPSerialization::SerializationString::copy(Serialization const & from)
{
	*(String*)data = *(String*)from.data;
}

void LWPSerialization::SerializationPoint::load(LWPLoadState const * lState)
{
	if (lState->readU4() != LWID_SERIAL_PONT) return;
	*(Point*)data = lState->readPoint();
}

void LWPSerialization::SerializationPoint::save(LWPSaveState const * sState) const
{
	sState->writeU4(LWID_SERIAL_PONT);
	sState->writePoint(*(Point*)data);
}

void LWPSerialization::SerializationPoint::copy(Serialization const & from)
{
	*(Point*)data = *(Point*)from.data;
}

void LWPSerialization::SerializationColor::load(LWPLoadState const * lState)
{
	if (lState->readU4() != LWID_SERIAL_COLR) return;
	*(Color*)data = lState->readColor();
}

void LWPSerialization::SerializationColor::save(LWPSaveState const * sState) const
{
	sState->writeU4(LWID_SERIAL_COLR);
	sState->writeColor(*(Color*)data);
}

void LWPSerialization::SerializationColor::copy(Serialization const & from)
{
	*(Color*)data = *(Color*)from.data;
}

void LWPSerialization::SerializationItem::load(LWPLoadState const * lState)
{
	if (lState->readU4() != LWID_SERIAL_ITEM) return;
	*(int*)data = lState->readI4();
}

void LWPSerialization::SerializationItem::save(LWPSaveState const * sState) const
{
	sState->writeU4(LWID_SERIAL_ITEM);
	sState->writeI4(*(int*)data);
}

void LWPSerialization::SerializationItem::copy(Serialization const & from)
{
	*(int*)data = *(int*)from.data;
}


//// LWPSerialization implementation ////

void LWPSerialization::add(char const * id, Serialization s)
{
	s.blockID.id = Ascii2Bin(id);
	s.blockID.token = id;
	serialize << s;
}

void LWPSerialization::add(char const * id, double & data)
{
	serialize << SerializationDouble(id, &data);
}

void LWPSerialization::add(char const * id, float & data)
{
	serialize << SerializationFloat(id, &data);
}

void LWPSerialization::add(char const * id, int & data)
{
	serialize << SerializationInt(id, &data);
}

void LWPSerialization::add(char const * id, String & data)
{
	serialize << SerializationString(id, &data);
}

void LWPSerialization::add(char const * id, Point & data)
{
	serialize << SerializationPoint(id, &data);
}

void LWPSerialization::add(char const * id, Color & data)
{
	serialize << SerializationColor(id, &data);
}

void LWPSerialization::add(char const * id, void * & data)
{
	serialize << SerializationItem(id, &data);
}

void LWPSerialization::addDeprecated(char const * id, Serialization s)
{
	s.blockID.id = Ascii2Bin(id);
	s.blockID.token = id;
	deprecate << s;
}

void LWPSerialization::addDeprecated(char const * id, double & data)
{
	deprecate << SerializationDouble(id, &data);
}

void LWPSerialization::addDeprecated(char const * id, float & data)
{
	deprecate << SerializationFloat(id, &data);
}

void LWPSerialization::addDeprecated(char const * id, int & data)
{
	deprecate << SerializationInt(id, &data);
}

void LWPSerialization::addDeprecated(char const * id, String & data)
{
	deprecate << SerializationString(id, &data);
}

void LWPSerialization::addDeprecated(char const * id, Point & data)
{
	deprecate << SerializationPoint(id, &data);
}

void LWPSerialization::addDeprecated(char const * id, Color & data)
{
	deprecate << SerializationColor(id, &data);
}

void LWPSerialization::addDeprecated(char const * id, void * & data)
{
	deprecate << SerializationItem(id, &data);
}

LWPSerialization::Serialization * LWPSerialization::findSerialization(LWID id)
{
	int i;
	for (i = 0; i < serialize.count(); i ++)
		if (serialize[i].blockID.id == id) return &serialize[i];
	for (i = 0; i < deprecate.count(); i ++)
		if (deprecate[i].blockID.id == id) return &deprecate[i];
	return 0;
}

unsigned int LWPSerialization::Ascii2Bin(char const * id)
{
	unsigned int idbin = 0;
	for (unsigned int i = 0; i < 4 && i < strlen(id); i ++) idbin = (idbin << 8) | id[i];
	return idbin;
}

LWError LWPSerialization::save(const LWPSaveState * sState) const
{
	// make a block ID for top ASCII block
	LWBlockIdent master;
	master.id = Ascii2Bin("PLUG");
	master.token = "PLUG";
	// write out master block if we're in ASCII
	if (sState->isAscii()) sState->beginBlk(master);
	// write out the serializations, each in its own block
	for (int i = 0; i < serialize.count(); i ++)
	{
		sState->beginBlk(serialize[i].blockID);
		serialize[i].save(sState);
		sState->endBlk();
	}
	// end master block if we're in ASCII
	if (sState->isAscii()) sState->endBlk();
	return 0;
}

LWError LWPSerialization::load(const LWPLoadState * lState)
{
	// make a block ID for top ASCII block
	LWBlockIdent master;
	master.id = Ascii2Bin("PLUG");
	master.token = "PLUG";
	// initialize a nice array of block IDs so we can use find()
	if (!blockList.count())
	{
		blockList.init(serialize.count() + deprecate.count() + 1);
		int i;
		for (i = 0; i < serialize.count(); i ++) blockList[i] = serialize[i].blockID;
		for (i = 0; i < deprecate.count(); i ++) blockList[i + serialize.count()] = deprecate[i].blockID;
		blockList.last().id = 0;	// 0 terminate the list
		blockList.last().token = 0;
	}
	// open the master block if we're in ASCII
	if (lState->isAscii())
	{
		if (!lState->openBlk(master)) return "ERROR: Plugin could not load data!";
	}
	// find each block and read the corresponding serialization
	for (Serialization * s = findSerialization(lState->findBlk(blockList));
		s; s = findSerialization(lState->findBlk(blockList)))
	{
		s->load(lState);
		lState->endBlk();
	}
	// end master block if we're in ASCII
	if (lState->isAscii()) lState->endBlk();
	return 0;
}

LWError LWPSerialization::copy(LWPSerialization const & from)
{
	for (int i = 0; i < serialize.count(); i ++) serialize[i].copy(from.serialize[i]);
	return 0;
}

