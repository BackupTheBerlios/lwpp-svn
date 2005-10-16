#ifndef LWCOMMON_LWSAVE_H_HEADER_INCLUDED_C1348DCE
#define LWCOMMON_LWSAVE_H_HEADER_INCLUDED_C1348DCE

class LWSave
{
 public:
   LWSave(const LWSaveState * toSave);
   void Save(bool & toSave);
   void Save(string & toSave);
   void Save(Real *toSave, int num);
   void Save(Vector3 & toSave);
   void Save(int &toSave);

 private:
   LWSaveState *FileHandle;
};



#endif /* LWCOMMON_LWSAVE_H_HEADER_INCLUDED_C1348DCE */
