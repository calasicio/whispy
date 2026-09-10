#pragma once

#include <cmath>

class Vector4
{
public:
  float x;
  float y;
  float z;
  float w;

  Vector4()
      : x(0.0f), y(0.0f), z(0.0f), w(1.0f)
  {
  }

  Vector4(float x, float y, float z, float w)
      : x(x), y(y), z(z), w(w)
  {
  }

  // Addition
  Vector4 operator+(const Vector4 &other) const
  {
    return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
  }

  Vector4 &operator+=(const Vector4 &other)
  {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
  }

  // Subtraction
  Vector4 operator-(const Vector4 &other) const
  {
    return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
  }

  Vector4 &operator-=(const Vector4 &other)
  {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
  }

  // Vector4 Multiplication (Hamilton Product)
  Vector4 operator*(const Vector4 &other) const
  {
    return Vector4(
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w,
        w * other.w - x * other.x - y * other.y - z * other.z);
  }

  Vector4 &operator*=(const Vector4 &other)
  {
    *this = *this * other;
    return *this;
  }

  // Scalar Multiplication
  Vector4 operator*(float scalar) const
  {
    return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
  }

  Vector4 &operator*=(float scalar)
  {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
  }

  // Scalar Division
  Vector4 operator/(float scalar) const
  {
    return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
  }

  Vector4 &operator/=(float scalar)
  {
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
  }

  // Negation
  Vector4 operator-() const
  {
    return Vector4(-x, -y, -z, -w);
  }

  // Length / Magnitude
  float length() const
  {
    return std::sqrt(x * x + y * y + z * z + w * w);
  }

  // Normalize in place
  Vector4 &normalize()
  {
    float magnitude = length();

    if (magnitude == 0.0f)
      return *this;

    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
    w /= magnitude;

    return *this;
  }

  // Return normalized copy
  Vector4 normalized() const
  {
    float magnitude = length();

    if (magnitude == 0.0f)
      return Vector4{0.0f, 0.0f, 0.0f, 1.0f};

    return Vector4{x / magnitude, y / magnitude, z / magnitude, w / magnitude};
  }

  // Conjugate
  Vector4 conjugate() const
  {
    return Vector4(-x, -y, -z, w);
  }

  // Dot product
  static float dot(const Vector4 &a, const Vector4 &b)
  {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
  }
};
