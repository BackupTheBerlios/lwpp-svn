// Autolist.h: interface for the Autolist class.
//
////////////////////////////////////////////////////

#ifndef AUTOLIST_INCLUDED
#define AUTOLIST_INCLUDED

#include <list>

/**
 * Autolist Design Pattern.
 * Modified from Game Programming Gems 3
 */
template <class T>
class Autolist
{
public:
   /** Constructor adds this pointer to the static Instances list*/
   Autolist()
   { 
      ms_List.push_back( static_cast<T>(this) );
   }

   /** Destructor removes this pointer from the static Instances list.*/
   virtual ~Autolist()
   { 
      ms_List.remove( static_cast<T>(this) ); 
   }

   typedef list<T> AutoList;
   typedef typename list<T>::iterator AutoIterator;

   /**  @todo remove this */
   static AutoList & GetList()
   {
      return ms_List;
   }

private:
   /// The Instance list
   static list<T>            ms_List;
};

template<class T> list<T>    Autolist<T>::ms_List;


#endif // AUTOLIST_INCLUDED



