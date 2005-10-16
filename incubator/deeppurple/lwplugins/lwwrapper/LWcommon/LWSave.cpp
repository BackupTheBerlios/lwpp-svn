#include "StdAfx.h"
#include "LWSave.h"

LWSave::LWSave(const LWSaveState * toSave) :
FileHandle(NULL)
{
   FileHandle=const_cast<LWSaveState *>(toSave);
}

void LWSave::Save(bool & toSave)
{
   const long SaveIt=toSave;
   this->FileHandle->writeI4(this->FileHandle->writeData,&SaveIt,1);
}

void LWSave::Save(string & toSave)
{
   this->FileHandle->writeStr(this->FileHandle->writeData,toSave.c_str());
}

void LWSave::Save(Real *toSave, int num)
{
   if (num<=0)
      return;

   float *SaveIt=new float[num];
   for (int i=0;i<num;i++)
   {
      SaveIt[i]=static_cast<float>(toSave[i]);
   }
   this->FileHandle->writeFP(this->FileHandle->writeData,SaveIt,num);
   delete [] SaveIt;
}

void LWSave::Save(Vector3 & toSave)
{
   float SaveIt[3];
   SaveIt[0]=static_cast<float>(toSave.x);
   SaveIt[1]=static_cast<float>(toSave.y);
   SaveIt[2]=static_cast<float>(toSave.z);
   this->FileHandle->writeFP(this->FileHandle->writeData,SaveIt,3);
}

void LWSave::Save(int &toSave)
{
   long SaveIt=toSave;
   this->FileHandle->writeI4(this->FileHandle->writeData, &SaveIt, 1);
}

