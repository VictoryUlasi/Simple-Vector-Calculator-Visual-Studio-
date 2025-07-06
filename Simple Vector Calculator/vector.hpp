#pragma once

class Vector2D
{
public:
    double x, y;

    Vector2D();
    Vector2D(double x, double y);

    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(double scalar) const;
    double dotProduct(const Vector2D& other) const;
    double magnitude() const;
    double angleBetween(const Vector2D& other) const;
    Vector2D normalize() const;
};

class Vector3D
{
public:
    double x, y, z;

    Vector3D();
    Vector3D(double x, double y, double z);

    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator*(double scalar) const;
    double dotProduct(const Vector3D& other) const;
    Vector3D crossProduct(const Vector3D& other) const; // Yields a vector that is orthogonal to the 2 Input Vectors
    double magnitude() const;
    double angleBetween(const Vector3D& other) const;
    Vector3D normalize() const;
};