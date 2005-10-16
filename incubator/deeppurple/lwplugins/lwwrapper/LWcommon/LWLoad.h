#ifndef LWCOMMON_LWLOAD_H_HEADER_INCLUDED_C1348659
#define LWCOMMON_LWLOAD_H_HEADER_INCLUDED_C1348659

class LWLoad
{
 public:
   LWLoad(const LWLoadState *toLoad);

   bool Load(bool &toLoad);
   bool Load(string & toLoad);
   bool Load(Real * toLoad, int num);
   bool Load(Vector3 & toLoad);
   bool Load(int &toLoad);









 private:
   LWLoadState *FileHandle;

};



#endif /* LWCOMMON_LWLOAD_H_HEADER_INCLUDED_C1348659 */
