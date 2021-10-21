#include "pch.h"
#include "Matrix.h"

namespace Eclipse
{
    // Matrix 3x3 Utilities
    void ECMtx33Identity(Matrix<float, 3, 3>& pResult)
    {
        pResult[0][0] = 1.0f;
        pResult[0][1] = 0.0f;
        pResult[0][2] = 0.0f;

        pResult[1][0] = 0.0f;
        pResult[1][1] = 1.0f;
        pResult[1][2] = 0.0f;

        pResult[2][0] = 0.0f;
        pResult[2][1] = 0.0f;
        pResult[2][2] = 1.0f;
    }

    void ECMtx33Translate(Matrix<float, 3, 3>& pResult, float x, float y)
    {
        ECMtx33Identity(pResult);

        pResult[0][2] = x;
        pResult[1][2] = y;
    }

    void ECMtx33Scale(Matrix<float, 3, 3>& pResult, float x, float y)
    {
        ECMtx33Identity(pResult);

        pResult[0][0] = x;
        pResult[1][1] = y;
    }

    void ECMtx33RotRad(Matrix<float, 3, 3>& pResult, float angle)
    {
        ECMtx33Identity(pResult);

        pResult[0][0] = cosf(angle);
        pResult[0][1] = -(sinf(angle));
        pResult[1][0] = sinf(angle);
        pResult[1][1] = cosf(angle);
    }

    void ECMtx33RotDeg(Matrix<float, 3, 3>& pResult, float angle)
    {
        ECMtx33Identity(pResult);

        pResult[0][0] = cosf(angle * (PI / 180.0f));
        pResult[0][1] = -(sinf(angle * (PI / 180.0f)));
        pResult[1][0] = sinf(angle * (PI / 180.0f));
        pResult[1][1] = cosf(angle * (PI / 180.0f));
    }

    void ECMtx33Transpose(Matrix<float, 3, 3>& pResult, const Matrix<float, 3, 3>& pMtx)
    {
        Matrix<float, 3, 3> temp{ pMtx };

        std::swap(temp[0][1], temp[1][0]);
        std::swap(temp[0][2], temp[2][0]);
        std::swap(temp[1][2], temp[2][1]);

        pResult = temp;
    }

    float ECMtx33Determinant(const Matrix<float, 3, 3>& pMtx)
    {
        return (pMtx[0][0] * (pMtx[1][1] * pMtx[2][2] - pMtx[1][2] * pMtx[2][1]) -
               (pMtx[0][1] * (pMtx[1][0] * pMtx[2][2] - pMtx[1][2] * pMtx[2][0])) +
               (pMtx[0][2] * (pMtx[1][0] * pMtx[2][1] - pMtx[1][1] * pMtx[2][0])));
    }

    void ECMtx33Inverse(Matrix<float, 3, 3>* pResult, const Matrix<float, 3, 3>& pMtx)
    {
        Matrix<float, 3, 3> temp = pMtx;

        float determinant = ECMtx33Determinant(pMtx);

        if (abs(determinant) <= EPSILON)
        {
            pResult = nullptr;
            return;
        }
        else
        {
            ECMtx33Identity(temp);
            ECMtx33Transpose(temp, pMtx);

            pResult->operator[](0)[0] = (temp[1][1] * temp[2][2] - temp[1][2] * temp[2][1]) / determinant;
            pResult->operator[](0)[1] = (-(temp[1][0] * temp[2][2] - temp[1][2] * temp[2][0])) / determinant;
            pResult->operator[](0)[2] = (temp[1][0] * temp[2][1] - temp[1][1] * temp[2][0]) / determinant;

            pResult->operator[](1)[0] = (-(temp[0][1] * temp[2][2] - temp[0][2] * temp[2][1])) / determinant;
            pResult->operator[](1)[1] = (temp[0][0] * temp[2][2] - temp[0][2] * temp[2][0]) / determinant;
            pResult->operator[](1)[2] = (-(temp[0][0] * temp[2][1] - temp[0][1] * temp[2][0])) / determinant;

            pResult->operator[](2)[0] = (temp[0][1] * temp[1][2] - temp[0][2] * temp[1][1]) / determinant;
            pResult->operator[](2)[1] = (-(temp[0][0] * temp[1][2] - temp[0][2] * temp[1][0])) / determinant;
            pResult->operator[](2)[2] = (temp[0][0] * temp[1][1] - temp[0][1] * temp[1][0]) / determinant;
        }
    }

    // Matrix 4x4 Utilities
    void ECMtx44Identity(Matrix<float, 4, 4>& pResult)
    {
        pResult[0][0] = 1.0f;
        pResult[0][1] = 0.0f;
        pResult[0][2] = 0.0f;
        pResult[0][3] = 0.0f;

        pResult[1][0] = 0.0f;
        pResult[1][1] = 1.0f;
        pResult[1][2] = 0.0f;
        pResult[1][3] = 0.0f;

        pResult[2][0] = 0.0f;
        pResult[2][1] = 0.0f;
        pResult[2][2] = 1.0f;
        pResult[2][3] = 0.0f;

        pResult[3][0] = 0.0f;
        pResult[3][1] = 0.0f;
        pResult[3][2] = 0.0f;
        pResult[3][3] = 1.0f;
    }

    void ECMtx44Translate(Matrix<float, 4, 4>& pResult, float x, float y, float z)
    {
        ECMtx44Identity(pResult);

        pResult[0][2] = x;
        pResult[1][2] = y;
        pResult[2][2] = z;
    }

    void ECMtx44Scale(Matrix<float, 4, 4>& pResult, float x, float y, float z)
    {
        ECMtx44Identity(pResult);

        pResult[0][0] = x;
        pResult[1][1] = y;
        pResult[2][2] = z;
    }

    void ECMtx44RotRad(Matrix<float, 4, 4>& pResult, float angle)
    {
        ECMtx44Identity(pResult);

        pResult[0][0] = cosf(angle);
        pResult[0][1] = -(sinf(angle));
        pResult[1][0] = sinf(angle);
        pResult[1][1] = cosf(angle);
    }

    void ECMtx44RotDeg(Matrix<float, 4, 4>& pResult, float angle)
    {
        ECMtx44Identity(pResult);

        pResult[0][0] = cosf(angle * (PI / 180.0f));
        pResult[0][1] = -(sinf(angle * (PI / 180.0f)));
        pResult[1][0] = sinf(angle * (PI / 180.0f));
        pResult[1][1] = cosf(angle * (PI / 180.0f));
    }

    void ECMtx44Transpose(Matrix<float, 4, 4>& pResult, const Matrix<float, 4, 4>& pMtx)
    {
        Matrix<float, 4, 4> temp{ pMtx };

        std::swap(temp[0][1], temp[1][0]);
        std::swap(temp[0][2], temp[2][0]);
        std::swap(temp[0][3], temp[3][0]);

        std::swap(temp[1][2], temp[2][1]);
        std::swap(temp[1][3], temp[3][1]);

        std::swap(temp[2][3], temp[3][2]);

        pResult = temp;
    }

    float ECMtx44Determinant(const Matrix<float, 4, 4>& pMtx)
    {
        float first = pMtx[0][0] * (pMtx[1][1] * pMtx[2][2] * pMtx[3][3] -
                      pMtx[1][1] * pMtx[2][3] * pMtx[3][2] -
                      pMtx[2][1] * pMtx[1][2] * pMtx[3][3] +
                      pMtx[2][1] * pMtx[1][3] * pMtx[3][2] +
                      pMtx[3][1] * pMtx[1][2] * pMtx[2][3] -
                      pMtx[3][1] * pMtx[1][3] * pMtx[2][2]);

        float second = pMtx[0][1] * ((-pMtx[1][0]) * pMtx[2][2] * pMtx[3][3] +
                       pMtx[1][0] * pMtx[2][3] * pMtx[3][2] +
                       pMtx[2][0] * pMtx[1][2] * pMtx[3][3] -
                       pMtx[2][0] * pMtx[1][3] * pMtx[3][2] -
                       pMtx[3][0] * pMtx[1][2] * pMtx[2][3] +
                       pMtx[3][0] * pMtx[1][3] * pMtx[2][2]);

        float third = pMtx[0][2] *(pMtx[1][0] * pMtx[2][1] * pMtx[3][3] -
                      pMtx[1][0] * pMtx[2][3] * pMtx[3][1] -
                      pMtx[2][0] * pMtx[1][1] * pMtx[3][3] +
                      pMtx[2][0] * pMtx[1][3] * pMtx[3][1] +
                      pMtx[3][0] * pMtx[1][1] * pMtx[2][3] -
                      pMtx[3][0] * pMtx[1][3] * pMtx[2][1]);

        float fourth = pMtx[0][3] * ((-pMtx[1][0]) * pMtx[2][1] * pMtx[3][2] +
                       pMtx[1][0] * pMtx[2][2] * pMtx[3][1] +
                       pMtx[2][0] * pMtx[1][1] * pMtx[3][2] -
                       pMtx[2][0] * pMtx[1][2] * pMtx[3][1] -
                       pMtx[3][0] * pMtx[1][1] * pMtx[2][2] +
                       pMtx[3][0] * pMtx[1][2] * pMtx[2][1]);

        return first + second + third + fourth;
    }

    void ECMtx44Inverse(Matrix<float, 4, 4>* pResult, const Matrix<float, 4, 4>& pMtx)
    {
        Matrix<float, 4, 4> temp = pMtx;

        float determinant = ECMtx44Determinant(pMtx);

        if (abs(determinant) <= EPSILON)
        {
            pResult = nullptr;
            return;
        }
        else
        {
            pResult->operator[](0)[0] = (pMtx[1][1] * pMtx[2][2] * pMtx[3][3] -
                                         pMtx[1][1] * pMtx[2][3] * pMtx[3][2] -
                                         pMtx[2][1] * pMtx[1][2] * pMtx[3][3] +
                                         pMtx[2][1] * pMtx[1][3] * pMtx[3][2] +
                                         pMtx[3][1] * pMtx[1][2] * pMtx[2][3] -
                                         pMtx[3][1] * pMtx[1][3] * pMtx[2][2]) / determinant;
            pResult->operator[](1)[0] = ((-pMtx[1][0]) * pMtx[2][2] * pMtx[3][3] +
                                         pMtx[1][0] * pMtx[2][3] * pMtx[3][2] +
                                         pMtx[2][0] * pMtx[1][2] * pMtx[3][3] -
                                         pMtx[2][0] * pMtx[1][3] * pMtx[3][2] -
                                         pMtx[3][0] * pMtx[1][2] * pMtx[2][3] +
                                         pMtx[3][0] * pMtx[1][3] * pMtx[2][2]) / determinant;
            pResult->operator[](2)[0] = (pMtx[1][0] * pMtx[2][1] * pMtx[3][3] -
                                         pMtx[1][0] * pMtx[2][3] * pMtx[3][1] -
                                         pMtx[2][0] * pMtx[1][1] * pMtx[3][3] +
                                         pMtx[2][0] * pMtx[1][3] * pMtx[3][1] +
                                         pMtx[3][0] * pMtx[1][1] * pMtx[2][3] -
                                         pMtx[3][0] * pMtx[1][3] * pMtx[2][1]) / determinant;
            pResult->operator[](3)[0] = ((-pMtx[1][0]) * pMtx[2][1] * pMtx[3][2] +
                                         pMtx[1][0] * pMtx[2][2] * pMtx[3][1] +
                                         pMtx[2][0] * pMtx[1][1] * pMtx[3][2] -
                                         pMtx[2][0] * pMtx[1][2] * pMtx[3][1] -
                                         pMtx[3][0] * pMtx[1][1] * pMtx[2][2] +
                                         pMtx[3][0] * pMtx[1][2] * pMtx[2][1]) / determinant;

            pResult->operator[](0)[1] = ((-pMtx[0][1]) * pMtx[2][2] * pMtx[3][3] +
                                         pMtx[0][1] * pMtx[2][3] * pMtx[3][2] +
                                         pMtx[2][1] * pMtx[0][2] * pMtx[3][3] -
                                         pMtx[2][1] * pMtx[0][3] * pMtx[3][2] -
                                         pMtx[3][1] * pMtx[0][2] * pMtx[2][3] +
                                         pMtx[3][1] * pMtx[0][3] * pMtx[2][2]) / determinant;
            pResult->operator[](1)[1] = (pMtx[0][0] * pMtx[2][2] * pMtx[3][3] -
                                         pMtx[0][0] * pMtx[2][3] * pMtx[3][2] -
                                         pMtx[2][0] * pMtx[0][2] * pMtx[3][3] +
                                         pMtx[2][0] * pMtx[0][3] * pMtx[3][2] +
                                         pMtx[3][0] * pMtx[0][2] * pMtx[2][3] -
                                         pMtx[3][0] * pMtx[0][3] * pMtx[2][2]) / determinant;
            pResult->operator[](2)[1] = ((-pMtx[0][0]) * pMtx[2][1] * pMtx[3][3] +
                                         pMtx[0][0] * pMtx[2][3] * pMtx[3][1] +
                                         pMtx[2][0] * pMtx[0][1] * pMtx[3][3] -
                                         pMtx[2][0] * pMtx[0][3] * pMtx[3][1] -
                                         pMtx[3][0] * pMtx[0][1] * pMtx[2][3] +
                                         pMtx[3][0] * pMtx[0][3] * pMtx[2][1]) / determinant;
            pResult->operator[](3)[1] = (pMtx[0][0] * pMtx[2][1] * pMtx[3][2] -
                                         pMtx[0][0] * pMtx[2][2] * pMtx[3][1] -
                                         pMtx[2][0] * pMtx[0][1] * pMtx[3][2] +
                                         pMtx[2][0] * pMtx[0][2] * pMtx[3][1] +
                                         pMtx[3][0] * pMtx[0][1] * pMtx[2][2] -
                                         pMtx[3][0] * pMtx[0][2] * pMtx[2][1]) / determinant;

            pResult->operator[](0)[2] = (pMtx[0][1] * pMtx[1][2] * pMtx[3][3] -
                                         pMtx[0][1] * pMtx[1][3] * pMtx[3][2] -
                                         pMtx[1][1] * pMtx[0][2] * pMtx[3][3] +
                                         pMtx[1][1] * pMtx[0][3] * pMtx[3][2] +
                                         pMtx[3][1] * pMtx[0][2] * pMtx[1][3] -
                                         pMtx[3][1] * pMtx[0][3] * pMtx[1][2]) / determinant;
            pResult->operator[](1)[2] = ((-pMtx[0][0]) * pMtx[1][2] * pMtx[3][3] +
                                         pMtx[0][0] * pMtx[1][3] * pMtx[3][2] +
                                         pMtx[1][0] * pMtx[0][2] * pMtx[3][3] -
                                         pMtx[1][0] * pMtx[0][3] * pMtx[3][2] -
                                         pMtx[3][0] * pMtx[0][2] * pMtx[1][3] +
                                         pMtx[3][0] * pMtx[0][3] * pMtx[1][2]) / determinant;
            pResult->operator[](2)[2] = (pMtx[0][0] * pMtx[1][1] * pMtx[3][3] -
                                         pMtx[0][0] * pMtx[1][3] * pMtx[3][1] -
                                         pMtx[1][0] * pMtx[0][1] * pMtx[3][3] +
                                         pMtx[1][0] * pMtx[0][3] * pMtx[3][1] +
                                         pMtx[3][0] * pMtx[0][1] * pMtx[1][3] -
                                         pMtx[3][0] * pMtx[0][3] * pMtx[1][1]) / determinant;
            pResult->operator[](3)[2] = ((-pMtx[0][0]) * pMtx[1][1] * pMtx[3][2] +
                                         pMtx[0][0] * pMtx[1][2] * pMtx[3][1] +
                                         pMtx[1][0] * pMtx[0][1] * pMtx[3][2] -
                                         pMtx[1][0] * pMtx[0][2] * pMtx[3][1] -
                                         pMtx[3][0] * pMtx[0][1] * pMtx[1][2] +
                                         pMtx[3][0] * pMtx[0][2] * pMtx[1][1]) / determinant;

            pResult->operator[](0)[3] = ((-pMtx[0][1]) * pMtx[1][2] * pMtx[2][3] +
                                         pMtx[0][1] * pMtx[1][3] * pMtx[2][2] +
                                         pMtx[1][1] * pMtx[0][2] * pMtx[2][3] -
                                         pMtx[1][1] * pMtx[0][3] * pMtx[2][2] -
                                         pMtx[2][1] * pMtx[0][2] * pMtx[1][3] +
                                         pMtx[2][1] * pMtx[0][3] * pMtx[1][2]) / determinant;
            pResult->operator[](1)[3] = (pMtx[0][0] * pMtx[1][2] * pMtx[2][3] -
                                         pMtx[0][0] * pMtx[1][3] * pMtx[2][2] -
                                         pMtx[1][0] * pMtx[0][2] * pMtx[2][3] +
                                         pMtx[1][0] * pMtx[0][3] * pMtx[2][2] +
                                         pMtx[2][0] * pMtx[0][2] * pMtx[1][3] -
                                         pMtx[2][0] * pMtx[0][3] * pMtx[1][2]) / determinant;
            pResult->operator[](2)[3] = ((-pMtx[0][0]) * pMtx[1][1] * pMtx[2][3] +
                                         pMtx[0][0] * pMtx[1][3] * pMtx[2][1] +
                                         pMtx[1][0] * pMtx[0][1] * pMtx[2][3] -
                                         pMtx[1][0] * pMtx[0][3] * pMtx[2][1] -
                                         pMtx[2][0] * pMtx[0][1] * pMtx[1][3] +
                                         pMtx[2][0] * pMtx[0][3] * pMtx[1][1]) / determinant;
            pResult->operator[](3)[3] = (pMtx[0][0] * pMtx[1][1] * pMtx[2][2] -
                                         pMtx[0][0] * pMtx[1][2] * pMtx[2][1] -
                                         pMtx[1][0] * pMtx[0][1] * pMtx[2][2] +
                                         pMtx[1][0] * pMtx[0][2] * pMtx[2][1] +
                                         pMtx[2][0] * pMtx[0][1] * pMtx[1][2] -
                                         pMtx[2][0] * pMtx[0][2] * pMtx[1][1]) / determinant;
        }
    }

    ECVec2 operator*(const ECMat3& pMtx, const ECVec2& rhs)
    {
        ECVec2 temp{ rhs };

        temp[0] += pMtx[0][2];
        temp[1] += pMtx[1][2];

        return temp;
    }

    ECVec3 operator*(const ECMat4& pMtx, const ECVec3& rhs)
    {
        ECVec3 temp{ rhs };

        temp[0] += pMtx[0][3];
        temp[1] += pMtx[1][3];
        temp[2] += pMtx[2][3];

        return temp;
    }
}
