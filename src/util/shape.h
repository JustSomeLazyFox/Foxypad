#pragma once

#include "vector.h"

enum Pivote {
  CORNER_TOP_LEFT,
  CORNER_TOP_RIGHT,
  CORNER_BOTTOM_LEFT,
  CORNER_BOTTOM_RIGHT,
  CENTER,
};

class Shape {
public:
  Vector2D position;
  Vector2D dimensions;
  Pivote pivote;

  Shape() : pivote(CORNER_TOP_LEFT) {}
  Shape(const Vector2D &position, const Vector2D &dimensions, Pivote pivote = CORNER_TOP_LEFT)
      : position(position), dimensions(dimensions), pivote(pivote) {}

  virtual Vector2D getPosition() const = 0;
  virtual Vector2D getDimensions() const = 0;
  virtual bool contains(const Vector2D &point) const = 0;
  virtual std::string toString() const = 0;
  virtual ~Shape() = default;
};

class Rectangle : public Shape {
public:
  Rectangle() {}
  Rectangle(const Vector2D &position, const Vector2D &dimensions, Pivote pivote = CORNER_TOP_LEFT) : Shape(position, dimensions, pivote) {}

  Vector2D getPosition() const {
    switch (pivote) {
    case CORNER_TOP_LEFT:
      return position;
    case CORNER_TOP_RIGHT:
      return Vector2D(position.getX() - dimensions.getX(), position.getY());
    case CORNER_BOTTOM_LEFT:
      return Vector2D(position.getX(), position.getY() - dimensions.getY());
    case CORNER_BOTTOM_RIGHT:
      return Vector2D(position.getX() - dimensions.getX(), position.getY() - dimensions.getY());
    case CENTER:
      return Vector2D(position.getX() - dimensions.getX() / 2, position.getY() - dimensions.getY() / 2);
    }
    return Vector2D();
  }

  Vector2D getDimensions() const { return dimensions; }

  bool contains(const Vector2D &point) const {
    Vector2D position = getPosition();
    return (point.getX() >= position.getX()) && (point.getX() <= (position.getX() + dimensions.getX())) && (point.getY() >= position.getY()) &&
           (point.getY() <= (position.getY() + dimensions.getY()));
  }

  std::string toString() const {
    return "Rectangle(position: " + position.toString() + ", dimensions: " + dimensions.toString() + ", pivote: " + std::to_string(pivote) + ")";
  }
};

class Circle : public Shape {
public:
  float radius;

  Circle() {}
  Circle(const Vector2D &position, float radius) : Shape(position, Vector2D(radius * 2, radius * 2), CENTER) { this->radius = radius; }

  Vector2D getPosition() const {
    switch (pivote) {
    case CENTER:
      return position;
    default:
      return Vector2D(position.getX() - dimensions.getX() / 2, position.getY() - dimensions.getY() / 2);
    }
  }

  Vector2D getDimensions() const { return dimensions; }

  bool contains(const Vector2D &point) const { return point.distanceTo(position) <= radius; }

  std::string toString() const { return "Circle(position: " + position.toString() + ", radius: " + std::to_string(radius) + ")"; }
};
