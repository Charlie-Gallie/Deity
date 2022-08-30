#pragma once

#include <olcPixelGameEngine.h>
#include <initializer_list>

namespace dty {
	template<typename T>
	struct Vector2Generic {
		using VecType = Vector2Generic<T>;

		union {
			struct { T x, y; };
			struct { T width, height; };
		};

		operator olc::vi2d() {
			return {
				(int)x,
				(int)y
			};
		}

		operator olc::vf2d() {
			return {
				(float)x,
				(float)y
			};
		}

		/*		Constructors		*/
		Vector2Generic(T _x = 0, T _y = 0) {
			x = _x;
			y = _y;
		}

		Vector2Generic(const VecType& _value) {
			*this = _value;
		}

		/*		Operators		*/

		// + - * /
		// Todo: Removing casting - It's guaranteed to be T
		template<typename O>
		VecType operator +(const Vector2Generic<O>& _other) {
			return VecType(
				(T)(x + (T)_other.x),
				(T)(y + (T)_other.y)
			);
		}

		template<typename O>
		VecType operator -(const Vector2Generic<O>& _other) {
			return VecType(
				(T)(x - (T)_other.x),
				(T)(y - (T)_other.y)
			);
		}

		template<typename O>
		VecType operator *(const Vector2Generic<O>& _other) {
			return VecType(
				(T)(x * (T)_other.x),
				(T)(y * (T)_other.y)
			);
		}

		template<typename O>
		VecType operator /(const Vector2Generic<O>& _other) {
			return VecType(
				(T)(x / (T)_other.x),
				(T)(y / (T)_other.y)
			);
		}

		// += -= *= /= =
		VecType& operator +=(const VecType& _other) {
			(T)(x += _other.x);
			(T)(y += _other.y);

			return *this;
		}

		VecType& operator -=(const VecType& _other) {
			(T)(x -= _other.x);
			(T)(y -= _other.y);

			return *this;
		}

		VecType& operator *=(const VecType& _other) {
			(T)(x *= _other.x);
			(T)(y *= _other.y);

			return *this;
		}

		VecType& operator /=(const VecType& _other) {
			(T)(x /= _other.x);
			(T)(y /= _other.y);

			return *this;
		}

		VecType& operator =(T _value) {
			x = _value;
			y = _value;

			return *this;
		}

		VecType& operator =(const VecType& _other) {
			x = _other.x;
			y = _other.y;

			return *this;
		}
	};

	// Conversions
	template<typename To, typename From>
	Vector2Generic<To> ConvertVector2(const Vector2Generic<From>& _value) {
		return {
			(To)_value.x,
			(To)_value.y
		};
	}

	template<typename To, typename From>
	olc::v2d_generic<To> ToV2D(const Vector2Generic<From>& _value) {
		return olc::v2d_generic<To>(
			static_cast<To>(_value.x),
			static_cast<To>(_value.y)
		);
	}

	using Vector2 = Vector2Generic<double>;
	using Vector2f = Vector2Generic<float>;
	using Vector2i = Vector2Generic<int32_t>;
	using Vector2u = Vector2Generic<uint32_t>;
}