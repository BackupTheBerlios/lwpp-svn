/** 
 * Referencable Design Pattern.
 * @author David Forstenlechner
 * @date   2002-2003
 */

#ifndef REFERENCABLE_INCLUDED
#define REFERENCABLE_INCLUDED

class Referencable 
{
public:
  /** 
   * Standard Constructor.
   * Initializes ReferenceCounter to 1
   */
	Referencable();

  /**  
   * Increments reference counter by 1.
   * @return Value of reference counter after incrementation
   */
	virtual int AddRef();

  /**  
   * Decrements reference counter by 1.
   * @return Value of reference counter after decrementation
   */
	virtual int ReleaseRef();

  /**  
   * Returns reference count.
   * @return Value of reference counter
   */
	int GetReferenceCount();

protected:
   /** Counting references. */
	int ReferenceCounter;
};

#endif //REFERENCABLE_INCLUDED
