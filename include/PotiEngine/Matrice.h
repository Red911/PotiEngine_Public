#pragma once
#include "vector"
#include "PotiEngine/Vector2.h"
#include "math.h"
#include "iostream"

namespace PotiEngine
{
	template<class T>
	class Matrice
	{
		public :
			
			//Constror & Deconstructor
			Matrice(int row, int col);
			Matrice(std::vector<T> mat, int row, int col);
			Matrice(const Matrice& mat);
			~Matrice() = default;

			T& Value(std::size_t i, std::size_t j);
			const T& Value(std::size_t i, std::size_t j) const;

			//Getter
			int GetNumberOfLines() const;
			int GetNumberOfColumns() const;
			std::vector<T> GetMatrice() const;
			void SetValue(int index, T value);

			//Identity
			static Matrice Identity(int size);
			bool isIdentity();

			//Operation
			template<typename U> static Matrice Multiply(const Matrice<T>& m1, U a);
			Matrice Multiply(const Matrice& m2);
			static Matrice Multiply(const Matrice& m1, const Matrice& m2);

			
			 template<typename U> static Matrice Multiply(const Matrice<T> mat, const Vector2<U>& v);

			Matrice Add(const Matrice& m2);

			static Matrice Add(const Matrice& m1, const Matrice& m2);

			//Operator
			template<typename U> Matrice operator*(U a);
			Matrice operator*(const Matrice& m2);
			
			Matrice operator-(const Matrice & m2);
			Matrice operator+(const Matrice& m2);
			Vector2<T> operator*(const Vector2<T>& v);

			T& operator[](int index);
			T& operator[](const Vector2<T> v);

			static Matrice Transpose(const Matrice<T> mat);
			static Matrice GenerateAugmentedMatrice(const Matrice<T> m1, const Matrice<T> m2);
			std::tuple<Matrice<T>, Matrice<T>> Split(int a);

			static void SwapLine(Matrice<T>& mat, int nLine, int otherLine);
			static void SwapColumn(Matrice<T>& mat, int nCol, int otherCol);

			static void MultiplyLine(Matrice<T>& mat, int nLine, float scalar);
			static void MultiplyColumn(Matrice<T>& mat, int nCol, int scalar);

			static void AddLineToAnother(Matrice<T>& mat, int nLine, int otherLine, int scalar);
			static void AddColumnToAnother(Matrice<T>& mat, int nCol, int otherCol, int scalar);

			static Matrice GetMinorMatrice(const Matrice<T> m, int row, int col);

			static Matrice Cofactor(const Matrice<T>& m);
			static int determinantOfMatrix(const Matrice<T>& mat, int dimension);

			Matrice GetInverse(const Matrice<T> m);

			static Matrice Translate(const Vector2<T>& translation);

			static std::string PrintMatrice(const Matrice<T>& mat);



		private:
			int m_nLine;
			int m_nColumns;
			std::vector<T> m_matrice;

	};

	using Matricef = Matrice<float>;
	using Matricei = Matrice<int>;

	//==== Constructeurs ====
	template<class T>
	inline Matrice<T>::Matrice(int row, int col)
	{
		m_nLine = row;
		m_nColumns = col;

		m_matrice = std::vector<T>(m_nLine * m_nColumns, 0);
		
	}

	template<class T>
	inline Matrice<T>::Matrice(std::vector<T> mat, int row, int col)
	{
		m_matrice = mat;

		m_nLine = row;
		m_nColumns = col;
	}

	template<class T>
	inline Matrice<T>::Matrice(const Matrice& mat)
	{
		m_matrice = mat.m_matrice;

		m_nLine = mat.m_nLine;
		m_nColumns = mat.m_nColumns;

	
	}

	template<class T>
	inline T& Matrice<T>::Value(std::size_t i, std::size_t j)
	{
		return m_matrice[i * 3 + j];
	}

	template<class T>
	inline const T& Matrice<T>::Value(std::size_t i, std::size_t j) const
	{
		return m_matrice[i * 3 + j];
	}
	
	// =====================

	// ==== Getter ====

	template<class T>
	inline int Matrice<T>::GetNumberOfLines() const
	{
		return m_nLine;
	}

	template<class T>
	inline int Matrice<T>::GetNumberOfColumns() const
	{
		return m_nColumns;
	}

	template<class T>
	inline std::vector<T> Matrice<T>::GetMatrice() const
	{
		return m_matrice;
	}

	template<class T>
	inline void Matrice<T>::SetValue(int index, T value)
	{
		m_matrice[index] = value;
	}


	// ================

	template<class T>
	inline T& Matrice<T>::operator[](int index)
	{
		return m_matrice[index];
	}

	template<class T>
	inline T& Matrice<T>::operator[](const Vector2<T> v)
	{
		assert(v.x < m_nColumns && v.y < m_nLine);
		assert(v.x > 0 && v.y > 0);

		return m_matrice[v.y * m_nColumns + v.x];
	}

	

	// ==== Identity ====

	template<class T>
	Matrice<T> Matrice<T>::Identity(int size)
	{
		Matrice mat(size, size);

		for (uint32_t i = 0; i < size; i++)
		{
			for (uint32_t j = 0; j < size; j++)
			{
				if (i == j) 
				{
					mat.m_matrice[j * mat.m_nColumns + i] = 1;
				}
				else
				{
					mat.m_matrice[j * mat.m_nColumns + i] = 0;
				}
			}
		}

		return mat;
	}

	template<class T>
	inline bool Matrice<T>::isIdentity()
	{
		if (m_nLine != m_nColumns) return false;

		for (uint32_t i = 0; i < m_nColumns; i++)
		{
			for (uint32_t j = 0; j < m_nLine; j++)
			{
				if (i == j)
				{
					if (m_matrice[j * m_nColumns + i] != 1) return false;
				}
				else if (m_matrice[j * m_nColumns + i] != 0) return false;
			}
		}

		return true;
	}
	// =====================
	// 
	// ==== Operation ====

		//Multiply

	template<class T>
	template<typename U>
	inline Matrice<T> Matrice<T>::Multiply(const Matrice<T>& m1, U a)
	{
		Matrice<T> m2(m1);

		for (uint32_t i = 0; i < m2.m_nColumns; i++)
		{
			for (uint32_t j = 0; j < m2.m_nLine; j++)
			{
				m2.m_matrice[j * m2.m_nColumns + i] = m1.m_matrice[j * m1.m_nColumns + i] * a;
			}
		}

		return m2;
	}

	template<class T>
	inline Matrice<T> Matrice<T>::Multiply(const Matrice& m2)
	{
		if (m_nColumns != m2.GetNumberOfLines())
			throw std::invalid_argument("The number of columns of Matrice 1 must be equal to the number of line of Matrice 2");

		Matrice<T> m3(m_nLine, m2.GetNumberOfColumns());

		for (int i = 0; i < m3.m_nColumns; i++)
		{
			for (int j = 0; j < m3.m_nLine; j++)
			{

				for (int k = 0; k < m_nColumns; k++)
				{

					m3.m_matrice[j * m3.m_nColumns + i] += m_matrice[j * m_nColumns + k] * m2.m_matrice[k * m2.m_nColumns + i];
				}
			}

		}

		return m3;
	}

	template<class T>
	inline Matrice<T> Matrice<T>::Multiply(const Matrice& m1, const Matrice& m2)
	{
		if (m1.m_nColumns != m2.GetNumberOfLines())
			throw std::invalid_argument("The number of columns of Matrice 1 must be equal to the number of line of Matrice 2");

		Matrice<T> m3(m1.m_nLine, m2.GetNumberOfColumns());

		for (int i = 0; i < m3.m_nColumns; i++)
		{
			for (int j = 0; j < m3.m_nLine; j++)
			{

				for (int k = 0; k < m1.m_nColumns; k++)
				{

					m3.m_matrice[j * m3.m_nColumns + i] += m1.m_matrice[j * m1.m_nColumns + k] * m2.m_matrice[k * m2.m_nColumns + i];
				}
			}

		}

		return m3;
	}

	template<class T>
	template<class U>
	inline Matrice<T> Matrice<T>::Multiply(const Matrice<T> mat, const Vector2<U>& v)
	{
		Matrice<U> vm(3,1);

		vm.m_matrice[0] = v.x;
		vm.m_matrice[1] = v.y;
		vm.m_matrice[2] = 1;

		return Matrice<T>::Multiply(mat, vm);
	}

	
	

	template<class T>
	inline Matrice<T> Matrice<T>::Add(const Matrice& m2)
	{
		if (m_nLine != m2.m_nLine || m_nColumns != m2.m_nColumns)
			throw std::invalid_argument("The row and columns must be the same");

		Matrice<T> mat(m_matrice, m_nLine, m_nColumns);

		for (int i = 0; i < mat.m_nColumns; i++)
		{
			for (int j = 0; j < mat.m_nLine; j++)
			{
				mat.m_matrice[j * mat.m_nColumns + i] += m2.m_matrice[j * m2.m_nColumns + i];
			}
		}

		return mat;
	}

	template<class T>
	inline Matrice<T> Matrice<T>::Add(const Matrice& m1, const Matrice& m2)
	{
		if (m1.m_nLine != m2.m_nLine || m1.m_nColumns != m2.m_nColumns)
			throw std::invalid_argument("The row and columns must be the same");

		Matrice<T> m3(m1);

		for (int i = 0; i < m1.m_nColumns; i++)
		{
			for (int j = 0; j < m1.m_nLine; j++)
			{
				m3.m_matrice[j * m3.m_nColumns + i] = m1.m_matrice[j * m1.m_nColumns + i] + m2.m_matrice[j * m2.m_nColumns + i];
			}
		}

		return m3;
	}



	

	// ==== Operator ====

		//Multiply

	template<class T>
	template<typename U>
	inline Matrice<T> Matrice<T>::operator*(U a)
	{
		Matrice<T> mat(m_matrice, m_nLine, m_nColumns);

		for (uint32_t i = 0; i < mat.GetNumberOfLines(); i++)
		{
			mat[i] *= a;
		}
		return mat;
	}

	template<class T>
	inline Vector2<T> Matrice<T>::operator*(const Vector2<T>& v)
	{
		Vector2<T> result;

		result.x = Value(0, 0) * v.x + Value(0, 1) * v.y + Value(0, 2);
		result.y = Value(1, 0) * v.x + Value(1, 1) * v.y + Value(1, 2);


		return result;
	}

	template<class T>
	inline Matrice<T> Matrice<T>::operator*(const Matrice& m2)
	{
		return Multiply(m2);
	}

	template<class T>
	inline Matrice<T> Matrice<T>::operator-(const Matrice& m2)
	{
		return Add(Multiply(m2, -1));
	}
	

	template<class T>
	inline Matrice<T> Matrice<T>::operator+(const Matrice& m2)
	{

		return Add(m2);
	}

	

	

	//Operateur Libre

	template<typename T, typename T2>
	Matrice<T> operator*(T2 a, const Matrice<T>& m);

	template <typename T, typename T2>
	Matrice<T> operator*(T2 a, const Matrice<T>& m)
	{
		return Matrice<T>::Multiply(m , a);
	}

	template<typename T, typename T2>
	Matrice<T> operator*(const Matrice<T>& m2, const Matrice<T2>& m1);

	template <typename T, typename T2>
	Matrice<T> operator*(const Matrice<T>& m2, const Matrice<T2>& m1)
	{
		return Matrice<T>::Multiply(m2, m1);
	}

	

	template<typename T, typename U>
	Matrice<T> operator*(const Matrice<T>& mat, const Vector2<U> v);

	template<typename T, typename U>
	Matrice<T> operator*(const Matrice<T>& mat, const Vector2<U> v)
	{
		return Matrice<T>::Multiply(mat, v);
	}

	template<typename T, typename T2>
	Matrice<T> operator+(const Matrice<T>& m2, const Matrice<T2>& m1);

	template <typename T, typename T2>
	Matrice<T> operator+(const Matrice<T>& m2, const Matrice<T2>& m1)
	{
		return Matrice<T>::Add(m2, m1);
	}

	template<typename T, typename T2>
	Matrice<T> operator-(const Matrice<T>& m2, const Matrice<T2>& m1);

	template <typename T, typename T2>
	Matrice<T> operator-(const Matrice<T>& m2, const Matrice<T2>& m1)
	{
		return Matrice<T>::Add(m2, Matrice<T>::Multiply(m1, -1));
	}


	

	// =====================

	template<class T>
	inline Matrice<T> Matrice<T>::Transpose(const Matrice<T> mat)
	{
		Matrice<T> m2(mat.m_nColumns,mat.m_nLine);

		for (uint32_t i = 0; i < mat.m_nColumns; i++)
		{
			for (uint32_t j = 0; j < mat.m_nLine; j++)
			{
				m2.m_matrice[i * m2.m_nColumns + j] = mat.m_matrice[j * mat.m_nColumns + i];
			}
		}

		return m2;
	}
	template<class T>
	inline Matrice<T> Matrice<T>::GenerateAugmentedMatrice(const Matrice<T> m1, const Matrice<T> m2)
	{
		if (m1.m_nLine != m2.m_nLine)
			throw std::invalid_argument("b must be different of zero");

		Matrice<T> m3(m1.m_nLine, m1.m_nColumns + m2.m_nColumns);

		for (int i = 0; i < m3.m_nColumns; i++)
		{
			for (int j = 0; j < m3.m_nLine; j++)
			{
				if (i < m1.m_nColumns)
				{
					m3.m_matrice[j * m3.m_nColumns + i] = m1.m_matrice[j * m1.m_nColumns + i];
				}
				else
				{
					m3.m_matrice[j * m3.m_nColumns + i] = m2.m_matrice[(j - m1.m_nColumns) * m2.m_nColumns + i];
				}
			}
		}


		return m3;
	}

	template<class T>
	inline std::tuple<Matrice<T>, Matrice<T>> Matrice<T>::Split(int a)
	{
		Matrice<T> m1(m_nLine, a + 1);
		Matrice<T> m2(m_nLine, m_nColumns - a - 1);

		for (int i = 0; i < m1.m_nColumns; i++)
		{
			for (int j = 0; j < m1.m_nLine; j++)
			{
				m1.m_matrice[j * m1.m_nColumns + i] = m_matrice[j * m_nColumns + i];
			}
		}

		for (int i = 0; i < m2.m_nColumns; i++)
		{
			for (int j = 0; j < m2.m_nLine; j++)
			{
				m2.m_matrice[j * m2.m_nColumns + i] = m_matrice[(j + a + 1) * m_nColumns + i];
			}
		}

		return { m1,m2 };
	}

	template<class T>
	inline void Matrice<T>::SwapLine(Matrice<T>& mat, int nLine, int otherLine)
	{
		for (uint32_t i = 0; i < mat.m_nColumns; i++)
		{
			std::swap(mat.m_matrice[i * mat.m_nColumns + nLine], mat.m_matrice[i * mat.m_nColumns + otherLine]);
		}
	}

	template<class T>
	inline void Matrice<T>::SwapColumn(Matrice<T>& mat, int nCol, int otherCol)
	{
		for (uint32_t i = 0; i < mat.m_nLine; i++)
		{
			std::swap(mat.m_matrice[nCol * mat.m_nColumns + i], mat.m_matrice[otherCol * mat.m_nColumns + i]);
		}

	}
	template<class T>
	inline void Matrice<T>::MultiplyLine(Matrice<T>& mat, int nLine, float scalar)
	{
		if(scalar == 0)
			throw std::invalid_argument("b must be different of zero");

		for (uint32_t i = 0; i < mat.m_nColumns; i++)
		{
			mat.m_matrice[i * mat.m_nColumns + nLine] *= scalar;
		}
	}
	template<class T>
	inline void Matrice<T>::MultiplyColumn(Matrice<T>& mat, int nCol, int scalar)
	{
		if (scalar == 0)
			throw std::invalid_argument("b must be different of zero");

		for (uint32_t i = 0; i < mat.m_nLine; i++)
		{
			mat.m_matrice[nCol * mat.m_nColumns + i] *= scalar;
		}
	}
	template<class T>
	inline void Matrice<T>::AddLineToAnother(Matrice<T>& mat, int nLine, int otherLine, int scalar)
	{
		for (uint32_t i = 0; i < mat.m_nColumns; i++)
		{
			mat.m_matrice[i * mat.m_nColumns + otherLine] += mat.m_matrice[i * mat.m_nColumns + otherLine] * scalar;
		}
	}
	template<class T>
	inline void Matrice<T>::AddColumnToAnother(Matrice<T>& mat, int nCol, int otherCol, int scalar)
	{
		for (uint32_t i = 0; i < mat.m_nLine; i++)
		{
			mat.m_matrice[otherCol * mat.m_nColumns + i] *= mat.m_matrice[nCol * mat.m_nColumns + i] *scalar;
		}
	}
	template<class T>
	inline Matrice<T> Matrice<T>::GetMinorMatrice(const Matrice<T> m,  int row, int col)
	{
		assert(row >= 0 && row < m.m_nLine);
		assert(col >= 0 && col < m.m_nColumns);
		assert(m.m_matrice.size() == m.m_nLine * m.m_nColumns);


		Matrice<T> retMatrix(m.m_nLine - 1, m.m_nColumns - 1);

		int32_t i = 0;
		for (int32_t y = 0; y < m.m_nLine; y++)
		{
			if (y == row)
				continue;

			for (int32_t x = 0; x < m.m_nColumns; x++)
			{
				if (x == col)
					continue;

				retMatrix.m_matrice[i] = m.m_matrice[y * m.m_nColumns + x];
				i++;
			}
		}

		return retMatrix;
		
	}
	template<class T>
	inline Matrice<T> Matrice<T>::Cofactor(const Matrice<T>& m)
	{
		Matrice<T> mat(m.m_nLine, m.m_nColumns);

		for (int i = 0; i < mat.m_nColumns; i++)
		{
			for (int j = 0; j < mat.m_nLine; j++)
			{
				
				auto min = Matrice<T>::GetMinorMatrice(m, i, j);
				auto d = Matrice<T>::determinantOfMatrix(min, 2);
				auto p = pow(-1, i + j) * d;

				mat.m_matrice[j * mat.m_nColumns + i] = p;
			}
		}

		return mat;

	}
	template<class T>
	inline int Matrice<T>::determinantOfMatrix(const Matrice<T>& mat, int dimension)
	{
		int det = 0;
		Matrice<T> m(mat.m_nLine,mat.m_nColumns);

		if (dimension == 1) {
			return mat.m_matrice[0];
		}
		else if (dimension == 2) {
			return ((mat.m_matrice[0] * mat.m_matrice[3]) - (mat.m_matrice[1] * mat.m_matrice[2]));
		}
		else 
		{
			int a11 = mat.m_matrice[0] * determinantOfMatrix(GetMinorMatrice(mat, 0, 0), 2);

			int a12 = mat.m_matrice[1] * determinantOfMatrix(GetMinorMatrice(mat, 0, 1), 2);

			int a13 = mat.m_matrice[2] * determinantOfMatrix(GetMinorMatrice(mat, 0, 2), 2);

			det = a11 - a12 + a13;
		}

		return det;
		
	}

	template<class T>
	inline Matrice<T> Matrice<T>::GetInverse(const Matrice<T> m)
	{
		Matrice<T> inverse(m.m_nLine, m.m_nColumns);

		Matrice<T> cofact(Cofactor(m));
		Matrice<T> transpose(Transpose(cofact));

		auto d = determinantOfMatrix(m, m.m_nLine);

		inverse = transpose * (1 / determinantOfMatrix(m, m.m_nLine));

		return inverse;
	}

	template<class T>
	inline Matrice<T> Matrice<T>::Translate(const Vector2<T>& translation)
	{
		Matrice<T> mat(std::vector<T>{ 1.0f, 0.0f, translation.x,
									   0.0f, 1.0f, translation.y,
									   0.0f, 0.0f, 1.0f, }, 
			3, 3);
		return mat;
	}
	

	template<class T>
	inline std::string Matrice<T>::PrintMatrice(const Matrice<T>& mat)
	{
	/*	std::string matrixString;

		for (int i = 0; i < mat.m_nColumns; i++)
		{
			matrixString += "[";

			for (int j = 0; j < mat.m_nLine; j++)
			{
				matrixString += std::to_string(mat.m_matrice[i * mat.m_nColumns + j]);

				if (j != mat.m_nLine - 1)
					matrixString += ",";
			}

			matrixString += "]\n";
		}

		matrixString += "\n";

		return matrixString;*/
	}
}


