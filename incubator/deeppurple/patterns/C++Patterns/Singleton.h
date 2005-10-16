#ifndef SINGLETON_INCLUDED
#define SINGLETON_INCLUDED

#include <cassert>

/** 
 * Modified from Game Programming Gems 1
 * Portions Copyright (C) Scott Bilas, 2000
 */
template <class T>
class Singleton
{
 public:
   static T& GetSingleton( void )
   {
      if ( ms_Singleton )
         return ( *ms_Singleton );  
      else
      {
         ms_Singleton=new T;
         return ( *ms_Singleton );
      }
   }

   static T* GetSingletonPtr( void )
   {
      if ( ms_Singleton )
         return ms_Singleton;  
      else
      {
         ms_Singleton=new T;
         return ms_Singleton;
      }
   }
   static void DestroySingleton()
   {
      if (ms_Singleton)
      {
         delete ms_Singleton;
         ms_Singleton=0;
      }
   }


 protected:
    Singleton( void )
    {
//        assert( !ms_Singleton );
//        int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
//        ms_Singleton = (T*)((int)this + offset);
    }
   ~Singleton( void )
        {  assert( ms_Singleton );  ms_Singleton = 0;  }

private:
   /**
    * @bug declare extern when compiled with exe!! Duplicated static members in dlls
    */
    static T* ms_Singleton;
};

template <class T> T* Singleton <T>::ms_Singleton = 0;  

#endif // SINGLETON_INCLUDEDtemplate <typename T>


