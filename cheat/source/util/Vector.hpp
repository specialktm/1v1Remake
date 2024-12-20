#pragma once
#include <math.h>
namespace cheat
{

	struct Vector3
	{
		float x;
		float y;
		float z;

	public:
		Vector3(void)
		{
			x = 0;
			y = 0;
			z = 0;
			Invalidate();
		}
		Vector3(float val)
		{
			x = val;
			y = val;
			z = val;
		}
		Vector3(float X, float Y, float Z)
		{
			x = X;
			y = Y;
			z = Z;
		}
		Vector3(const float* clr)
		{
			x = clr[0];
			y = clr[1];
			z = clr[2];
		}

		void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
		{
			x = ix; y = iy; z = iz;
		}
		bool IsValid() const
		{
			return true;
		}
		void Invalidate()
		{
			return;
		}

		float& operator[](int i)
		{
			return ((float*)this)[i];
		}
		float operator[](int i) const
		{
			return ((float*)this)[i];
		}

		Vector3 Zero()
		{
			Vector3 ret;
			ret.x = 0;
			ret.y = 0;
			ret.z = 0;
			return ret;
		}

		bool IsZero()
		{
			if (x == 0 && y == 0 && z == 0)
				return true;
			else
				return false;
		}

		bool operator==(const Vector3& src) const
		{
			return (src.x == x) && (src.y == y) && (src.z == z);
		}
		bool operator!=(const Vector3& src) const
		{
			return (src.x != x) || (src.y != y) || (src.z != z);
		}
		inline float Distance(const Vector3& Vector3)
		{
			return sqrt(
				(x - Vector3.x) * (x - Vector3.x) +
				(y - Vector3.y) * (y - Vector3.y) +
				(z - Vector3.z) * (z - Vector3.z));
		}
		Vector3& operator+=(const Vector3& v)
		{
			x += v.x; y += v.y; z += v.z;
			return *this;
		}
		Vector3& operator-=(const Vector3& v)
		{
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}
		Vector3& operator*=(float fl)
		{
			x *= fl;
			y *= fl;
			z *= fl;
			return *this;
		}
		Vector3& operator*=(const Vector3& v)
		{
			x *= v.x;
			y *= v.y;
			z *= v.z;
			return *this;
		}
		Vector3& operator/=(const Vector3& v)
		{
			x /= v.x;
			y /= v.y;
			z /= v.z;
			return *this;
		}
		Vector3& operator+=(float fl)
		{
			x += fl;
			y += fl;
			z += fl;
			return *this;
		}
		Vector3& operator/=(float fl)
		{
			x /= fl;
			y /= fl;
			z /= fl;
			return *this;
		}
		Vector3& operator-=(float fl)
		{
			x -= fl;
			y -= fl;
			z -= fl;
			return *this;
		}

		void Clamp()
		{
			if (this->x > 180.f)
				this->x -= 360.f;

			else if (this->x < -180.f)
				this->x += 360.f;

			if (this->z > 180.f)
				this->z -= 360.f;

			else if (this->z < -180.f)
				this->z += 360.f;

			if (this->x < -89.f)
				this->x = -89.f;

			if (this->x > 89.f)
				this->x = 89.f;

			while (this->z < -180.0f)
				this->z += 360.0f;

			while (this->z > 180.0f)
				this->z -= 360.0f;
		}

		void NormalizeInPlace()
		{
			*this = Normalized();
		}
		Vector3 Normalized() const
		{
			Vector3 res = *this;
			float l = res.Length();
			if (l != 0.0f) {
				res /= l;
			}
			else {
				res.x = res.y = res.z = 0.0f;
			}
			return res;
		}

		float DistTo(const Vector3& vOther) const
		{
			Vector3 delta;

			delta.x = x - vOther.x;
			delta.y = y - vOther.y;
			delta.z = z - vOther.z;

			return delta.Length();
		}
		float DistToSqr(const Vector3& vOther) const
		{
			Vector3 delta;

			delta.x = x - vOther.x;
			delta.y = y - vOther.y;
			delta.z = z - vOther.z;

			return delta.LengthSqr();
		}
		float Dot(const Vector3& vOther) const
		{
			return (x * vOther.x + y * vOther.y + z * vOther.z);
		}
		float Length() const
		{
			return sqrt(x * x + y * y + z * z);
		}
		float LengthSqr(void) const
		{
			return (x * x + y * y + z * z);
		}
		float Length2D() const
		{
			return sqrt(x * x + y * y);
		}
		float deltaDistance(Vector3 target_position) const
		{
			return sqrt(
				pow(x - target_position.x, 2) +
				pow(y - target_position.y, 2) +
				pow(z - target_position.z, 2));
		}

		Vector3& operator=(const Vector3& vOther)
		{
			x = vOther.x; y = vOther.y; z = vOther.z;
			return *this;
		}

		Vector3 operator-(void) const
		{
			return Vector3(-x, -y, -z);
		}
		Vector3 operator+(const Vector3& v) const
		{
			return Vector3(x + v.x, y + v.y, z + v.z);
		}
		Vector3 operator-(const Vector3& v) const
		{
			return Vector3(x - v.x, y - v.y, z - v.z);
		}
		Vector3 operator*(float fl) const
		{
			return Vector3(x * fl, y * fl, z * fl);
		}
		Vector3 operator*(const Vector3& v) const
		{
			return Vector3(x * v.x, y * v.y, z * v.z);
		}
		Vector3 operator/(float fl) const
		{
			return Vector3(x / fl, y / fl, z / fl);
		}
		Vector3 operator/(const Vector3& v) const
		{
			return Vector3(x / v.x, y / v.y, z / v.z);
		}
		inline Vector3 Normalize()
		{
			Vector3 Vector3;
			float length = this->Length();

			if (length != 0) {
				Vector3.x = x / length;
				Vector3.y = y / length;
				Vector3.z = z / length;
			}
			else
				Vector3.x = Vector3.y = 0.0f; Vector3.z = 1.0f;

			return Vector3;
		}
		inline float Normalizes()
		{
			Vector3 res = *this;
			float l = res.Length();
			if (l != 0.0f)
			{
				res /= l;
			}
			else
			{
				res.x = res.y = res.z = 0.0f;
			}
			return l;
		}
	};

	inline Vector3 operator*(float lhs, const Vector3& rhs)
	{
		return rhs * lhs;
	}
	inline Vector3 operator/(float lhs, const Vector3& rhs)
	{
		return rhs / lhs;
	}

	class __declspec(align(16)) Vector3Aligned : public Vector3
	{
	public:
		inline Vector3Aligned(void) {};
		inline Vector3Aligned(float X, float Y, float Z)
		{
			Init(X, Y, Z);
		}

	public:
		explicit Vector3Aligned(const Vector3& vOther)
		{
			Init(vOther.x, vOther.y, vOther.z);
		}

		Vector3Aligned& operator=(const Vector3& vOther)
		{
			Init(vOther.x, vOther.y, vOther.z);
			return *this;
		}

		Vector3Aligned& operator=(const Vector3Aligned& vOther)
		{
			Init(vOther.x, vOther.y, vOther.z);
			return *this;
		}

		float w;
	};

	struct Vector4
	{
	public:
		float x;
		float y;
		float z;
		float w;
		Vector4 Zero();
	};

	struct Matrix3x4
	{
		float m[3][4];
	};

	struct Matrix4x3
	{
		float m[4][3];
	};

	struct Matrix4x4
	{
		union { Vector4 v[4]; float m[4][4]; struct { Vector4 right; Vector4 up; Vector4 forward; Vector4 trans; }; };
	};

	typedef float vec_t;
	// 2D Vector
	class Vector2
	{
	public:
		// Members
		float x, y;

		// Construction/destruction:
		Vector2(void);
		Vector2(vec_t X, vec_t Y);
		Vector2(vec_t* clr);

		Vector2(const Vector2& vOther)
		{
			x = vOther.x; y = vOther.y;
		}

		// Initialization
		void Init(vec_t ix = 0.0f, vec_t iy = 0.0f);
		// TODO (Ilya): Should there be an init that takes a single float for consistency?

		// Got any nasty NAN's?
		bool IsValid() const;
		void Invalidate();

		// array access...
		vec_t operator[](int i) const;
		vec_t& operator[](int i);

		// Base address...
		vec_t* Base();
		vec_t const* Base() const;

		// Initialization methods
		void Random(vec_t minVal, vec_t maxVal);
		void Zero(); ///< zero out a vector

		// equality
		bool operator==(const Vector2& v) const;
		bool operator!=(const Vector2& v) const;

		// arithmetic operations
		Vector2& operator+=(const Vector2& v)
		{
			x += v.x; y += v.y;
			return *this;
		}

		Vector2& operator-=(const Vector2& v)
		{
			x -= v.x; y -= v.y;
			return *this;
		}

		Vector2& operator*=(float fl)
		{
			x *= fl;
			y *= fl;
			return *this;
		}

		Vector2& operator*=(const Vector2& v)
		{
			x *= v.x;
			y *= v.y;
			return *this;
		}

		Vector2& operator/=(const Vector2& v)
		{
			x /= v.x;
			y /= v.y;
			return *this;
		}

		// this ought to be an opcode.
		Vector2& operator+=(float fl)
		{
			x += fl;
			y += fl;
			return *this;
		}

		// this ought to be an opcode.
		Vector2& operator/=(float fl)
		{
			x /= fl;
			y /= fl;
			return *this;
		}
		Vector2& operator-=(float fl)
		{
			x -= fl;
			y -= fl;
			return *this;
		}

		// negate the vector components
		void Negate();

		// Get the vector's magnitude.
		vec_t Length() const;

		// Get the vector's magnitude squared.
		vec_t LengthSqr(void) const
		{
			return (x * x + y * y);
		}
		void Clamp();
		// return true if this vector is (0,0,0) within tolerance
		bool IsZero(float tolerance = 0.01f) const
		{
			return (x > -tolerance && x < tolerance &&
				y > -tolerance && y < tolerance);
		}

		vec_t NormalizeInPlace();
		Vector2 Normalized() const;
		bool IsLengthGreaterThan(float val) const;
		bool IsLengthLessThan(float val) const;

		// check if a vector is within the box defined by two other vectors
		bool WithinAABox(Vector2 const& boxmin, Vector2 const& boxmax);

		// Get the distance from this vector to the other one.
		vec_t DistTo(const Vector2& vOther) const;

		// Get the distance from this vector to the other one squared.
		// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' .  
		// may be able to tidy this up after switching to VC7
		vec_t DistToSqr(const Vector2& vOther) const
		{
			Vector2 delta;

			delta.x = x - vOther.x;
			delta.y = y - vOther.y;

			return delta.LengthSqr();
		}

		// Copy
		void CopyToArray(float* rgfl) const;

		// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
		// is about 12% faster than the actual vector equation (because it's done per-component
		// rather than per-vector).
		void MulAdd(const Vector2& a, const Vector2& b, float scalar);

		// Dot product.
		vec_t Dot(const Vector2& vOther) const;

		// assignment
		Vector2& operator=(const Vector2& vOther);

		// 2d
		vec_t Length2D(void) const;
		vec_t Length2DSqr(void) const;

		/// Get the component of this vector parallel to some other given vector
		Vector2  ProjectOnto(const Vector2& onto);

		// copy constructors
		// Vector2(const Vector2 &vOther);

		// arithmetic operations
		Vector2 operator-(void) const;

		Vector2 operator+(const Vector2& v) const;
		Vector2 operator-(const Vector2& v) const;
		Vector2 operator*(const Vector2& v) const;
		Vector2 operator/(const Vector2& v) const;
		Vector2 operator*(float fl) const;
		Vector2 operator/(float fl) const;

		// Cross product between two vectors.
		Vector2 Cross(const Vector2& vOther) const;

		// Returns a vector with the min or max in X, Y, and Z.
		Vector2 Min(const Vector2& vOther) const;
		Vector2 Max(const Vector2& vOther) const;
	};
}