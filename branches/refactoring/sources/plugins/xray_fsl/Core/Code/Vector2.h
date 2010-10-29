#pragma once

namespace xrFSL
{
	template<typename T> struct SVector2 
	{
		template<typename F> SVector2<T>& Set(const SVector2<F>& a);
		SVector2<T>& Set(T _x = T(), T _y = T());
		SVector2<T>& Set(const T _xy[2]);

		void Get(T _xy[2]) const;
		T Magnitude() const;
		T Distance(const SVector2<T>& a) const;
		bool Similar(const SVector2<T>& a, T e = T(1e-6)) const;
		SVector2<T>& Min(const SVector2<T>& a);
		SVector2<T>& Max(const SVector2<T>& a);
		SVector2<T>& Add(const SVector2<T>& a);
		SVector2<T>& Sub(const SVector2<T>& a, const SVector2<T>& b);
		SVector2<T>& Mul(T s);
		int Compare(const SVector2<T>& right) const;

		bool operator<(const SVector2<T>& right) const;
		bool operator==(const SVector2<T>& right) const;
		bool operator!=(const SVector2<T>& right) const;
		T& operator[](size_t i);
		const T& operator[](size_t i) const;

		union 
		{
			struct 
			{
				T x, y;
			};
			struct 
			{
				T u, v;
			};
			T xy[2];
		};
	};

	// Vectors typedef
	typedef SVector2<float> fVector2;
	typedef SVector2<double> dVector2;
	typedef SVector2<int32> i32Vector2;

	//==================================================
	// Inline impl
	//==================================================
	template<typename T> template<typename F> inline SVector2<T>& SVector2<T>::Set(const SVector2<F>& a)
	{
		x = T(a.x);
		y = T(a.y);
		return *this;
	}

	template<typename T> inline SVector2<T>& SVector2<T>::Set(T _x, T _y) { x = _x; y = _y; return *this; }

	template<typename T> inline SVector2<T>& SVector2<T>::Set(const T _xy[2])
	{
		x = _xy[0];
		y = _xy[1];
		return *this;
	}

	template<typename T> inline void SVector2<T>::Get(T _xy[2]) const
	{
		_xy[0] = x;
		_xy[1] = y;
	}

	template<typename T> inline T SVector2<T>::Magnitude() const { return std::sqrt(x*x + y*y); }

	template<typename T> inline T SVector2<T>::Distance(const SVector2<T>& a) const
	{
		T dx = a.x - x, dy = a.y - y;
		return std::sqrt(dx*dx + dy*dy);
	}

	template<typename T> bool SVector2<T>::Similar(const SVector2<T>& a, T e) const
	{
		if (e <= std::abs(a.x - x))
			return false;
		if (e <= std::abs(a.y - y))
			return false;
		return true;
	}

	template<typename T> inline SVector2<T>& SVector2<T>::Min(const SVector2<T>& a)
	{
		if (x >= a.x)
			x = a.x;
		if (y >= a.y)
			y = a.y;
		return *this;
	}

	template<typename T> inline SVector2<T>& SVector2<T>::Max(const SVector2<T>& a)
	{
		if (x < a.x)
			x = a.x;
		if (y < a.y)
			y = a.y;
		return *this;
	}

	template<typename T> inline SVector2<T>& SVector2<T>::Add(const SVector2<T>& a)
	{
		x += a.x;
		y += a.y;
		return *this;
	}

	template<typename T> inline SVector2<T>& SVector2<T>::Sub(const SVector2<T>& a, const SVector2<T>& b)
	{
		x = a.x - b.x;
		y = a.y - b.y;
		return *this;
	}

	template<typename T> inline SVector2<T>& SVector2<T>::Mul(T s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	template<typename T> inline bool SVector2<T>::operator<(const SVector2<T>& right) const
	{
		return x < right.x || (x == right.x && y < right.y);
	}

	template<typename T> inline bool SVector2<T>::operator==(const SVector2<T>& right) const
	{
		return (x == right.x && y == right.y);
	}

	template<typename T> inline bool SVector2<T>::operator!=(const SVector2<T>& right) const
	{
		return (x != right.x || y != right.y);
	}

	template<typename T> inline T& SVector2<T>::operator[](size_t i) { return xy[i]; }

	template<typename T> inline const T& SVector2<T>::operator[](size_t i) const { return xy[i]; }

	template<typename T> inline int SVector2<T>::Compare(const SVector2<T>& right) const
	{
#if 1
		return (x < right.x) ? -1 : (x == right.x ? (y < right.y ? -1 : (y == right.y ? 0 : +1)) : +1);
#else
		if (x < right.x)
			return -1;
		else if (x > right.x)
			return +1;
		if (y < right.y)
			return -1;
		else if (y > right.y)
			return +1;
		return 0;
#endif
	}
}