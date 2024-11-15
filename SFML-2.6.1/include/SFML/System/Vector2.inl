////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2023 Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T>::Vector2() :
	x(0),
	y(0)
{

}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T>::Vector2(T X, T Y) :
	x(X),
	y(Y)
{

}


////////////////////////////////////////////////////////////
template <typename T>
template <typename U>
inline Vector2<T>::Vector2(const Vector2<U>& vector) :
	x(static_cast<T>(vector.x)),
	y(static_cast<T>(vector.y))
{
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T> operator -(const Vector2<T>& right)
{
	return Vector2<T>(-right.x, -right.y);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right)
{
	left.x += right.x;
	left.y += right.y;

	return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right)
{
	left.x -= right.x;
	left.y -= right.y;

	return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T> operator +(const Vector2<T>& left, const Vector2<T>& right)
{
	return Vector2<T>(left.x + right.x, left.y + right.y);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T> operator -(const Vector2<T>& left, const Vector2<T>& right)
{
	return Vector2<T>(left.x - right.x, left.y - right.y);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T> operator *(const Vector2<T>& left, T right)
{
	return Vector2<T>(left.x * right, left.y * right);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T> operator *(T left, const Vector2<T>& right)
{
	return Vector2<T>(right.x * left, right.y * left);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T>& operator *=(Vector2<T>& left, T right)
{
	left.x *= right;
	left.y *= right;

	return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T> operator /(const Vector2<T>& left, T right)
{
	return Vector2<T>(left.x / right, left.y / right);
}


////////////////////////////////////////////////////////////
template <typename T>
inline Vector2<T>& operator /=(Vector2<T>& left, T right)
{
	left.x /= right;
	left.y /= right;

	return left;
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator ==(const Vector2<T>& left, const Vector2<T>& right)
{
	return (left.x == right.x) && (left.y == right.y);
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator !=(const Vector2<T>& left, const Vector2<T>& right)
{
	return (left.x != right.x) || (left.y != right.y);
}

////////////////////////////////////////////////////////////
//자체 제작 수정
// 
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//길이
template <typename T>
inline T SqrtMagnitude(const Vector2<T>& v)
{
	return (v.x * v.x + v.y * v.y);
}

template <typename T>
inline T Magnitude(const Vector2<T>& v)
{
	return sqrtf(SqrtMagnitude<T>(v));
}

template <>
inline float Magnitude(const Vector2<float>& v)
{
	return sqrtf(SqrtMagnitude<float>(v));
}

template <>
inline double Magnitude(const Vector2<double>& v)
{
	return sqrt(SqrtMagnitude<double>(v));
}

template <typename T>
inline T Length(const Vector2<T>& v)
{
	return Magnitude(v);
}
////////////////////////////////////////////////////////////
//거리
template <typename T>
inline T Distance(const Vector2<T>& left, const Vector2<T>& right)
{
	return Magnitude(left - right);
}
////////////////////////////////////////////////////////////
//정규화
template <typename T>
inline Vector2<T> Nomalize(const Vector2<T>& v)
{
	T len = Length(v);
	return Vector2<T>(v.x / len, v.y / len);
}
////////////////////////////////////////////////////////////
//Dot
template <typename T>
inline T Dot(const Vector2<T>& left, const Vector2<T>& right)
{
	return left.x * right.x + left.y * right.y;
}

