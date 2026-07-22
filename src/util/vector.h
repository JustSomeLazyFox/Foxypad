#pragma once

#include <cmath>
#include <string>
class Vector2D {
public:
  int x, y;
  Vector2D() : x(0), y(0) {}
  Vector2D(int x, int y) : x(x), y(y) {}
  Vector2D(int both) : x(both), y(both) {}

  void setX(int x) { this->x = x; }
  void setY(int y) { this->y = y; }
  void setXY(int x, int y) {
    this->x = x;
    this->y = y;
  }
  void setBoth(int both) {
    this->x = both;
    this->y = both;
  }
  void moveX(int dx) { this->x += dx; }
  void moveY(int dy) { this->y += dy; }
  void moveXY(int dx, int dy) {
    this->x += dx;
    this->y += dy;
  }
  void moveBoth(int d) {
    this->x += d;
    this->y += d;
  }

  int getX() const { return x; }
  int getY() const { return y; }

  Vector2D operator+(const Vector2D &other) const { return Vector2D(x + other.x, y + other.y); }
  Vector2D operator-(const Vector2D &other) const { return Vector2D(x - other.x, y - other.y); }
  Vector2D operator*(const Vector2D &other) const { return Vector2D(x * other.x, y * other.y); }
  Vector2D operator/(const Vector2D &other) const { return Vector2D(x / other.x, y / other.y); }

  bool operator==(const Vector2D &other) const { return x == other.x && y == other.y; }
  bool operator!=(const Vector2D &other) const { return !(*this == other); }

  Vector2D operator+(int value) const { return Vector2D(x + value, y + value); }
  Vector2D operator-(int value) const { return Vector2D(x - value, y - value); }
  Vector2D operator*(int value) const { return Vector2D(x * value, y * value); }
  Vector2D operator/(int value) const { return Vector2D(x / value, y / value); }

  Vector2D &operator+=(const Vector2D &other) {
    x += other.x;
    y += other.y;
    return *this;
  }
  Vector2D &operator-=(const Vector2D &other) {
    x -= other.x;
    y -= other.y;
    return *this;
  }
  Vector2D &operator*=(const Vector2D &other) {
    x *= other.x;
    y *= other.y;
    return *this;
  }
  Vector2D &operator/=(const Vector2D &other) {
    x /= other.x;
    y /= other.y;
    return *this;
  }

  Vector2D &operator+=(int value) {
    x += value;
    y += value;
    return *this;
  }
  Vector2D &operator-=(int value) {
    x -= value;
    y -= value;
    return *this;
  }
  Vector2D &operator*=(int value) {
    x *= value;
    y *= value;
    return *this;
  }
  Vector2D &operator/=(int value) {
    x /= value;
    y /= value;
    return *this;
  }

  float distanceTo(const Vector2D &other) const { return (float)std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y)); }

  std::string toString() const { return "Vector2D<" + std::to_string(x) + ", " + std::to_string(y) + ">"; }
};

class Vector3D {
public:
  int x, y, z;
  Vector3D() : x(0), y(0), z(0) {}
  Vector3D(int x, int y, int z) : x(x), y(y), z(z) {}
  Vector3D(int both) : x(both), y(both), z(both) {}

  void setX(int x) { this->x = x; }
  void setY(int y) { this->y = y; }
  void setZ(int z) { this->z = z; }
  void setXYZ(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  void setBoth(int both) {
    this->x = both;
    this->y = both;
    this->z = both;
  }
  void moveX(int dx) { this->x += dx; }
  void moveY(int dy) { this->y += dy; }
  void moveZ(int dz) { this->z += dz; }
  void moveXYZ(int dx, int dy, int dz) {
    this->x += dx;
    this->y += dy;
    this->z += dz;
  }
  void moveBoth(int d) {
    this->x += d;
    this->y += d;
    this->z += d;
  }

  int getX() const { return x; }
  int getY() const { return y; }
  int getZ() const { return z; }

  Vector3D operator+(const Vector3D &other) const { return Vector3D(x + other.x, y + other.y, z + other.z); }
  Vector3D operator-(const Vector3D &other) const { return Vector3D(x - other.x, y - other.y, z - other.z); }
  Vector3D operator*(const Vector3D &other) const { return Vector3D(x * other.x, y * other.y, z * other.z); }
  Vector3D operator/(const Vector3D &other) const { return Vector3D(x / other.x, y / other.y, z / other.z); }

  bool operator==(const Vector3D &other) const { return x == other.x && y == other.y && z == other.z; }
  bool operator!=(const Vector3D &other) const { return !(*this == other); }

  Vector3D operator+(int value) const { return Vector3D(x + value, y + value, z + value); }
  Vector3D operator-(int value) const { return Vector3D(x - value, y - value, z - value); }
  Vector3D operator*(int value) const { return Vector3D(x * value, y * value, z * value); }
  Vector3D operator/(int value) const { return Vector3D(x / value, y / value, z / value); }

  Vector3D &operator+=(const Vector3D &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }
  Vector3D &operator-=(const Vector3D &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
  }
  Vector3D &operator*=(const Vector3D &other) {
    x *= other.x;
    y *= other.y;
    z *= other.z;
    return *this;
  }
  Vector3D &operator/=(const Vector3D &other) {
    x /= other.x;
    y /= other.y;
    z /= other.z;
    return *this;
  }

  Vector3D &operator+=(int value) {
    x += value;
    y += value;
    z += value;
    return *this;
  }
  Vector3D &operator-=(int value) {
    x -= value;
    y -= value;
    z -= value;
    return *this;
  }
  Vector3D &operator*=(int value) {
    x *= value;
    y *= value;
    z *= value;
    return *this;
  }
  Vector3D &operator/=(int value) {
    x /= value;
    y /= value;
    z /= value;
    return *this;
  }

  float distanceTo(const Vector3D &other) const {
    return (float)std::sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) + (z - other.z) * (z - other.z));
  }

  std::string toString() const { return "Vector3D<" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ">"; }
};