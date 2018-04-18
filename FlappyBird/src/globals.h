#ifndef _GLOBALS_H__
#define __GLOBALS_H__

#define DELETE(x) if(x != nullptr)	{delete x; x = nullptr;}
#define DELETE_ARRAY(x) if(x != nullptr) {delete[] x; x = nullptr;}
#endif
