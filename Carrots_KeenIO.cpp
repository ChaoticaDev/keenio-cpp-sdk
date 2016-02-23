// Carrots_KeenIO.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <map>
#include <string>

using namespace std;
#include "Carrots_KeenIO.h"


// This is an example of an exported variable
CARROTS_KEENIO_API int nCarrots_KeenIO=0;

// This is an example of an exported function.
CARROTS_KEENIO_API int fnCarrots_KeenIO(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see Carrots_KeenIO.h for the class definition
CCarrots_KeenIO::CCarrots_KeenIO()
{
    return;
}
