#include "StdAfx.h"
#include "FileLib.h"

namespace FileLib {
    bool FileExists(std::string ToCheck)
    {
        if ( ToCheck == "" )
            return false;
        std::ifstream FOpener;
        FOpener.open(ToCheck.c_str());
        if (FOpener.is_open())
        {
            FOpener.close();
            return true;
        }
        else
            return false;
    }

}