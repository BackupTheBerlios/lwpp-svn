/// EXPERIMENTAL

#ifndef LWEnvelope_H
#define LWEnvelope_H

#define SHAPE_TCB   0
#define SHAPE_HERM  1
#define SHAPE_BEZI  2
#define SHAPE_LINE  3
#define SHAPE_STEP  4
#define SHAPE_BEZ2  5

#define BEH_RESET      0
#define BEH_CONSTANT   1
#define BEH_REPEAT     2
#define BEH_OSCILLATE  3
#define BEH_OFFSET     4
#define BEH_LINEAR     5

#define LWKeyFrameID     void*

struct LWKey
{
   LWEnvKeyframeID KeyID;
	double Time;
	double	value;
	int		shape;
	double	tension;
	double	continuity;
	double	bias;
	double	par0;
	double	par1;
	double	par2;
	double	par3;
};

class LWEnvelope
{
public:
	LWEnvelope();
   LWEnvelope(const LWEnvelope&);
	LWEnvelope(LWChannelID parentChannel);
	~LWEnvelope();
	LWEnvelope& operator=(const LWEnvelope&);
	bool Init(LWChannelID Channel);

	bool Save( const LWSaveState *ss);
	bool Load( const LWLoadState *ls);
	LWEnvelopeID CopyToGroup(LWEnvelopeID result);

	bool RestoreEnvelope();
	bool UpdateKeyTable();
	bool AddKey(LWKey NewKey);
	bool MoveFirst();
	bool MoveNext();
	double GetValue();
	double GetTime();
	
	int GetNumKeys();
	bool GetTimeSpan();
	double Evaluate(double Time);

	LWEnvelopeID	Envelope;
	LWChannelID		EnvChannel;
	int NumKeys;

	LWEnvKeyframeID KeyCursor;
	int index;
	bool RemoveAllKeys();
	double StartTime;
	double EndTime;
	double TimeSpan;

	int InternalCalculation;
	std::list<LWKey> Keys;
	Real LeftBlindEnd;
	Real RightBlindEnd;
    int  PrePostBehavior[ 2 ];
	float outgoing( std::list<LWKey>::iterator key0, std::list<LWKey>::iterator key1 );
	float incoming( std::list<LWKey>::iterator key0, std::list<LWKey>::iterator key1 );
    void hermite( float t, float *h1, float *h2, float *h3, float *h4 );

  /**
   * Gets the first key in envelope.
   * @return Pointer to the first key
   */
   LWKeyFrameID getFirst();

  /**
   * Gets the first key in envelope.
   * @param prev Key previous to the one requested.
   * @return Pointer to the next key
   */
   LWKeyFrameID getNext(LWKeyFrameID prev);
   void destroyKey(LWKeyFrameID toDestroy);
   int getNumKeys();
   LWKeyFrameID addKey(Real newValue, Real atTime);
   void setKeyValue(LWKeyFrameID key, Real value);




	bool		IsDeleted;
};

#endif