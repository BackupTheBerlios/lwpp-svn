// Observer.cpp: implementation of the Observer class.
//
//////////////////////////////////////////////////////////////////////

#include "Observer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Observer::Observer()
{

}

Observer::~Observer()
{

}

void Observable::addObserver (Observer* o) { 
 _observers.push_back(o); 
} 

void Observable::deleteObserver (Observer* o) { 
    int count = _observers.size(); 
 int i; 

 for (i = 0; i < count; i++) 
 { 
   if(_observers[i] == o) 
   break; 
 } 
 if(i < count) 
  _observers.erase(_observers.begin() + i); 

} 

void Observable::notifyObservers () 
{ 
    int count = _observers.size(); 

    for (int i = 0; i < count; i++) 
        (_observers[i])->Update(this); 
}



int Observable::numObservers()
{
   return this->_observers.size();
}

