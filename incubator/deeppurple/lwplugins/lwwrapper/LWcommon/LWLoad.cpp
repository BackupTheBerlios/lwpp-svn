#include "StdAfx.h"
#include "LWLoad.h"

LWLoad::LWLoad(const LWLoadState *toLoad)
{
   FileHandle=const_cast<LWLoadState *>(toLoad);
}

bool LWLoad::Load(bool &toLoad)
{
   long LoadIt=toLoad;
   if(this->FileHandle->readI4(this->FileHandle->readData,&LoadIt,1))
      return true;
   else
      return false;
}

bool LWLoad::Load(string & toLoad)
{
   char sBuffer[1000];
   this->FileHandle->readStr(this->FileHandle->readData,sBuffer,1000);
   toLoad=sBuffer;
   return true;
}

bool LWLoad::Load(Real * toLoad, int num)
{
   if (num<=0)
      return false;

   float *LoadIt=new float[num];
   if (this->FileHandle->readFP(this->FileHandle->readData,LoadIt,num)!=num)
      return false;

   for (int i=0;i<num;i++)
   {
      toLoad[i]=Real(LoadIt[i]);
   }
   delete [] LoadIt;

   return true;
}

bool LWLoad::Load(Vector3 & toLoad)
{
   float LoadIt[3];
   if(this->FileHandle->readFP(this->FileHandle->readData,LoadIt,3)==3)
   {
      toLoad.x=Real(LoadIt[0]);
      toLoad.y=Real(LoadIt[1]);
      toLoad.z=Real(LoadIt[2]);
      return true;
   }
   else
      return false;
}

bool LWLoad::Load(int &toLoad)
{
   long LoadIt;
   this->FileHandle->readI4(this->FileHandle->readData,&LoadIt,1);
   toLoad=LoadIt;
   return true;
}

