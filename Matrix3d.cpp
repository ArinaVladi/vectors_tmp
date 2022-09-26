#include <cstring>
#include <math.h>
#include "Matrix3d.hpp"

Matrix3d::Matrix3d()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            data[i][j] = 0;
}

Matrix3d::Matrix3d(const double (&data)[3][3])
{
    memcpy(this->data, data, 3 * 3 * sizeof(double));
}

Matrix3d Matrix3d::rotationMatrix(double angle)
{
    double s = sin(angle), c = cos(angle);

    return Matrix3d({ { c, -s, 0 },
                  { s,  c, 0 },
                  { 0,  0, 1 } });
}

Vector2d Matrix3d::operator*(const Vector2d &vec)
{
    return Vector2d(data[0][0] * vec.x + data[0][1] * vec.y + data[0][2],
                data[1][0] * vec.x + data[1][1] * vec.y + data[1][2]);
}
