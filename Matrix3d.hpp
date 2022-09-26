#ifndef MAT3_HPP
#define MAT3_HPP

#include "Vector2d.hpp"

/**
 * 3x3 matrix
 */
class Matrix3d
{
public:
    /// Initializes matrix with zeroes
    Matrix3d();
    Matrix3d(const double (&data)[3][3]);

    static Matrix3d rotationMatrix   (double angle);

    /// Applies transform to the vector
    Vector2d operator*(const Vector2d &vec);

    double data[3][3];
};

#endif