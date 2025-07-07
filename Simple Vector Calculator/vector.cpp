#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include <cmath>
#include <numbers>
#include "vector.hpp"

// 2D VECTORS
Vector2D::Vector2D() {};
Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

Vector2D Vector2D::operator+(const Vector2D& other) const
{
    Vector2D resultant(x + other.x, y + other.y);

    return resultant;
}

Vector2D Vector2D::operator-(const Vector2D& other) const
{
    Vector2D resultant(x - other.x, y - other.y);

    return resultant;
}

Vector2D Vector2D::operator*(double scalar) const
{
    Vector2D resultant(scalar * x, scalar * y);

    return resultant;
}

double Vector2D::dotProduct(const Vector2D& other) const
{
    double resultantScalar = (x * other.x) + (y * other.y);

    return resultantScalar;
}

double Vector2D::magnitude() const
{
    double resultantScalar = sqrt(this->dotProduct(*this)); // Fancier way to the the line below.
    // double resultantScalar = sqrt(pow(x, 2) + pow(y, 2));

    return resultantScalar;
}
double Vector2D::angleBetween(const Vector2D& other) const
{
    double resultantAngle = acos((this->dotProduct(other)) / ((this->magnitude()) * (other.magnitude())));
    return resultantAngle * (180 / M_PI); // convert from radians to degrees
}

Vector2D Vector2D::normalize() const
{
    double magnitude = this->magnitude();
    return Vector2D((x / magnitude), (y / magnitude)); //Implement no return, just do operation in function and return nothing so... this.x = x/magnitude etc...
}

// 3D VECTORS
Vector3D::Vector3D() {};
Vector3D::Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}

Vector3D Vector3D::operator+(const Vector3D& other) const
{
    Vector3D resultant(x + other.x, y + other.y, z + other.z);

    return resultant;
}

Vector3D Vector3D::operator-(const Vector3D& other) const
{
    Vector3D resultant(x - other.x, y - other.y, z - other.z);

    return resultant;
}

Vector3D Vector3D::operator*(double scalar) const
{
    Vector3D resultant(scalar * x, scalar * y, scalar * z);

    return resultant;
}

double Vector3D::dotProduct(const Vector3D& other) const
{
    double resultantScalar = (x * other.x) + (y * other.y) + (z * other.z);

    return resultantScalar;
}

Vector3D Vector3D::crossProduct(const Vector3D& other) const
{
    Vector3D resultant(((y * other.z) - (z * other.y)), ((z * other.x) - (x * other.z)), ((x * other.y) - (y * other.x)));

    return resultant;
}

double Vector3D::magnitude() const
{
    double resultantScalar = sqrt(this->dotProduct(*this)); // Fancier way to do the same thing below
    // double resultantScalar = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

    return resultantScalar;
}

double Vector3D::angleBetween(const Vector3D& other) const
{
    double resultantAngle = acos((this->dotProduct(other)) / ((this->magnitude()) * (other.magnitude())));
    return resultantAngle * (180 / M_PI);
}

Vector3D Vector3D::normalize() const
{
    double magnitude = this->magnitude();
    return Vector3D((x / magnitude), (y / magnitude), (z / magnitude));
}