#pragma once

//#include "Vector.h"
#include <iostream>
#include <vector>
#include <algorithm>

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
                data.push_back(std::vector<value_type>());

                for (size_type j = 0; j < cols; j++)
                {
                    // For Cols
                    data[i].push_back(value_type{});
                }
            }
        }

        Matrix(float value)
            : rows{ M }, cols{ N }
        {
            for (size_type i = 0; i < rows; i++)
            {
                // For Rows
                data.push_back(std::vector<value_type>());

                for (size_type j = 0; j < cols; j++)
                {
                    // For Cols
                    data[i].push_back(value);
                }
            }
        }

        Matrix(const value_type* pArr)
            : rows{ M }, cols{ N }
        {
            data.reserve(rows);
            int counter = 0;

            for (size_type i = 0; i < rows; i++)
            {
                data[i].reserve(cols);

                for (size_type j = 0; j < cols; j++)
                {
                    data[i][j] = pArr[counter];
                    counter++;
                }
            }
        }

        Matrix(value_type _00, value_type _01, value_type _02,
               value_type _10, value_type _11, value_type _12,
               value_type _20, value_type _21, value_type _22)
            : rows{ M }, cols{ N }
        {
            for (size_type i = 0; i < rows; i++)
            {
                data.push_back(std::vector<value_type>());
            }

            data[0].push_back(_00);
            data[0].push_back(_01);
            data[0].push_back(_02);
            data[1].push_back(_10);
            data[1].push_back(_11);
            data[1].push_back(_12);
            data[2].push_back(_20);
            data[2].push_back(_21);
            data[2].push_back(_22);
        }

        Matrix(value_type _00, value_type _01, value_type _02, value_type _03,
               value_type _10, value_type _11, value_type _12, value_type _13,
               value_type _20, value_type _21, value_type _22, value_type _23,
               value_type _30, value_type _31, value_type _32, value_type _33)
            : rows{ M }, cols{ N }
        {
            for (size_type i = 0; i < rows; i++)
            {
                data.push_back(std::vector<value_type>());
            }

            data[0].push_back(_00);
            data[0].push_back(_01);
            data[0].push_back(_02);
            data[0].push_back(_03);
            data[1].push_back(_10);
            data[1].push_back(_11);
            data[1].push_back(_12);
            data[1].push_back(_13);
            data[2].push_back(_20);
            data[2].push_back(_21);
            data[2].push_back(_22);
            data[2].push_back(_23);
            data[3].push_back(_30);
            data[3].push_back(_31);
            data[3].push_back(_32);
            data[3].push_back(_33);
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

        const std::vector<T>& operator[] (const size_t index) const
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

        void ConvertToGlmMat3Type()
        {
            glm::mat3 temp{};

            temp[0][0] = data[0][0];
            temp[0][1] = data[0][1];
            temp[0][2] = data[0][2];

            temp[1][0] = data[1][0];
            temp[1][1] = data[1][1];
            temp[1][2] = data[1][2];

            temp[2][0] = data[2][0];
            temp[2][1] = data[2][1];
            temp[2][2] = data[2][2];

            return temp;
        }

        void ConvertToGlmMat4Type()
        {
            glm::mat4 temp{};

            temp[0][0] = data[0][0];
            temp[0][1] = data[0][1];
            temp[0][2] = data[0][2];
            temp[0][3] = data[0][3];

            temp[1][0] = data[1][0];
            temp[1][1] = data[1][1];
            temp[1][2] = data[1][2];
            temp[1][3] = data[1][3];

            temp[2][0] = data[2][0];
            temp[2][1] = data[2][1];
            temp[2][2] = data[2][2];
            temp[2][3] = data[2][3];

            temp[3][0] = data[3][0];
            temp[3][1] = data[3][1];
            temp[3][2] = data[3][2];
            temp[3][3] = data[3][3];

            return temp;
        }

    private:
        size_type rows;
        size_type cols;
        std::vector<std::vector<T>> data;
    };

    typedef Matrix<float, 3, 3> ECMat3;
    typedef Matrix<float, 4, 4> ECMat4;

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

    // Matrix 3x3 Utilities
    void ECMtx33Identity(Matrix<float, 3, 3>& pResult);
    void ECMtx33Translate(Matrix<float, 3, 3>& pResult, float x, float y);
    void ECMtx33Scale(Matrix<float, 3, 3>& pResult, float x, float y);
    void ECMtx33RotRad(Matrix<float, 3, 3>& pResult, float angle);
    void ECMtx33RotDeg(Matrix<float, 3, 3>& pResult, float angle);
    void ECMtx33Transpose(Matrix<float, 3, 3>& pResult, const Matrix<float, 3, 3>& pMtx);
    float ECMtx33Determinant(const Matrix<float, 3, 3>& pMtx);
    void ECMtx33Inverse(Matrix<float, 3, 3>* pResult, const Matrix<float, 3, 3>& pMtx);

    // Matrix 4x4 Utilities
    void ECMtx44Identity(Matrix<float, 4, 4>& pResult);
    void ECMtx44Translate(Matrix<float, 4, 4>& pResult, float x, float y, float z);
    void ECMtx44Scale(Matrix<float, 4, 4>& pResult, float x, float y, float z);
    void ECMtx44RotRad(Matrix<float, 4, 4>& pResult, float angle);
    void ECMtx44RotDeg(Matrix<float, 4, 4>& pResult, float angle);
    void ECMtx44Transpose(Matrix<float, 4, 4>& pResult, const Matrix<float, 4, 4>& pMtx);
    float ECMtx44Determinant(const Matrix<float, 4, 4>& pMtx);
    void ECMtx44Inverse(Matrix<float, 4, 4>* pResult, const Matrix<float, 4, 4>& pMtx);

    // For Translation Matrices with Vectors
    ECVec2 operator*(const ECMat3& pMtx, const ECVec2& rhs);
    ECVec3 operator*(const ECMat4& pMtx, const ECVec3& rhs);
}
