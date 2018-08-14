
#include "Files.h"
namespace Tomography {
/* Null, because instance will be initialized on demand. */
//Files* Files::fInstance = 0;
Files *Files::fInstance = 0;

Files* Files::instance()
{
    //if (fInstance == 0)
	if (!fInstance)
        fInstance = new Files;//();

    return fInstance;
}

} //end of tomography namespace
