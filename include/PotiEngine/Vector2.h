#pragma once

#include <math.h>
#include <PotiEngine/pi.h>
#include <cmath>
#include <iostream>

namespace PotiEngine
{
	template<class T>
	class Vector2
	{
		
		public:

			T x = 0;
			T y = 0;

			Vector2();
			explicit Vector2(T V);
			Vector2(T a, T b);
			Vector2(const Vector2& a);
			~Vector2() = default;

		public:
			//Operator with vector
				// + operator
			Vector2 operator+(const Vector2& v) const;
	

				// - operator
			Vector2 operator-(const Vector2& v) const;
		

				// * operator
			Vector2 operator*(const Vector2& v) const;
			

				// / operator
			Vector2 operator/(const Vector2& v) const;
			


			//Operator with scalar
				// * operator
			Vector2 operator*(T a) const;
			

				// / operator
			Vector2 operator/(T a) const;
			



			Vector2& operator+=(const Vector2& v);
			Vector2& operator-=(const Vector2& v);
			Vector2& operator*=(const Vector2& v);
			Vector2& operator/=(const Vector2& v);

			Vector2& operator*=(T a);
			Vector2& operator/=(T a);

			bool operator==(const Vector2& v) const;
			

			bool operator!=(const Vector2& v) const;
			

			operator Vector2<float>() const;

		public:
			//Function

			float Length() const;
			Vector2<float> Normalize() const;
			
			float Dot(T a, T b) const;
			float Dot(const Vector2& a) const;
			
			float DistanceTo(T a, T b) const;
			float DistanceTo(const Vector2& a) const;

			static Vector2<T> Rotate(const Vector2& vec, float degrees);
			
			
	};

	using Vector2f = Vector2<float>;
	using Vector2i = Vector2<int>;
	
	
	//Constructor
	template<typename T>
	Vector2<T>::Vector2()
	{
		x = T{};
		y = T{};
	}

	template <class T>
	Vector2<T>::Vector2(T a, T b) {
		x = a;
		y = b;
	}

	template<typename T>
	Vector2<T>::Vector2(T V) :
		x(V),
		y(V)
	{
	}

	template <class T>
	Vector2<T>::Vector2(const Vector2& v) {
		x = (T)v.x;
		y = (T)v.y;
	}

	//Operator with vector
		// + operator

	template <class T>
	Vector2<T> Vector2<T>::operator+(const Vector2& v) const 
	{
		return Vector2<T>(x + v.x, y + v.y);
	}

		//- operator
	template <class T>
	Vector2<T> Vector2<T>::operator-(const Vector2& v) const 
	{
		return Vector2<T> (x - v.x, y - v.y);
	}


		//* operator
	template <class T>
	Vector2<T> Vector2<T>::operator*(const Vector2& v) const 
	{
		return Vector2<T> (x * v.x, y * v.y );
	}

	
		// / operator
	template <class T>
	Vector2<T> Vector2<T>::operator/(const Vector2& v) const 
	{
		return Vector2<T> (x / v.x, y / v.y );
	}

	

	//Operator with scalar
		// * operator

	template <class T>
	Vector2<T> Vector2<T>::operator*(T a) const 
	{
		return Vector2<T> (x * a, y * a );
	}

		// / operator
	template <class T>
	Vector2<T> Vector2<T>::operator/(T a) const 
	{
		return Vector2<T> (x / a, y / a );
	}


	
	template <class T>
	Vector2<T>& Vector2<T>::operator+=(const Vector2& v) 
	{
		return *this = *this + v;
	}

	template <class T>
	Vector2<T>& Vector2<T>::operator-=(const Vector2& v) 
	{
		return *this = *this - v;
	}

	template <class T>
	Vector2<T>& Vector2<T>::operator*=(const Vector2& v) 
	{
		return *this = *this * v;
	}

	template <class T>
	Vector2<T>& Vector2<T>::operator/=(const Vector2& v) 
	{
		return *this = *this / v;
	}

	template <class T>
	Vector2<T>& Vector2<T>::operator*=(T a) 
	{
		return *this = *this * a;
	}

	template <class T>
	Vector2<T>& Vector2<T>::operator/=(T a) 
	{
		return *this = *this / a;
	}

	//== operator
	template <class T>
	bool Vector2<T>::operator==(const Vector2<T>& v) const 
	{
		return x == v.x && y == v.y;
	}



	//!= operator
	template <class T>
	bool Vector2<T>::operator!=(const Vector2<T>& v) const 
	{
		return !(*this == v);
	}



	template <class T>
	Vector2<T>::operator Vector2<float>() const 
	{
		return { static_cast<float>(this->x), static_cast<float>(this->y) };
	}

	//====Operateur libre====

	template<typename T, typename T2>
	Vector2<T> operator*(T2 a, const Vector2<T>& v);

	template <typename T, typename T2>
	Vector2<T> operator*(T2 a, const Vector2<T>& v)
	{
		return Vector2<T>(a * v.x, a * v.y);
	}

	template<typename T, typename T2>
	Vector2<T> operator/(T2 a, const Vector2<T>& v);

	template <typename T, typename T2>
	Vector2<T> operator/(T2 a, const Vector2<T>& v)
	{
		return Vector2<T>(a / v.x, a / v.y);
	}


	//====Function====

	template <class T>
	float Vector2<T>::Length() const {
		return x + y;
	}

	template <class T>
	Vector2<float> Vector2<T>::Normalize() const 
	{
		float s = Length();

		if (s == 0.f) 
		{
			return { 0.f, 0.f };
		}

		return { x / s, y / s };
	}

	template <class T>
	float Vector2<T>::Dot(T a, T b) const
	{
		return (this->x * a) + (this->y * b);
	}

	
	template <class T>
	float Vector2<T>::Dot(const Vector2& a) const 
	{
		return Dot(a.x, a.y);
	}

	template <class T>
	float Vector2<T>::DistanceTo(T a, T b) const 
	{
		return sqrtf(powf(this->x - a, 2.0f) + powf(this->y - b, 2.0f));
	}
	
	template <class T>
	float Vector2<T>::DistanceTo(const Vector2& a) const
	{

		return DistanceTo(a.x, a.y);
	}

	template<typename T>
	Vector2<T> PotiEngine::Vector2<T>::Rotate(const Vector2& vec, float degrees)
	{
		float radRotation = DEG2RAD(degrees);
		float s = std::sin(radRotation);
		float c = std::cos(radRotation);

		Vector2 rotatedVec;
		rotatedVec.x = vec.x * c - vec.y * s;
		rotatedVec.y = vec.x * s + vec.y * c;

		return rotatedVec;
	}

}

