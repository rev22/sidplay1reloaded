//
// /home/ms/sources/sidplay/libsidplay/include/RCS/smart.h,v
//

#ifndef SMART_H
#define SMART_H


#include <iostream.h>
#include <iomanip.h>
#include <ctype.h>
#include "mytypes.h"


template <class T>
class smartPtrBase
{
 public:  // ----------------------------------------------------------------
	
	// --- constructors ---
	
	smartPtrBase(T* pBuffer, udword bufferLen, bool bufOwner = false) : dummy(0)
	{
		doFree = bufOwner;
		if ( bufferLen >= 1 )
		{
			pBufCurrent = ( pBufBegin = pBuffer );
			pBufEnd = pBufBegin + bufferLen;
			bufLen = bufferLen;
			status = true;
		}
		else
		{
			pBufCurrent = ( pBufBegin = ( pBufEnd = 0 ));
			bufLen = 0;
			status = false;
		}
	}
	
	// --- destructor ---
	
	virtual ~smartPtrBase()
	{
		if ( doFree && (pBufBegin != 0) )
		{
			delete[] pBufBegin;
		}
	}
	
	// --- public member functions ---
	
	virtual T* tellBegin()  { return pBufBegin;	}
	virtual udword tellLength()  { return bufLen; }
	virtual udword tellPos()  { return (udword)(pBufCurrent-pBufBegin); }
  
	virtual bool reset()
	{
		if ( bufLen >= 1 )
		{
			pBufCurrent = pBufBegin;
			return (status = true);
		}
		else
		{
			return (status = false);
		}
	}

	virtual bool good()
	{
		if ( pBufCurrent < pBufEnd )
			return true;
		else
			return false;
	}
	
	virtual bool fail()  
	{
		if ( pBufCurrent == pBufEnd )
			return true;
		else
			return false;
	}
	
	virtual void operator ++()
	{
		if ( status && !fail() )
		{
			pBufCurrent++;
		}
		else
		{
			status = false;
		}
	}
	
	virtual void operator ++(int)
	{
		if ( status && !fail() )
		{
			pBufCurrent++;
		}
		else
		{
			status = false;
		}
	}
	
	virtual void operator --()
	{
		if ( status && (pBufCurrent > pBufBegin) )
		{
			pBufCurrent--;
		}
		else
		{
			status = false;
		}
	}
	
	virtual void operator --(int)
	{
		if ( status && (pBufCurrent > pBufBegin) )
		{
			pBufCurrent--;
		}
		else
		{
			status = false;
		}
	}
	
	virtual void operator +=(udword offset)
	{
		if ( status && ((pBufCurrent + offset) < pBufEnd) )
		{
			pBufCurrent += offset;
		}
		else
		{
			status = false;
		}
	}
	
	virtual void operator -=(udword offset)
	{
		if ( status && ((pBufCurrent - offset) >= pBufBegin) )
		{
			pBufCurrent -= offset;
		}
		else
		{
			status = false;
		}
	}
	
	T operator*()
	{
		if ( status && good() )
		{
			return *pBufCurrent;
		}
		else
		{
			status = false;
			return dummy;
		}
	}

	T& operator [](udword index)
	{
		if ( status && ((pBufCurrent + index) < pBufEnd) )
		{
			return pBufCurrent[index];
		}
		else
		{
			status = false;
			return dummy;
		}
        }

	virtual operator bool()  { return status; }
	
 protected:  // -------------------------------------------------------------
	
	T* pBufBegin;
	T* pBufEnd;
	T* pBufCurrent;
	udword bufLen;
	bool status;
	bool doFree;
	T dummy;
};


template <class T>
class smartPtr : public smartPtrBase<T>
{
 public:  // ----------------------------------------------------------------
	
	// --- constructors ---
	
	smartPtr(T* pBuffer, udword bufferLen, bool bufOwner = false) : smartPtrBase<T>(pBuffer, bufferLen, bufOwner)
	{
	}
	
};


#endif
