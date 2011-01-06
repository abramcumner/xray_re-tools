/**********************************************************************
 *<
   FILE: bitarray.h

   DESCRIPTION:

   CREATED BY: Dan Silva

   HISTORY:

 *>   Copyright (c) 1994, All Rights Reserved.
 **********************************************************************/

#ifndef BITARRAY__H
#define BITARRAY__H

#include "maxheap.h"
#include <windef.h>
#include <limits.h>

#include "ioapi.h"

class BitArrayCallback: public MaxHeapOperators
{
   public:
   virtual void proc(int n)=0;
};

// Direction indicators for BitArray::Rotate and BitArray::Shift
enum
{
   LEFT_BITSHIFT  = 0,
   RIGHT_BITSHIFT = 1,
};

// Generate statistics on bitarrays created.  Requires a complete rebuild to toggle
// on and off.
// #define DL_BITARRAY_STATS

class BitArray: public MaxHeapOperators {
   enum
   {
      kMAX_LOCALBITS = CHAR_BIT * sizeof(DWORD_PTR),
   };

   union
   {
      // numBits cannot be put into a DWORD_PTR so memory had to be allocated.
      DWORD_PTR*  bits;

      // bits fit into a DWORD_PTR object; no memory allocation needed.
      DWORD_PTR   localBits;
   };

   long numBits;

   public:
      class NumberSetProxy
      : public MaxHeapOperators {
         public:
            inline operator bool() const           // if( array.NumberSet() )
            {
               return !mArray.IsEmpty();
            }

            inline bool operator !() const         // if( !array.NumberSet() )
            {
               return mArray.IsEmpty();
            }

            inline operator int() const            // int n = array.NumberSet();
            {
               return mArray.NumberSetImpl();
            }

            inline operator DWORD_PTR() const
            {
               return mArray.NumberSetImpl();
            }

            inline operator float() const
            {
               return mArray.NumberSetImpl();
            }

            #ifdef WIN64
            inline operator DWORD() const
            {
               return mArray.NumberSetImpl();
            }
            #endif

            inline bool operator <(int n) const    // if( array.NumberSet() < 3 )
            {
               // if( NumberSet() < 0 ) or a negative, always returns false.
               // if( NumberSet() < 1 ), basically mean "IsEmpty()".
               // if( NumberSet() < n ), we use !(NumberSet() >= n)
               return (n <= 0) ? false : ((n == 1) ? mArray.IsEmpty() : !mArray.NumberSetAtLeastImpl(n));
            }

            inline bool operator <=(int n) const   // if( array.NumberSet() <= 3 )
            {
               // if( x <= n ) ==> if( !(x >= (n+1)) )
               return !mArray.NumberSetAtLeastImpl(n+1);
            }

            inline bool operator >(int n) const    // if( array.NumberSet() > 3 )
            {
               // if( x > 0 ) ==> !IsEmpty()
               // if( x > n ) ==> if( x >= (n+1) )
               return n ? mArray.NumberSetAtLeastImpl(n+1) : !mArray.IsEmpty();
            }

            inline bool operator >=(int n) const   // if( array.NumberSet() >= 3 )
            {
               return mArray.NumberSetAtLeastImpl(n);
            }

            inline bool operator ==(int n) const   // if( array.NumberSet() == 3 )
            {
               return mArray.NumberSetEqualImpl(n);
            }

            inline bool operator !=(int n) const   // if( array.NumberSet() != 3 )
            {
               return !mArray.NumberSetEqualImpl(n);
            }

            inline int operator +(int n) const         // int n = array.NumberSet() + 3;
            {
               return mArray.NumberSetImpl() + n;
            }

            inline int operator -(int n) const         // int n = array.NumberSet() + 3;
            {
               return mArray.NumberSetImpl() - n;
            }

            inline int operator *(int n) const         // int n = array.NumberSet() * 3;
            {
               return mArray.NumberSetImpl() * n;
            }

            inline int operator /(int n) const         // int n = array.NumberSet() / 3;
            {
               return mArray.NumberSetImpl() / n;
            }

            inline int operator %(int n) const         // int n = array.NumberSet() % 3;
            {
               return mArray.NumberSetImpl() % n;
            }

            inline int operator +(const NumberSetProxy& proxy) const
            {
               return mArray.NumberSetImpl() + int(proxy);
            }

            inline int operator -(const NumberSetProxy& proxy) const
            {
               return mArray.NumberSetImpl() - int(proxy);
            }

            inline int operator *(const NumberSetProxy& proxy) const
            {
               return mArray.NumberSetImpl() * int(proxy);
            }

         private:
            const BitArray& mArray;

            friend class BitArray;
            // Can only be created by the BitArray itself.
            inline NumberSetProxy(const BitArray& a) : mArray(a) {}
      };

      friend class NumberSetProxy;

   public:
      inline BitArray() { bits = NULL; numBits = 0; BitArrayAllocated(); }
      inline BitArray(int n)
      {
         if( UseLocalBits(n) )
         {
            numBits     = n;
            localBits   = 0;

            BitArrayAllocated();
         }
         else
         {
            CreateBitArrayImpl(n);
         }
      }
      inline BitArray(const BitArray& b)
      {
         if( b.UseLocalBits() )
         {
            localBits   = b.localBits;
            numBits     = b.numBits;

            BitArrayAllocated();
         }
         else
         {
            SetBitsFromImpl(b);
         }
      }

      inline ~BitArray()
         { if( !UseLocalBits() ) FreeBitsImpl(); else BitArrayDeallocated(); }

      DllExport void SetSize(int n, int save=0);  // save=1:preserve old bit values

      inline int  GetSize() const { return numBits; }
      inline void ClearAll() { UseLocalBits() ? localBits = 0 : ClearAllImpl(); }
      inline void SetAll() // Only set used bits; leave the others at zero.
         { UseLocalBits() ? localBits = BitMask(numBits) - 1 : SetAllImpl(); }

      inline void Set(int i)
      {
         DbgAssert(i>-1&&i<numBits);
         UseLocalBits() ? localBits |= BitMask(i) : SetImpl(i);
      }

      inline void Clear(int i)
      {
         DbgAssert(i>-1&&i<numBits);

         UseLocalBits() ? localBits &= ~BitMask(i) : ClearImpl(i);
      }

      inline void Set(int i, int b) { b ? Set(i) : Clear(i); }
      inline int operator[](int i) const
      {
         DbgAssert (i>-1);
         DbgAssert (i<numBits);

         return UseLocalBits() ? (localBits & BitMask(i) ? 1 : 0) : GetNthBitImpl(i);
      }

      inline bool IsEmpty() const   { return UseLocalBits() ? !localBits : IsEmptyImpl(); }
      inline bool AnyBitSet() const { return !IsEmpty(); }

      // how many bits are 1's; use IsEmpty() for faster checks
      // Returns a proxy object which can optimize client code depending on the type
      // of access required (ie: != 0 would call IsEmpty(), etc)
      inline NumberSetProxy NumberSet() const
      {
         return NumberSetProxy(*this);
      }

      DllExport void Compress();
      DllExport void Expand();
      DllExport void Reverse(BOOL keepZero = FALSE);  // keepZero=TRUE keeps zero bit where it is
      DllExport void Rotate(int direction, int count);            // With wraparound
      DllExport void Shift(int direction, int count, int where=0);   // Without wraparound
      DllExport void EnumSet(BitArrayCallback &cb);  // enumerates elements that are 1's
      DllExport void DeleteSet (BitArray & dset, int mult=1);
      DllExport IOResult Save(ISave* isave);
      DllExport IOResult Load(ILoad* iload);

      // Comparison operator
      inline bool operator==(const BitArray& b) const
      {
         return (numBits == b.numBits) &&
                (UseLocalBits() ? (localBits == b.localBits) : CompareBitsImpl(b));
      }

      // Assignment operators
      DllExport BitArray& operator=(const BitArray& b);

      // Assignment operators: These require arrays of the same size!
      DllExport BitArray& operator&=(const BitArray& b);  // AND=
      DllExport BitArray& operator|=(const BitArray& b);  // OR=
      DllExport BitArray& operator^=(const BitArray& b);  // XOR=

      // Binary operators: These require arrays of the same size!
      DllExport BitArray operator&(const BitArray&) const; // AND
      DllExport BitArray operator|(const BitArray&) const; // OR
      DllExport BitArray operator^(const BitArray&) const; // XOR

      // Unary operators
      inline BitArray operator~() const
      {
         return UseLocalBits() ? BitArray(~localBits, numBits, true) : OperatorNotImpl();
      }

   private:
      inline BitArray(DWORD_PTR localBits_, long numBits_, bool zeroHighBits = false) :
         localBits(localBits_), numBits(numBits_)
      {
         DbgAssert( UseLocalBits() );

         if( zeroHighBits )
             ZeroUnusedBitsImpl();

         BitArrayAllocated();
      }

      inline bool UseLocalBits() const { return numBits <= kMAX_LOCALBITS; }
      inline bool UseLocalBits(int n) const { return n <= kMAX_LOCALBITS; }
      inline DWORD_PTR BitMask(int i) const
         // NOTE: Shifting by kMAX_LOCALBITS will give an undefined behavior; the
         // chip actually limits the shift from 0 to kMAX_LOCALBITS-1, so most likely
         // you simply return '1' when what you wanted was zero.
         { return (i < kMAX_LOCALBITS) ? (DWORD_PTR(1) << i) : DWORD_PTR(0); }

      // Used internally to treat the bit array the same way whether it's new'ed or
      // simply local.
      inline const DWORD_PTR* GetBitPtr() const
      {
         return UseLocalBits() ? &localBits : bits;
      }

      inline DWORD_PTR* GetBitPtr()
      {
         return UseLocalBits() ? &localBits : bits;
      }

      // Called from the ctor only; initializes an array filled with zeroes.
      DllExport void CreateBitArrayImpl(int n);
      // Called from the ctor only; initializes from an array of bits.
      DllExport void SetBitsFromImpl(const BitArray&);

      DllExport void ClearAllImpl();
      DllExport void SetAllImpl();

      DllExport void SetImpl(int i);
      DllExport void ClearImpl(int i);

      DllExport void SetImpl(int i, int b);
      DllExport int  GetNthBitImpl(int i) const;
      DllExport int  NumberSetImpl() const;
      DllExport bool IsEmptyImpl() const;

      DllExport BitArray OperatorNotImpl() const;
      DllExport bool CompareBitsImpl(const BitArray&) const;

      DllExport void FreeBitsImpl();

      #ifndef DL_BITARRAY_STATS
         inline void BitArrayAllocated() {}
         inline void BitArrayDeallocated() {}
      #else
         class BitArrayStats;
         friend class BitArrayStats;

         DllExport void BitArrayAllocated();
         DllExport void BitArrayDeallocated();
      #endif

      DllExport bool NumberSetImpl(int n) const;         // Exhaustive count; can be dead slow
      DllExport bool NumberSetEqualImpl(int n) const;    // Stops as soon as count will be higher
      DllExport bool NumberSetAtLeastImpl(int n) const;  // Stops as soon as count reaches limit.

      // Zeroes out bits over numBits so we can always use fast comparisons (memcmp,
      // ==, etc) without having to mask out the last chunk.
      DllExport void ZeroUnusedBitsImpl();
   };

// Help the compiler out when the array.NumberSet is on the right-hand of the equation
template <typename T>
inline T operator +(T n, const BitArray::NumberSetProxy& proxy)
{
   return n + proxy.operator int();
}

template <typename T>
inline T operator -(T n, const BitArray::NumberSetProxy& proxy)
{
   return n - proxy.operator int();
}

template <typename T>
inline T operator *(T n, const BitArray::NumberSetProxy& proxy)
{
   return proxy.operator *(n);
}

template <typename T>
inline T operator /(T n, const BitArray::NumberSetProxy& proxy)
{
   return n / proxy.operator int();
}

template <typename T>
inline T operator %(T n, const BitArray::NumberSetProxy& proxy)
{
   return n % proxy.operator int();
}

template <typename T>
inline bool operator <=(T n, const BitArray::NumberSetProxy& proxy)
{
    return proxy.operator >=(n);
}

template <typename T>
inline bool operator <(T n, const BitArray::NumberSetProxy& proxy)
{
   return proxy.operator >(n);
}

template <typename T>
inline bool operator >(T n, const BitArray::NumberSetProxy& proxy)
{
   return proxy.operator <(n);
}

template <typename T>
inline bool operator >=(T n, const BitArray::NumberSetProxy& proxy)
{
   return proxy.operator <=(n);
}

template <typename T>
inline bool operator ==(T n, const BitArray::NumberSetProxy& proxy)
{
   return proxy.operator ==(n);
}

template <typename T>
inline bool operator !=(T n, const BitArray::NumberSetProxy& proxy)
{
   return proxy.operator !=(n);
}

template <typename T>
inline void operator +=(T& n, const BitArray::NumberSetProxy& proxy)
{
   n += proxy.operator int();
}

template <typename T>
inline void operator -=(T& n, const BitArray::NumberSetProxy& proxy)
{
   n -= proxy.operator int();
}

#endif
