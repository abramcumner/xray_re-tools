#pragma once

#include "Vector3.h"

namespace xrFSL
{
	template<typename T> struct SAABB 
	{
		SAABB<T>& Null();
		SAABB<T>& Set(const SAABB<T>& box);
		SAABB<T>& Invalidate();
 		SAABB<T>& Extend(const SVector3<T>& p);
// 		SAABB<T>& Grow(T s);
// 		SAABB<T>& Grow(const SVector3<T>& s);
// 		SAABB<T>& Shrink(T s);
// 		SAABB<T>& Shrink(const SVector3<T>& s);
		SAABB<T>& Fix();
		T Volume() const;
		T Size() const;
		T Width() const;
		T Height() const;
		T Depth() const;
		void Center(SVector3<T>& v) const;
		T OverlapRatio(const SAABB<T>& box) const;
		bool Contain(const SVector3<T>& p) const;
		bool Contain(const SAABB<T>& box) const;
		bool Intersect(const SAABB<T>& box) const;
		bool Intersect_zy(const SAABB<T>& box) const;
		SAABB<T>& Merge(const SAABB<T>& box);

		union 
		{
			struct 
			{
				SVector3<T>	min;
				SVector3<T>	max;
			};
			struct 
			{
				T		x1, y1, z1;
				T		x2, y2, z2;
			};
		};
	};

	// AABB typedefs
	typedef SAABB<float> fBox;
	typedef SAABB<int16> i16Box;

	//================================================
	// inline impl
	//================================================
	template<typename T> inline SAABB<T>& SAABB<T>::Null()
	{
		min.Set();
		max.Set();
		return *this;
	}

	template<typename T> inline SAABB<T>& SAABB<T>::Set(const SAABB<T>& box)
	{
		min.Set(box.min);
		max.Set(box.max);
		return *this;
	}

 	template<typename T> inline SAABB<T>& SAABB<T>::Invalidate()
 	{
		x1 = y1 = z1 = FLT_MAX;
		x2 = y2 = z2 = -FLT_MAX;
 		return *this;
 	}

 	template<typename T> inline SAABB<T>& SAABB<T>::Extend(const SVector3<T>& p)
 	{
 		min.Min(p);
 		max.Max(p);
 		return *this;
 	}
 
// 	template<typename T> inline SAABB<T>& SAABB<T>::Grow(const SVector3<T>& s)
// 	{
// 		min.Sub(s);
// 		max.Add(s);
// 		return *this;
// 	}
// 
// 	template<typename T> inline SAABB<T>& SAABB<T>::Grow(T s)
// 	{
// 		min.Sub(s);
// 		max.Add(s);
// 		return *this;
// 	}
// 
// 	template<typename T> inline SAABB<T>& SAABB<T>::Shrink(const SVector3<T>& s)
// 	{
// 		min.Sub(s);
// 		max.Add(s);
// 		return Fix();
// 	}
// 
// 	template<typename T> inline SAABB<T>& SAABB<T>::Shrink(T s)
// 	{
// 		min.Add(s);
// 		max.Sub(s);
// 		return Fix();
// 	}

	template<typename T> inline SAABB<T>& SAABB<T>::Fix()
	{
		if (x2 < x1)
			x1 = x2 = T(0.5)*(x1 + x2);
		if (y2 < y1)
			y1 = y2 = T(0.5)*(y1 + y2);
		if (z2 < z1)
			z1 = z2 = T(0.5)*(z1 + z2);
		return *this;
	}

	template<typename T> inline bool SAABB<T>::Contain(const SVector3<T>& p) const
	{
		return (p.x >= min.x && p.x <= max.x) &&
			(p.y >= min.y && p.y <= max.y) &&
			(p.z >= min.z && p.z <= max.z);
	}

	template<typename T> inline bool SAABB<T>::Contain(const SAABB<T>& box) const
	{
		return (x1 <= box.x1 && box.x2 <= x2) &&
			(y1 <= box.y1 && box.y2 <= y2) &&
			(z1 <= box.z1 && box.z2 <= z2);
	}

	template<typename T> inline bool SAABB<T>::Intersect(const SAABB<T>& box) const
	{
		if (x1 > box.x2 || x2 < box.x1)
			return false;
		if (y1 > box.y2 || y2 < box.y1)
			return false;
		if (z1 > box.z2 || z2 < box.z1)
			return false;
		return true;
	}

	template<typename T> inline bool SAABB<T>::Intersect_zy(const SAABB<T>& box) const
	{
		if (z1 > box.z2 || z2 < box.z1)
			return false;
		if (y1 > box.y2 || y2 < box.y1)
			return false;
		return true;
	}

	template<typename T> inline T SAABB<T>::Volume() const
	{
		return (x2 - x1)*(y2 - y1)*(z2 - z1);
	}

	template<typename T> inline T SAABB<T>::Size() const
	{
		return (x2 - x1) + (y2 - y1) + (z2 - z1);
	}

	template<typename T> inline T SAABB<T>::Width() const { return x2 - x1; }
	template<typename T> inline T SAABB<T>::Height() const { return y2 - y1; }
	template<typename T> inline T SAABB<T>::Depth() const { return z2 - z1; }

	template<typename T> inline void SAABB<T>::Center(SVector3<T>& p) const
	{
		p.Add(max, min).Mul(T(0.5));
	}

	template<typename T> inline T SAABB<T>::OverlapRatio(const SAABB<T>& box) const
	{
		T w = (x2 < box.x2) ? x2 : box.x2;
		w -= (x1 < box.x1) ? box.x1 : x1;
		if (w <= 0)
			return 0;
		T h = (y2 < box.z2) ? y2 : box.y2;
		h -= (y1 < box.y1) ? box.y1 : y1;
		if (h <= 0)
			return 0;
		T d = (z2 < box.z2) ? z2 : box.z2;
		d -= (z1 < box.z1) ? box.z1 : z1;
		if (d <= 0)
			return 0;
		return (w*h*d)/box.volume();
	}

	template<typename T> inline SAABB<T>& SAABB<T>::Merge(const SAABB<T>& box)
	{
		if (x1 > box.x1)
			x1 = box.x1;
		if (y1 > box.y1)
			y1 = box.y1;
		if (z1 > box.z1)
			z1 = box.z1;
		if (x2 < box.x2)
			x2 = box.x2;
		if (y2 < box.y2)
			y2 = box.y2;
		if (z2 < box.z2)
			z2 = box.z2;
		return *this;
	}
}