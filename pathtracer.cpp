// usage (exename).exe >> (filename).ppm

#include <iostream>
#include <vector>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

class Vector
{
public:
	double x, y, z;

	Vector() : x(1), y(1), z(1) {}
	Vector(double x, double y, double z) : x(x), y(y), z(z) {}

	double& operator[](const int i) { return i==0  ? x : (i==1 ? y : z); }
	const double& operator[](const int i) const { return i==0  ? x : (i==1 ? y : z); }

	Vector operator~() const { return Vector(-x, -y, -z); }

	Vector operator+= (const Vector& v) { x += v.x; y += v.y; z += v.z; return *this; }
	Vector operator-= (const Vector& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	Vector operator*= (double s) { x *= s; y *= s; z *= s; return *this; }
	Vector operator/= (double s) { return *this *= 1/s; }

	Vector  operator*(const float v) const { return { x * v, y * v, z * v }; }
	double length() const { return sqrt(x*x + y*y + z*z); }
	Vector normalized() const { return (*this) * (1 / length()); }

};

std::ostream& operator<<(std::ostream &out, const Vector& v) { out << v.x << " " << v.y << " " << v.z; return out; }
Vector operator+(const Vector& a, const Vector& b) { return Vector(a.x + b.x, a.y + b.y, a.z + b.z); }
Vector operator-(const Vector& a, const Vector& b) { return Vector(a.x - b.x, a.y - b.y, a.z - b.z); }
Vector operator*(const Vector& a, const Vector& b) { return Vector(a.x * b.x, a.y * b.y, a.z * b.z); }
Vector operator*(const Vector& a, double b) { return Vector(a.x * b, a.y * b, a.z * b); }
Vector operator*(double a, const Vector& b) { return Vector(a * b.x, a * b.y, a * b.z); }
Vector operator/(const Vector& a, double b) { return (1/b) * a; }
double dot(const Vector& a, const Vector& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
Vector cross(const Vector& a, const Vector& b) { return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
Vector unit_vector(const Vector& v) { return v / v.length(); }
double clamp(double x, double min, double max) { return x < min ? min : (x > max ? max : x); }


using Color = Vector; 
using Point = Vector;

Vector reflect(const Vector& v, const Vector& n) { return v - 2 * dot(v, n) * n; }

void printColor(std::ostream& out, const Color& v)
{
	out << static_cast<int>(255 * v.x) << " " << static_cast<int>(255 * v.y) << " " << static_cast<int>(255 * v.z) << std::endl;
}

class Light
{
public:
	Vector position;
	double intensity;

	Light(const Vector& pos, double intense) : position(pos), intensity(intense) {}
};

class Material
{
public:
	double refractiveIndex;
	double albedo[4];
	Vector diffuseColor;
	double specularExponent;

	Material(double refr, double a, double ba, double c, double d, double r, double g, double b, double specExp) : refractiveIndex(refr), albedo{a,ba,c,d},diffuseColor(r, g, b), specularExponent(specExp) {}
	Material() : refractiveIndex(1), albedo{1,1,1,1}, diffuseColor(1,1,1), specularExponent(0) {}
};


class Sphere
{
public:
	Vector center;
	double radius;
	Material material;

	Sphere(const Vector& center, double radius, const Material &mat) : center(center), radius(radius), material(mat) {}
	Sphere(double x, double y, double z, double radius, const Material& mat) : center(x, y, z), radius(radius), material(mat) {}
	Sphere() : center(0, 0, 0), radius(1) {}

	bool rayIntersect(const Vector &orig, const Vector &direction, double &t) const
	{
		const Vector L = center - orig;
		const double tca = dot(L, direction);
		if (tca < 0) return false;
		const double d2 = dot(L, L) - tca * tca;
		if (d2 > radius * radius) return false;
		const double thc = sqrt(radius * radius - d2);
		double t0 = tca - thc;
		double t1 = tca + thc;

		if (t0 > t1) std::swap(t0, t1);

		if (t0 < 0)
		{
			t0 = t1; 
			if (t0 < 0) return false; 
		}

		t = t0;

		return true;
	}
};

Vector castRay(const Vector &orig, const Vector &dir, const std::vector<Sphere> &spheres, const std::vector<Light> &lights)
{
	Vector hit, N, point;
	double sphereDistance = std::numeric_limits<double>::max();
	for ( Sphere s : spheres)
	{
		double t;
		if (s.rayIntersect(orig, dir, t) && t < sphereDistance)
		{
			sphereDistance = t;
			hit = orig + dir * t;
			N = (hit - s.center).normalized();

			double diffuseLightIntensity = 0.0;
			double specularLightIntensity = 0.0;
			for ( Light l : lights)
			{
				Vector lightDirection = (l.position - point).normalized();

				diffuseLightIntensity += l.intensity * std::max(0.0, dot(lightDirection, N));
				specularLightIntensity += l.intensity * pow(std::max(0.0, dot(lightDirection, ~reflect(lightDirection, N))), s.material.specularExponent);

				
			}

			return s.material.diffuseColor * diffuseLightIntensity * s.material.albedo[0] + Vector(1., 1., 1.) * specularLightIntensity * s.material.albedo[1];
		}
	}

	return Vector(0.2, 0.7, 0.8);
}

int main()
{
	// image
	constexpr int imageWidth = 1024;
	constexpr int imageHeight = 768;

	Material ivory = Material(1.0, 0.9, 0.5, 0.1, 0.0, 0.4, 0.4, 0.3, 50);
	Material red_rubber = Material(1.0, 1.4, 0.3, 0.0, 0.0, 0.3, 0.1, 0.1, 10);



	

	std::cout << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

	std::vector<Sphere> spheres;
	spheres.push_back(Sphere(Vector(-3, 0, -16), 2, ivory));
	spheres.push_back(Sphere(Vector(-1.0, -1.5, -12), 2, red_rubber));
	spheres.push_back(Sphere(Vector(1.5, -0.5, -18), 3, red_rubber));
	spheres.push_back(Sphere(Vector(7, 5, -18), 4, ivory));

	std::vector<Light> lights;
	lights.push_back(Light(Vector(-20, 20, 20), 3.0));


	for (int j = imageHeight - 1; j >= 0; --j)
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

		for (int i = 0; i < imageWidth; ++i)
		{
			const double x = (2 * (i + 0.5) / imageWidth - 1) * imageWidth / imageHeight;
			const double y = (2 * (j + 0.5) / imageHeight - 1);
			Vector dir = unit_vector(Vector(x, y, -1));
			printColor(std::cout, castRay(Vector(0, 0, 0), dir, spheres,lights));
		}
		
	}


}


















