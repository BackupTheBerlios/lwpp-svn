#ifndef OBSERVER_INCLUDED
#define OBSERVER_INCLUDED

#include <vector> 

using namespace std; 

class Observable;

/** 
 * Observer Design Pattern.
 * @author  David Forstenlechner
 * @date    2002-2003
 * @see     Observable
 */
class Observer  
{
public:
	Observer();
	virtual ~Observer();
  /**
   * An observed class Instance signaled a change.
   * @param theChangedSubject Observable object signalled to have changed
   */
   virtual void Update(Observable* theChangedSubject ) = 0; 
};

/** 
 * Belonging to the Observer Design Pattern.
 * @author  David Forstenlechner
 * @date    2002-2003
 * @see     Observer
 */
class Observable { 
public: 
   /** Standard Constructor. */
   Observable() {}; 
   /** Standard Destructor. */
   ~Observable() {}; 

  /** 
   * Add an Observer to be notified. 
   * @param toAdd Observer instance you would like to get notified.
   */
   virtual void addObserver( Observer* toAdd ); 
  /** 
   * Remove Observer from notification list.
   * @param toDelete Observer instance to be deleted from the notification list
   */
   virtual void deleteObserver( Observer* toDelete ); 
   /** Notify Observers on notification list*/
   virtual void notifyObservers(); 
   int numObservers();


private: 
   /** Observers to be notified. */
   vector<Observer*> _observers; 
}; 

#endif // OBSERVER_INCLUDED