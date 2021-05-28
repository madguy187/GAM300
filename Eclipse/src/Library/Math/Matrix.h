#pragma once

#include <iostream>
#include <vector>

namespace Eclipse
{
    template <typename T, size_t M, size_t N>
    class Matrix 
    {
    public:
        using size_type = size_t;
        using value_type = T;
        using reference = value_type&;
        using const_reference = const reference;
        using pointer = value_type*;
        using const_pointer = const value_type*;

        Matrix()
            : rows{ M }, cols{ N } 
        {
            for (size_type i = 0; i < rows; i++)
            {
                // For Rows
                data.push_back(std::vector<T>());

                for (size_type j = 0; j < cols; j++)
                {
                    // For Cols
                    data[i].push_back(T{});
                }
            }
        }

        Matrix(const T* pArr)
            : rows{ M }, cols{ N }
        {
            data.reserve(rows);

            for (size_type i = 0; i < rows; i++)
            {
                // For Rows
                data[i].reserve(cols);

                for (size_type j = 0; j < cols; j++)
                {
                    // For Cols
                    data[i][j] = (pArr[i]);
                }
            }
        }

        Matrix(Matrix const& rhs)
            : rows{ rhs.Rows() }, cols{ rhs.Cols() }, data{ rhs.data } {}

        Matrix<T, M, N>& operator=(Matrix<T, M, N> const& rhs)
        {
            if (&rhs != this)
            {
                rows = rhs.Rows();
                cols = rhs.Cols();

                for (size_t i = 0; i < rhs.Rows(); i++)
                {
                    for (size_t j = 0; j < rhs.Cols(); j++)
                    {
                        data[i][j] = rhs.data[i][j];
                    }
                }
            }

            return *this;
        }

        // Binary/Unary operators
        Matrix<T, M, N> operator+(Matrix<T, M, N> const& rhs)
        {
            Matrix<T, M, N> temp;

            for (size_t i = 0; i < rhs.Rows(); i++) 
            {
                for (size_t j = 0; j < rhs.Cols(); j++) 
                {
                    temp[i][j] = data[i][j] + rhs.data[i][j];
                }
            }

            return temp;
        }

        Matrix<T, M, N> operator-(Matrix<T, M, N> const& rhs)
        {
            Matrix<T, M, N> temp;

            for (size_t i = 0; i < rhs.Rows(); i++) 
            {
                for (size_t j = 0; j < rhs.Cols(); j++)
                {
                    temp[i][j] = data[i][j] - rhs.data[i][j];
                }
            }

            return temp;
        }

        Matrix<T, M, N> operator*(Matrix<T, M, N> const& rhs)
        {
            Matrix<T, M, N> temp;

            for (size_t i = 0; i < rhs.Rows(); i++) 
            {
                for (size_t j = 0; j < rhs.Cols(); j++) 
                {
                    temp[i][j] = 0;

                    for (size_t k = 0; k < rhs.Rows(); k++) 
                    {
                        temp.data[i][j] += data[i][k] * rhs.data[k][j];
                    }
                }
            }

            return temp;
        }

        Matrix<T, M, N> operator*(const value_type scalar)
        {
            Matrix<T, M, N> temp;

            for (size_t i = 0; i < rows; i++) 
            {
                for (size_t j = 0; j < cols; j++) 
                {
                    temp[i][j] = data[i][j] * scalar;
                }
            }

            return temp;
        }

        // Assignment operators
        Matrix<T, M, N>& operator+=(Matrix<T, M, N> const& rhs)
        {
            *this = *this + rhs;
            return *this;
        }

        Matrix<T, M, N>& operator-=(Matrix<T, M, N> const& rhs)
        {
            *this = *this - rhs;
            return *this;
        }

        Matrix<T, M, N>& operator*=(Matrix<T, M, N> const& rhs)
        {
            *this = *this * rhs;
            return *this;
        }

        Matrix<T, M, N>& operator*=(const value_type scalar)
        {
            *this = *this * scalar;
            return *this;
        }

        // Boolean Functions
        bool operator==(Matrix<T, M, N> const& rhs) const
        {
            if (rows != rhs.rows || cols != rhs.cols)
            {
                return false;
            }

            for (size_t i = 0; i < rows; ++i)
            {
                for (size_t j = 0; j < cols; ++j)
                {
                    if (data[i][j] != rhs.data[i][j])
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        bool operator!=(Matrix<T, M, N> const& rhs) const
        {
            return !(*this == rhs);
        }

        std::vector<T>& operator[] (const size_t index)
        {
            return data[index];
        }

        size_t Rows() const
        {
            return rows;
        }

        size_t Cols() const
        {
            return cols;
        }

        typedef Matrix<float, 3, 3> ECMat3;
        typedef Matrix<float, 4, 4> ECMat4;

    private:
        size_type rows;
        size_type cols;
        std::vector<std::vector<T>> data;
    };

    template <typename T, size_t M, size_t N>
    std::ostream& operator<<(std::ostream& os, const Matrix<T, M, N> m) 
    {
        for (size_t i = 0; i < m.Rows(); i++) 
        {
            for (size_t j = 0; j < m.Cols(); j++) 
            {
                os << m[i][j] << " ";
            }

            os << "\n";
        }

        return os;
    }

    template <typename T, size_t M, size_t N>
    Matrix<T, M, N> operator*(T const& scalar, Matrix<T, M, N>& rhs) 
    {
        Matrix<T> result(rhs.Rows(), rhs.Cols());

        for (size_t i = 0; i < rhs.Rows(); ++i)
        {
            for (size_t j = 0; j < rhs.Cols(); ++j)
            {
                result[i][j] = scalar * rhs.Data(i, j);
            }
        }

        return result;
    }
}
