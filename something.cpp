#include <iostream>
#include <vector>

class Vector3
{
public:
	double x, y, z;

	// Constructors
	Vector3() : x(0), y(0), z(0) {}
	Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
	Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

	Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }
	Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }
	Vector3 operator*(const double& d) const { return Vector3(x * d, y * d, z * d); }
	Vector3 operator/(const double& d) const { return Vector3(x / d, y / d, z / d); }

	// non const versions
	Vector3 operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector3 operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector3 operator*=(const double& d) { x *= d; y *= d; z *= d; return *this; }
	Vector3 operator/=(const double& d) { x /= d; y /= d; z /= d; return *this; }

	// alternative stuff
	Vector3 operator*(const Vector3& v) const { return Vector3(x * v.x, y * v.y, z * v.z); }
	Vector3 operator/(const Vector3& v) const { return Vector3(x / v.x, y / v.y, z / v.z); }

	Vector3 operator-() const { return Vector3(-x, -y, -z); }
	Vector3 operator+() const { return *this; }

	bool operator==(const Vector3& v) const { return x == v.x && y == v.y && z == v.z; }
	double length() const { return sqrt(x * x + y * y + z * z); }
	double norm() const { return x * x + y * y + z * z; }

	double dot(const Vector3& v) const
	{ return x * v.x + y * v.y + z * v.z; }
	Vector3 cross(const Vector3& v) const
	{ return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
};

std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

using color = Vector3;

class Vector2
{
public:
	double x, y;

	// constructors
	Vector2() : x(0), y(0) {}
	Vector2(double x, double y) : x(x), y(y) {}
	Vector2(const Vector2& v) : x(v.x), y(v.y) {}

	Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
	Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
	Vector2 operator*(const double& d) const { return Vector2(x * d, y * d); }
	Vector2 operator/(const double& d) const { return Vector2(x / d, y / d); }

	// non const versions
	Vector2 operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
	Vector2 operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
	Vector2 operator*=(const double& d) { x *= d; y *= d; return *this; }
	Vector2 operator/=(const double& d) { x /= d; y /= d; return *this; }

	// alternative stuff
	Vector2 operator*(const Vector2& v) const { return Vector2(x * v.x, y * v.y); }
	Vector2 operator/(const Vector2& v) const { return Vector2(x / v.x, y / v.y); }

	Vector2 operator-() const { return Vector2(-x, -y); }

	bool operator==(const Vector2& v) const { return x == v.x && y == v.y; }

};

std::ostream& operator<<(std::ostream& os, const Vector2& v)
{
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

int main()
{
	// test code
	Vector3 v1(1, 2, 3);
	std::cout << v1 << std::endl;
	Vector2 v3(1, 2);
	std::cout << v3 << std::endl;


}
