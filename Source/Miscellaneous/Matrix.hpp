#pragma once

#include <initializer_list>
#include <iostream>
#include <array>

namespace dty {
	template<typename T, int32_t SizeY>
	struct Matrix1xGeneric {
		using MatrixType = Matrix1xGeneric<T, SizeY>;

		T& operator[](size_t _index) {
			return column[_index];
		}

	private:
		std::array<T, SizeY> column = { 0 };
	};

	template<typename T, int32_t SizeX, int32_t SizeY>
	struct MatrixGeneric {
		using MatrixType = MatrixGeneric<T, SizeX, SizeY>;
		using Matrix1xType = Matrix1xGeneric<T, SizeY>;

		Matrix1xType& operator[](size_t _index) {
			return rows[_index];
		}

		// This won't work for anything other than a 2x2 matrix!
		MatrixType& Invert() {
			T
				&a = rows[0][0],
				&b = rows[0][1],
				&c = rows[1][0],
				&d = rows[1][1];

			double determinant = 1 / (a * d - b * c );

			a = determinant * d;
			b = determinant * -b;
			c = determinant * -c;
			d = determinant * a;

			return *this;
		}

		friend std::ostream& operator<<(std::ostream& _ostream, MatrixType& _value) {
			for (size_t indexX = 0; indexX < SizeX; ++indexX) {
				_ostream << "[ ";
				for (size_t indexY = 0; indexY < SizeY; ++indexY) {
					_ostream << _value[indexX][indexY] << " ";
				}
				_ostream << "]\n";
			}

			return _ostream;
		}
	private:
		std::array<Matrix1xType, SizeX> rows;
	};

	using Matrix2x2 = MatrixGeneric<double, 2, 2>;
}