#ifdef CHANGED
#include "pageprovider.h"

//----------------------------------------------------------------------
// PageProvider
//     Classe pour gérer les pages physiques depuis Init
//----------------------------------------------------------------------

PageProvider::PageProvider()
{
 bitMap = new BitMap(NumPhysPages);
}

PageProvider::~PageProvider()
{
delete bitMap;
}


int PageProvider::getEmptyPage()
{
return bitMap->Find();
}

void PageProvider::releasePage(int page)
{
bitMap -> Clear(page);
}

int PageProvider::numAvailPage()
{
return bitMap->NumClear();
}

#endif // CHANGED

