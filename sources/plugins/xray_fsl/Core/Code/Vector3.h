#pragma once

namespace xrFSL
{
	template<typename T> struct SQuaternion;
	template<typename T> struct SMatrix;

	template<typename T> struct SVector3
	{
		template<typename F> SVector3<T>& Set(const SVector3<F>& v);

		SVector3<T>& Set(T _x = T(), T _y = T(), T _z = T());
		SVector3<T>& Set(const T _xyz[3]);
		void Get(T _xyz[3]) const;
		T SquareMagnitude() const;
		T Magnitude() const;
		T Distance(const SVector3<T>& v) const;
		SVector3<T>& SetMagnitude(T l);
		SVector3<T>& Normalize();
		SVector3<T>& NormalizeSafe();
		T NormalizeMagnitude();
		SVector3<T>& Abs();
		SVector3<T>& Abs(const SVector3<T>& v);
		SVector3<T>& Min(const SVector3<T>& v);
		SVector3<T>& Max(const SVector3<T>& v);
		SVector3<T>& Sub(const SVector3<T>& a, const SVector3<T>& b);
		SVector3<T>& Sub(const SVector3<T>& a);
		SVector3<T>& Sub(const SVector3<T>& a, T s);
		SVector3<T>& Sub(T s);
		SVector3<T>& Add(const SVector3<T>& a);
		SVector3<T>& Add(const SVector3<T>& a, const SVector3<T>& b);
		SVector3<T>& Add(const SVector3<T>& a, T s);
		SVector3<T>& Add(T s);
		SVector3<T>& Mul(T s);
		SVector3<T>& Mul(const SVector3<T>& a, T s);
		SVector3<T>& Mul(const SVector3<T>& a, const SVector3<T>& s);
		SVector3<T>& Mul(const SVector3<T>& s);
		SVector3<T>& Div(T s);
		SVector3<T>& Mad(const SVector3<T>& p, const SVector3<T>& d, T s);
		SVector3<T>& Lerp(const SVector3<T>& a, const SVector3<T>& b, T t);
		SVector3<T>& Invert();
		SVector3<T>& Invert(const SVector3<T>& v);
		SVector3<T>& Rotate(const SQuaternion<T>& q);
		SVector3<T>& Rotate(const SVector3<T>& v, const SQuaternion<T>& q);
		SVector3<T>& Rotate(const SMatrix<T>& m);
		SVector3<T>& Rotate(const SVector3<T>& v, const SMatrix<T>& m);
		SVector3<T>& Transform(const SMatrix<T>& m);
		SVector3<T>& Transform(const SVector3<T>& v, const SMatrix<T>& m);
		SVector3<T>& CrossProduct(const SVector3<T>& v1, const SVector3<T>& v2);
		SVector3<T>& CrossProduct(const SVector3<T>& v);
		SVector3<T>& CalcNormal(const SVector3<T>& p0, const SVector3<T>& p1, const SVector3<T>& p2);
		SVector3<T>& CalcNormal_non_Normalized(const SVector3<T>& p0, const SVector3<T>& p1, const SVector3<T>& p2);
		SVector3<T>& CalcCenter(const SVector3<T>& p0, const SVector3<T>& p1, const SVector3<T>& p2);
		SVector3<T>& Inertion(const SVector3<T>& p, float v);
		SVector3<T>& Average(const SVector3<T>& a);
		SVector3<T>& Average(const SVector3<T>& a, const SVector3<T>& b);
		SVector3<T>& Reflect(const SVector3<T>& dir, const SVector3<T>& norm);
		SVector3<T>& Slide(const SVector3<T>& dir, const SVector3<T>& norm);
		T DotProduct(const SVector3<T>& v) const;
		T Cosine(const SVector3<T>& v) const;
		bool Similar(const SVector3<T>& v, T e = T(1e-6)) const;
		bool Inverted(const SVector3<T>& v, T e = T(1e-6)) const;
		int Compare(const SVector3<T>& right) const;
		bool operator<(const SVector3<T>& right) const;
		bool operator>(const SVector3<T>& right) const;
		bool operator==(const SVector3<T>& right) const;
		bool operator!=(const SVector3<T>& right) const;
		T& operator[](size_t i);
		const T& operator[](size_t i) const;

		//SVector3<T>&	decompress(uint16_t);
		//uint16	compress() const;

		union 
		{
			struct 
			{
				T	x, y, z;
			};
			T		xyz[3];
		};
	};

	// Vectors typedef
	typedef SVector3<double> dVector3;
	typedef SVector3<float> fVector3;
	typedef SVector3<int32> i32Vector3;
	typedef SVector3<int16> i16Vector3;
	typedef SVector3<uint16> u16Vector3;

	//================================================
	// inline impl
	//================================================
	template<typename T> template<typename F> inline SVector3<T>& SVector3<T>::Set(const SVector3<F>& v)
	{
		x = T(v.x);
		y = T(v.y);
		z = T(v.z);
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Set(T _x, T _y, T _z)
	{
		x = _x;
		y = _y;
		z = _z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Set(const T _xyz[3])
	{
		x = _xyz[0];
		y = _xyz[1];
		z = _xyz[2];
		return *this;
	}

	template<typename T> inline void SVector3<T>::Get(T _xyz[3]) const
	{
		_xyz[0] = x;
		_xyz[1] = y;
		_xyz[2] = z;
	}

	template<typename T> inline T SVector3<T>::SquareMagnitude() const { return x*x + y*y + z*z; }

	template<typename T> inline T SVector3<T>::Magnitude() const { return std::sqrt(x*x + y*y + z*z); }

	template<typename T> inline T SVector3<T>::Distance(const SVector3<T>& v) const
	{
		T dx = v.x - x, dy = v.y - y, dz = v.z - z;
		return std::sqrt(dx*dx + dy*dy + dz*dz);
	}

	template<typename T> inline SVector3<T>& SVector3<T>::SetMagnitude(T m) { return Mul(m/Magnitude()); }

	template<typename T> inline SVector3<T>& SVector3<T>::Normalize() { return Div(Magnitude()); }

	template<typename T> inline SVector3<T>& SVector3<T>::NormalizeSafe()
	{
		T m = Magnitude();
		return (m > T(1e-6)) ? Div(m) : Set(0, 0, 0);
	}

	template<typename T> inline T SVector3<T>::NormalizeMagnitude()
	{
		T m = Magnitude();
		Div(m);
		return m;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Abs()
	{
		if (x < 0)
			x = -x;
		if (y < 0)
			y = -y;
		if (z < 0)
			z = -z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Abs(const SVector3<T>& v)
	{
		x = std::Abs(v.x);
		y = std::Abs(v.y);
		z = std::Abs(v.z);
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Min(const SVector3<T>& v)
	{
		if (x >= v.x)
			x = v.x;
		if (y >= v.y)
			y = v.y;
		if (z >= v.z)
			z = v.z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Max(const SVector3<T>& v)
	{
		if (x < v.x)
			x = v.x;
		if (y < v.y)
			y = v.y;
		if (z < v.z)
			z = v.z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Sub(const SVector3<T>& a, const SVector3<T>& b)
	{
		x = a.x - b.x;
		y = a.y - b.y;
		z = a.z - b.z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Sub(const SVector3<T>& a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Sub(const SVector3<T>& a, T s)
	{
		x = a.x - s;
		y = a.y - s;
		z = a.z - s;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Sub(T s)
	{
		x -= s;
		y -= s;
		z -= s;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Add(const SVector3<T>& a, const SVector3<T>& b)
	{
		x = a.x + b.x;
		y = a.y + b.y;
		z = a.z + b.z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Add(const SVector3<T>& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Add(const SVector3<T>& a, T s)
	{
		x = a.x + s;
		y = a.y + s;
		z = a.z + s;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Add(T s)
	{
		x += s;
		y += s;
		z += s;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Mul(T s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Mul(const SVector3<T>& v, T s)
	{
		x = v.x * s;
		y = v.y * s;
		z = v.z * s;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Mul(const SVector3<T>& a, const SVector3<T>& s)
	{
		x = a.x * s.x;
		y = a.y * s.y;
		z = a.z * s.z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Mul(const SVector3<T>& s)
	{
		x *= s.x;
		y *= s.y;
		z *= s.z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Div(T s)
	{
		// no reciprocal for better precision
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Mad(const SVector3<T>& p, const SVector3<T>& d, T s)
	{
		x = p.x + d.x*s;
		y = p.y + d.y*s;
		z = p.z + d.z*s;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Lerp(const SVector3<T>& a, const SVector3<T>& b, T t)
	{
		T w = 1 - t;
		x = a.x*w + b.x*t;
		y = a.y*w + b.y*t;
		z = a.z*w + b.z*t;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Invert()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Invert(const SVector3<T>& v)
	{
		x = -v.x;
		y = -v.y;
		z = -v.z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Rotate(const SQuaternion<T>& q)
	{
		T rw =-q.x*x - q.y*y - q.z*z;
		T rx = q.w*x + q.y*z - q.z*y;
		T ry = q.w*y + q.z*x - q.x*z;
		T rz = q.w*z + q.x*y - q.y*x;
		x = -rw*q.x + rx*q.w - ry*q.z + rz*q.y;
		y = -rw*q.y + ry*q.w - rz*q.x + rx*q.z;
		z = -rw*q.z + rz*q.w - rx*q.y + ry*q.x;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Rotate(const SVector3<T>& v, const SQuaternion<T>& q)
	{
		return Set(v).Rotate(q);
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Rotate(const SVector3<T>& v, const SMatrix<T>& m)
	{
		x = m._11*v.x + m._21*v.y + m._31*v.z;
		y = m._12*v.x + m._22*v.y + m._32*v.z;
		z = m._13*v.x + m._23*v.y + m._33*v.z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Rotate(const SMatrix<T>& m)
	{
		T _x = m._11*x + m._21*y + m._31*z;
		T _y = m._12*x + m._22*y + m._32*z;
		z = m._13*x + m._23*y + m._33*z;
		x = _x;
		y = _y;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Transform(const SVector3<T>& v, const SMatrix<T>& m)
	{
		x = m._41 + m._11*v.x + m._21*v.y + m._31*v.z;
		y = m._42 + m._12*v.x + m._22*v.y + m._32*v.z;
		z = m._43 + m._13*v.x + m._23*v.y + m._33*v.z;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Transform(const SMatrix<T>& m)
	{
		T _x = m._41 + m._11*x + m._21*y + m._31*z;
		T _y = m._42 + m._12*x + m._22*y + m._32*z;
		z = m._43 + m._13*x + m._23*y + m._33*z;
		x = _x;
		y = _y;
		return *this;
	}

	template<typename T> inline
		SVector3<T>& SVector3<T>::CalcNormal(const SVector3<T>& p0, const SVector3<T>& p1, const SVector3<T>& p2)
	{
		return Sub(p1, p0).CrossProduct(SVector3<T>().Sub(p2, p0)).Normalize();
	}

	template<typename T> inline
		SVector3<T>& SVector3<T>::CalcNormal_non_Normalized(const SVector3<T>& p0, const SVector3<T>& p1, const SVector3<T>& p2)
	{
		return Sub(p1, p0).CrossProduct(SVector3<T>().Sub(p2, p0));
	}

	template<typename T> inline
		SVector3<T>& SVector3<T>::CalcCenter(const SVector3<T>& p0, const SVector3<T>& p1, const SVector3<T>& p2)
	{
		const T k_third = T(1)/3;
		x = (p0.x + p1.x + p2.x)*k_third;
		y = (p0.y + p1.y + p2.y)*k_third;
		z = (p0.z + p1.z + p2.z)*k_third;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::CrossProduct(const SVector3<T>& v1, const SVector3<T>& v2)
	{
		x = v1.y*v2.z - v1.z*v2.y;
		y = v1.z*v2.x - v1.x*v2.z;
		z = v1.x*v2.y - v1.y*v2.x;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::CrossProduct(const SVector3<T>& v)
	{
		T _x = y*v.z - z*v.y;
		T _y = z*v.x - x*v.z;
		z = x*v.y - y*v.x;
		x = _x;
		y = _y;
		return *this;
	}

	template<typename T> inline T SVector3<T>::DotProduct(const SVector3<T>& v) const
	{
		return x*v.x + y*v.y + z*v.z;
	}

	// for non-unit length vectors
	template<typename T> T SVector3<T>::Cosine(const SVector3<T>& v) const
	{
		return DotProduct(v)/std::sqrt(SquareMagnitude() * v.SquareMagnitude());
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Inertion(const SVector3<T>& p, float v)
	{
		T t = 1 - v;
		x = x*v + p.x*t;
		y = y*v + p.y*t;
		z = z*v + p.z*t;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Average(const SVector3<T>& a)
	{
		const T k_half = T(1)/2;
		x = (x + a.x)*k_half;
		y = (y + a.y)*k_half;
		z = (z + a.z)*k_half;
		return *this;
	}

	template<typename T> inline SVector3<T>& SVector3<T>::Average(const SVector3<T>& a, const SVector3<T>& b)
	{
		const T k_half = T(1)/2;
		x = (a.x + b.x)*k_half;
		y = (a.y + b.y)*k_half;
		z = (a.z + b.z)*k_half;
		return *this;
	}

	// dir, norm - unit vectors
	template<typename T> inline SVector3<T>& SVector3<T>::Reflect(const SVector3<T>& dir, const SVector3<T>& norm)
	{
		T t = -2*dir.DotProduct(norm);
		x = norm.x*t + dir.x;
		y = norm.y*t + dir.y;
		z = norm.z*t + dir.z;
		return *this;
	}

	// dir, norm - unit vectors
	template<typename T> inline SVector3<T>& SVector3<T>::Slide(const SVector3<T>& dir, const SVector3<T>& norm)
	{
		T dp = dir.DotProduct(norm);
		x = dir.x - dp*norm.x;
		y = dir.y - dp*norm.y;
		z = dir.z - dp*norm.z;
		return *this;
	}

	template<typename T> bool SVector3<T>::Similar(const SVector3<T>& v, T e) const
	{
		if (e <= std::Abs(v.x - x))
			return false;
		if (e <= std::Abs(v.y - y))
			return false;
		if (e <= std::Abs(v.z - z))
			return false;
		return true;
	}

	template<typename T> bool SVector3<T>::Inverted(const SVector3<T>& v, T e) const
	{
		if (e <= std::Abs(v.x + x))
			return false;
		if (e <= std::Abs(v.y + y))
			return false;
		if (e <= std::Abs(v.z + z))
			return false;
		return true;
	}

	template<typename T> inline int SVector3<T>::Compare(const SVector3<T>& right) const
	{
		//	return (x < right.x) ? -1 : (x == right.x && (y < right.y || (y == right.y && z < right.z)));
		if (x < right.x)
			return -1;
		else if (x > right.x)
			return +1;
		if (y < right.y)
			return -1;
		else if (y > right.y)
			return +1;
		if (z < right.z)
			return -1;
		else if (z > right.z)
			return +1;
		return 0;
	}

	template<typename T> inline bool SVector3<T>::operator<(const SVector3<T>& right) const
	{
		return x < right.x || (x == right.x && (y < right.y || (y == right.y && z < right.z)));
	}

	template<typename T> inline bool SVector3<T>::operator>(const SVector3<T>& right) const
	{
		return x > right.x || (x == right.x && (y > right.y || (y == right.y && z > right.z)));
	}

	template<typename T> inline bool SVector3<T>::operator==(const SVector3<T>& right) const
	{
		return (x == right.x && y == right.y && z == right.z);
	}

	template<typename T> inline bool SVector3<T>::operator!=(const SVector3<T>& right) const
	{
		return x != right.x || y != right.y || z != right.z;
	}

	template<typename T> inline T& SVector3<T>::operator[](size_t i) { return xyz[i]; }

	template<typename T> inline const T& SVector3<T>::operator[](size_t i) const { return xyz[i]; }
}