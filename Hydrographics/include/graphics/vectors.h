#ifndef VECTORS_H
#define VECTORS_H

#include <cmath>

//----------------------VEC2--------------------------------
class vec2{
public:
    double x, y;

    vec2(double _x = 0, double _y = 0);
    vec2 operator+(vec2) const;
    vec2 operator+=(vec2);
    vec2 operator-(vec2) const;
    vec2 operator-=(vec2);
    vec2 operator-() const;
    vec2 operator*(double val) const;
    vec2 operator/(double val) const;

    double mod();
    vec2 normalize();
};
//----------------------------------------------------------

//----------------------VEC3--------------------------------
class vec3{
public:
    double x, y, z;

    vec3(double _x = 0, double _y = 0, double _z = 0);
    vec3 operator+(vec3) const;
    vec3 operator+=(vec3);
    vec3 operator-(vec3) const;
    vec3 operator-=(vec3);
    vec3 operator-() const;
    vec3 operator*(double val) const;
    vec3 operator*=(double val);
    vec3 operator/(double val) const;

    double mod();
    vec3 normalize();
};

vec3 ang_vec3(double _x, double _y);

double dot(vec3 a, vec3 b);
vec3 cross(vec3 a, vec3 b);
//----------------------------------------------------------

#endif //VECTORS_H
