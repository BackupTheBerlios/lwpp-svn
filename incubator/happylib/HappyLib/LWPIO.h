
#ifndef _LWPIO_H
#define _LWPIO_H

#include "HLString.h"
#include "HLMatrix.h"
#include "HLMutex.h"
#include "LWPPlugin.h"

extern "C"
{
	#include <lwio.h>
}


//// LWPLoadState is a wrapper for LWLoadState [FIXME] more stuff to wrap ////

class LWPLoadState	// [NOTE] structure is same as LWLoadState
{
public:
	LWLoadState lState;
	int isAscii() const { return lState.ioMode == LWIO_ASCII; }
	LWID readID(const LWBlockIdent * id) const
	{
		return lState.readID(lState.readData, id);
	}
	LWID findBlk(const LWBlockIdent * id) const
	{
		return lState.findBlk(lState.readData, id);
	}
	void endBlk() const
	{
		lState.endBlk(lState.readData);
	}
	LWID openBlk(LWBlockIdent const & id) const
	{
		LWBlockIdent array[2];
		array[0] = id;
		array[1].id = 0;
		array[1].token = 0;
		return lState.findBlk(lState.readData, array);
	}
	float readFP() const
	{
		float val;
		lState.readFP(lState.readData, &val, 1);
		return val;
	}
	char readI1() const
	{
		char val;
		lState.readI1(lState.readData, &val, 1);
		return val;
	}
	short readI2() const
	{
		short val;
		lState.readI2(lState.readData, &val, 1);
		return val;
	}
	int readI4() const
	{
		long val;
		lState.readI4(lState.readData, &val, 1);
		return (int)val;
	}
	unsigned char readU1() const	// [NOTE] LW has a SERIOUS BUG with Unsigneds in ASCII files
	{
		unsigned char val;
		lState.readI1(lState.readData, (char*)&val, 1);
		return val;
	}
	unsigned short readU2() const	// [NOTE] LW has a SERIOUS BUG with Unsigneds in ASCII files
	{
		unsigned short val;
		lState.readI2(lState.readData, (short*)&val, 1);
		return val;
	}
	unsigned int readU4() const	// [NOTE] LW has a SERIOUS BUG with Unsigneds in ASCII files
	{
		unsigned long val;
		lState.readI4(lState.readData, (long*)&val, 1);
		return (unsigned int)val;
	}
	String readStr() const
	{
		char buf[1024];
		int len = lState.readStr(lState.readData, buf, 1024);
		return String(buf);
	}
	Point readPoint() const
	{
		Point point;
		point[0] = readFP();
		point[1] = readFP();
		point[2] = readFP();
		return point;
	}
	Color readColor() const
	{
		Color color;
		color[0] = readFP();
		color[1] = readFP();
		color[2] = readFP();
		return color;
	}
};


//// LWPSaveState is a wrapper for LWSaveState [FIXME] more stuff to wrap ////

class LWPSaveState	// [NOTE] structure is same as LWSaveState
{
public:
	LWSaveState sState;
	int isAscii() const { return sState.ioMode == LWIO_ASCII; }
	void writeID(const LWBlockIdent * id) const
	{
		sState.writeID(sState.writeData, id);
	}
	void beginBlk(LWBlockIdent const & blockID, int leaf = 0) const
	{
		sState.beginBlk(sState.writeData, &blockID, leaf);
	}
	void endBlk() const
	{
		sState.endBlk(sState.writeData);
	}
	void writeFP(double dval) const
	{
		float val = (float)dval;
		sState.writeFP(sState.writeData, &val, 1);
	}
	void writeI1(char val) const
	{
		sState.writeI1(sState.writeData, &val, 1);
	}
	void writeI2(short val) const
	{
		sState.writeI2(sState.writeData, &val, 1);
	}
	void writeI4(long val) const
	{
		sState.writeI4(sState.writeData, &val, 1);
	}
	void writeU1(unsigned char val) const	// [NOTE] LW has a SERIOUS BUG with Unsigneds in ASCII files
	{
		sState.writeI1(sState.writeData, (char*)&val, 1);
	}
	void writeU2(unsigned short val) const	// [NOTE] LW has a SERIOUS BUG with Unsigneds in ASCII files
	{
		sState.writeI2(sState.writeData, (short*)&val, 1);
	}
	void writeU4(unsigned long val) const	// [NOTE] LW has a SERIOUS BUG with Unsigneds in ASCII files
	{
		sState.writeI4(sState.writeData, (long*)&val, 1);
	}
	void writeStr(String const & str) const
	{
		sState.writeStr(sState.writeData, str);
	}
	void writePoint(Point const & point) const
	{
		writeFP(point[0]);
		writeFP(point[1]);
		writeFP(point[2]);
	}
	void writeColor(Color const & color) const
	{
		writeFP(color[0]);
		writeFP(color[1]);
		writeFP(color[2]);
	}
};


//// LWPGlobalIO is a filesystem-like thing for the Global Memory pool [FIXME] ////

class LWPGlobalIO
{
private:
	Queue<unsigned char> buf;		// buffer
	Stack<unsigned int> blockQ;		// locations of block headers while writing nested blocks
public:
	enum state { READ, WRITE } const rw;
	char const * const name;		// name of Global Pool entry
	LWPGlobalIO(char const * name, state rw) : name(name), rw(rw)
	{
		if (rw == READ)		// read the Global Pool entry into the buffer
		{
			// [FIXME]
		}
	}
	~LWPGlobalIO()
	{
		if (rw == WRITE)	// write the buffer into a Global Pool entry
		{
			// [FIXME]
		}
	}
	// [FIXME] need actual io funcs
};


//// LWPGlobalLoadState is a LWLoadState poser that uses the Global Memory pool as a filesystem ////

class LWPGlobalLoadState	// [NOTE] structure is same as LWLoadState
{
private:
	int 	ioMode;
	LWPGlobalIO	* io;
	int 	(*read)		(void *data, char  *, int len);
	int 	(*readI1)	(void *data, char  *, int n);
	int 	(*readI2)	(void *data, short *, int n);
	int 	(*readI4)	(void *data, long  *, int n);
	int 	(*readU1)	(void *data, unsigned char	*, int n);
	int 	(*readU2)	(void *data, unsigned short *, int n);
	int 	(*readU4)	(void *data, unsigned long	*, int n);
	int 	(*readFP)	(void *data, float *, int n);
	int 	(*readStr)	(void *data, char  *, int max);
	LWID	(*readID)	(void *data, const LWBlockIdent *);
	LWID	(*findBlk)	(void *data, const LWBlockIdent *);
	void	(*endBlk)	(void *data);
	int 	(*depth)	(void *data);
public:
	LWPGlobalLoadState(char const * name) : ioMode(LWIO_BINARY)
	{
		io = new LWPGlobalIO(name, LWPGlobalIO::READ);
		// [FIXME] init the callbacks
	}
	~LWPGlobalLoadState() { delete io; }
	// [FIXME] need actual io funcs
};


//// LWPGlobalSaveState is a LWSaveState poser that uses the Global Memory pool as a filesystem ////

class LWPGlobalSaveState	// [NOTE] structure is same as LWSaveState
{
private:
	int 	ioMode;
	LWPGlobalIO * io;
	void	(*write)	(void *data, char const *, int len);
	void	(*writeI1)  (void *data, char const *, int n);
	void	(*writeI2)  (void *data, const short *, int n);
	void	(*writeI4)  (void *data, const long *, int n);
	void	(*writeU1)  (void *data, const unsigned char *, int n);
	void	(*writeU2)  (void *data, const unsigned short *, int n);
	void	(*writeU4)  (void *data, const unsigned long *, int n);
	void	(*writeFP)  (void *data, const float *, int n);
	void	(*writeStr) (void *data, char const *);
	void	(*writeID)  (void *data, const LWBlockIdent *);
	void	(*beginBlk) (void *data, const LWBlockIdent *, int leaf);
	void	(*endBlk)	(void *data);
	int 	(*depth)	(void *data);
public:
	LWPGlobalSaveState(char const * name) : ioMode(LWIO_BINARY)
	{
		io = new LWPGlobalIO(name, LWPGlobalIO::WRITE);
		// [FIXME] init the callbacks
	}
	~LWPGlobalSaveState() { delete io; }
	// [FIXME] need actual io funcs
};

#endif
