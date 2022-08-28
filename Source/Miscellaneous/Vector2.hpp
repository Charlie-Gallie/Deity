#pragma once

namespace dty {
	template<typename T>
	struct Vector2Generic {
		using VecType = Vector2Generic<T>;

		union {
			struct { T x, y; };
			struct { T width, height; };
		};

		/*		Operators		*/

		// + - * /
		VecType& operator +(const VecType& _other) {
			return {
				(T)(x + _other.x),
				(T)(y + _other.y)
			};
		}

		VecType& operator -(const VecType& _other) {
			return {
				(T)(x - _other.x),
				(T)(y - _other.y)
			};
		}

		VecType& operator *(const VecType& _other) {
			return {
				(T)(x * _other.x),
				(T)(y * _other.y)
			};
		}

		VecType& operator /(const VecType& _other) {
			return {
				(T)(x / _other.x),
				(T)(y / _other.y)
			};
		}

		// += -= *= /=
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
	};

	using Vector2 = Vector2Generic<double>;
	using Vector2f = Vector2Generic<float>;
	using Vector2i = Vector2Generic<int32_t>;
	using Vector2ui = Vector2Generic<uint32_t>;
}