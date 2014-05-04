// Simple smart pointer class.

#ifndef smart_h
#define smart_h

typedef unsigned long int ulint;

template <class T>
class smartPtrBase
{
 protected:  // -------------------------------------------------------------
    
    T* bufBegin;
    T* bufEnd;
    T* pBufCurrent;
    ulint bufLen;
    bool status;
    bool doFree;
    T dummy;

 public:  // ----------------------------------------------------------------
    
    // --- constructors ---
    
    smartPtrBase(T* buffer, ulint bufferLen, bool bufOwner = false) : dummy(0)
    {
        doFree = bufOwner;
        if ( bufferLen >= 1 )
        {
            this->pBufCurrent = ( this->bufBegin = buffer );
            this->bufEnd = this->bufBegin + bufferLen;
            this->bufLen = bufferLen;
            this->status = true;
        }
        else
        {
            this->pBufCurrent = ( this->bufBegin = ( this->bufEnd = 0 ));
            this->bufLen = 0;
            this->status = false;
        }
    }
    
    // --- destructor ---
    
    virtual ~smartPtrBase()
    {
        if ( doFree && (this->bufBegin != 0) )
        {
            delete[] this->bufBegin;
        }
    }
    
    // --- public member functions ---
    
    virtual T* tellBegin()  { return this->bufBegin; }
    virtual ulint tellLength()  { return this->bufLen; }
    virtual ulint tellPos()  { return (ulint)(this->pBufCurrent-bufBegin); }

    virtual bool checkIndex(ulint index)
    {
        return ((this->pBufCurrent+index)<this->bufEnd);
    }
    
    virtual bool reset()
    {
        if ( this->bufLen >= 1 )
        {
            this->pBufCurrent = this->bufBegin;
            return (this->status = true);
        }
        else
        {
            return (this->status = false);
        }
    }

    virtual bool good()
    {
        return (this->pBufCurrent<this->bufEnd);
    }
    
    virtual bool fail()  
    {
        return (this->pBufCurrent==this->bufEnd);
    }
    
    virtual void operator ++()
    {
        if ( good() )
        {
            this->pBufCurrent++;
        }
        else
        {
            this->status = false;
        }
    }
    
    virtual void operator ++(int)
    {
        if ( good() )
        {
            this->pBufCurrent++;
        }
        else
        {
            this->status = false;
        }
    }
    
    virtual void operator --()
    {
        if ( !fail() )
        {
            this->pBufCurrent--;
        }
        else
        {
            this->status = false;
        }
    }
    
    virtual void operator --(int)
    {
        if ( !fail() )
        {
            this->pBufCurrent--;
        }
        else
        {
            this->status = false;
        }
    }
    
    virtual void operator +=(ulint offset)
    {
        if (checkIndex(offset))
        {
            this->pBufCurrent += offset;
        }
        else
        {
            this->status = false;
        }
    }
    
    virtual void operator -=(ulint offset)
    {
        if ((pBufCurrent-offset) >= this->bufBegin)
        {
            this->pBufCurrent -= offset;
        }
        else
        {
            this->status = false;
        }
    }
    
    T operator*()
    {
        if ( good() )
        {
	  return *(this->pBufCurrent);
        }
        else
        {
            this->status = false;
            return dummy;
        }
    }

    T& operator [](ulint index)
    {
        if (checkIndex(index))
        {
            return this->pBufCurrent[index];
        }
        else
        {
            this->status = false;
            return dummy;
        }
    }

    virtual operator bool()  { return this->status; }
    
};


template <class T>
class smartPtr : public smartPtrBase<T>
{
 public:  // ----------------------------------------------------------------
    
    // --- constructors ---
    
    smartPtr(T* buffer, ulint bufferLen, bool bufOwner = false)
        : smartPtrBase<T>(buffer, bufferLen, bufOwner)
    {
    }
    
    smartPtr()
        : smartPtrBase<T>(0,0)
    {
    }

    void setBuffer(T* buffer, ulint bufferLen)
    {
        if ( bufferLen >= 1 )
        {
            this->pBufCurrent = ( this->bufBegin = buffer );
            this->bufEnd = this->bufBegin + bufferLen;
            this->bufLen = bufferLen;
            this->status = true;
        }
        else
        {
            this->pBufCurrent = this->bufBegin = this->bufEnd = 0;
            this->bufLen = 0;
            this->status = false;
        }
    }
};

#endif  // smart_h
