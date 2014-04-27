// vector_3d.h is part of Noggit3, licensed via GNU General Publiicense (version 3).
// Bernd Lrwald <bloerwald+noggit@googlemail.com>
// Stephan Biegel <project.modcraft@googlemail.com>
// Tigurius <bstigurius@googlemail.com>

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>


class vector3D
{
public:
    vector3D (float x = 0.0f, float y = 0.0f, float z = 0.0f)
        : _x (x)
        , _y (y)
        , _z (z)
    {}

    vector3D (const vector3D& v)
        : _x (v._x)
        , _y (v._y)
        , _z (v._z)
    {}

    inline vector3D& operator= (const vector3D &v)
    {
        _x = v._x;
        _y = v._y;
        _z = v._z;
        return *this;
    }

    const float& x() const
    {
        return _x;
    }
    const float& y() const
    {
        return _y;
    }
    const float& z() const
    {
        return _z;
    }
    float& x()
    {
        return _x;
    }
    float& y()
    {
        return _y;
    }
    float& z()
    {
        return _z;
    }

    const float& x (const float& x_)
    {
        return _x = x_;
    }
    const float& y (const float& y_)
    {
        return _y = y_;
    }
    const float& z (const float& z_)
    {
        return _z = z_;
    }

    inline vector3D operator+ (const vector3D &v) const
    {
        return vector3D (_x + v._x, _y + v._y, _z + v._z);
    }

    inline vector3D operator- (const vector3D &v) const
    {
        return vector3D (_x - v._x, _y - v._y, _z - v._z);
    }

    inline float operator* (const vector3D &v) const
    {
        return _x * v._x + _y * v._y + _z * v._z;
    }

    inline vector3D operator* (const float& d) const
    {
        return vector3D (_x * d, _y * d, _z * d);
    }

    friend vector3D operator* (const float& d, const vector3D& v)
    {
        return v * d;
    }

    inline vector3D operator% (const vector3D& v) const
    {
        return vector3D ( _y * v._z - _z * v._y
                           , _z * v._x - _x * v._z
                           , _x * v._y - _y * v._x
                           );
    }

    inline vector3D& operator+= (const vector3D& v)
    {
        _x += v._x;
        _y += v._y;
        _z += v._z;
        return *this;
    }

    inline vector3D& operator-= (const vector3D& v)
    {
        _x -= v._x;
        _y -= v._y;
        _z -= v._z;
        return *this;
    }

    inline vector3D& operator*= (float d)
    {
        _x *= d;
        _y *= d;
        _z *= d;
        return *this;
    }

    inline float length_squared() const
    {
        return _x * _x + _y * _y + _z * _z;
    }

    inline float length() const
    {
        return sqrtf (length_squared());
    }

    inline vector3D& normalize()
    {
        const float repri (1.0f / length());
        _x *= repri;
        _y *= repri;
        _z *= repri;
        return *this;
    }

    inline operator float*()
    {
        return _data;
    }

    inline operator const float*() const
    {
        return _data;
    }

    inline bool is_inside_of (const vector3D& a, const vector3D& b ) const
    {
        return a._x < _x && b._x > _x
                && a._y < _y && b._y > _y
                && a._z < _z && b._z > _z;
    }

private:
    union
    {
        float _data[3];
        struct
        {
            float _x;
            float _y;
            float _z;
        };
    };
};


#endif // VECTOR3D_H
