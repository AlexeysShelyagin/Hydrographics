#include "vectors.h"

//----------------------VEC2--------------------------------
vec2::vec2(double _x, double _y){
    x = _x;
    y = _y;
}

vec2 vec2::operator+ (vec2 b) const{
    return vec2(
            x + b.x,
            y + b.y
    );
}

vec2 vec2::operator+= (vec2 b){
    return vec2(
            x += b.x,
            y += b.y
    );
}

vec2 vec2::operator- (vec2 b) const{
    return vec2(
            x - b.x,
            y - b.y
    );
}

vec2 vec2::operator-= (vec2 b){
    return vec2(
            x -= b.x,
            y -= b.y
    );
}

vec2 vec2::operator- () const{
    return vec2(
            -x,
            -y
    );
}

vec2 vec2::operator* (double val) const{
    return vec2(
            x * val,
            y * val
    );
}

vec2 vec2::operator/ (double val) const{
    return vec2(
            x / val,
            y / val
    );
}

double vec2::mod() {
    return sqrt(x*x + y*y);
}

vec2 vec2::normalize() {
    double mod = 1 / this -> mod();
    return vec2(
            x * mod,
            y * mod
    );
}
//----------------------------------------------------------

//----------------------VEC3--------------------------------
vec3::vec3(double _x, double _y, double _z){
    x = _x;
    y = _y;
    z = _z;
}

vec3 vec3::operator+ (vec3 b) const{
    return vec3(
            x + b.x,
            y + b.y,
            z + b.z
            );
}

vec3 vec3::operator+= (vec3 b){
    return vec3(
            x += b.x,
            y += b.y,
            z += b.z
    );
}

vec3 vec3::operator- (vec3 b) const{
    return vec3(
            x - b.x,
            y - b.y,
            z - b.z
    );
}

vec3 vec3::operator-= (vec3 b){
    return vec3(
            x -= b.x,
            y -= b.y,
            z -= b.z
    );
}

vec3 vec3::operator- () const{
    return vec3(
            -x,
            -y,
            -z
    );
}

vec3 vec3::operator* (double val) const{
    return vec3(
            x * val,
            y * val,
            z * val
    );
}

vec3 vec3::operator*= (double val){
    return vec3(
            x *= val,
            y *= val,
            z *= val
    );
}

vec3 vec3::operator/ (double val) const{
    return vec3(
            x / val,
            y / val,
            z / val
    );
}

double vec3::mod() {
    return sqrt(x*x + y*y + z*z);
}

vec3 vec3::normalize() {
    double mod = 1 / this -> mod();
    return vec3(
            x * mod,
            y * mod,
            z * mod
            );
}

vec3 ang_vec3(double _x, double _y) {
    return vec3(
        cos(_x) * sin(_y),
        sin(_x) * sin(_y),
        cos(_y)
    );
}

double dot(vec3 a, vec3 b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 cross(vec3 a, vec3 b){
    return vec3(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
    );
}
//----------------------------------------------------------