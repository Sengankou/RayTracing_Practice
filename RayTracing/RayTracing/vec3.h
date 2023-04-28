#pragma once
#pragma once

#include <cmath>
#include <iostream>

// 3次元ベクトルを表すクラス

using std::sqrt;

class vec3
{
public:
	vec3() : e{0, 0, 0} {}
	vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	// オペレーターのオーバーロード
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](int i) const { return e[i]; }
	double &operator[](int i) { return e[i]; }

	vec3 &operator+=(const vec3 &v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3 &operator*=(const double t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3 &operator/=(const double t)
	{
		return *this *= 1 / t;
	}

	double length() const
	{
		return sqrt(length_squared());
	}
	double length_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	inline static vec3 random()
	{
		return vec3(random_double(), random_double(), random_double());
	}
	inline static vec3 random(double min, double max)
	{
		return vec3(random_double(min, max),
					random_double(min, max),
					random_double(min, max));
	}

public:
	double e[3];
};

// vec3の出力を定義
inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// vec3同士の演算
inline vec3 operator+(const vec3 &u, const vec3 &v)
{
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v)
{
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v)
{
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v)
{
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t)
{
	return t * v;
}

inline vec3 operator/(vec3 v, double t)
{
	return (1 / t) * v;
}

// 内積を返す
inline double dot(const vec3 &u, const vec3 &v)
{
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

// 外積を返す
inline vec3 cross(const vec3 &u, const vec3 &v)
{
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1], u.e[2] * v.e[0] - u.e[0] * v.e[2], u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// 単位ベクトルを返す関数
inline vec3 unit_vector(vec3 v)
{
	return v / v.length();
}

// 単位球内のランダムな点を生成する関数
vec3 random_in_unit_sphere()
{
	while (true)
	{
		// 単位立方体内のランダムな点を生成する
		auto p = vec3::random(-1, 1);
		// 生成された点が単位球の外側にある場合、ループを継続する
		if (p.length_squared() >= 1)
			continue;
		// 単位球の内側にある場合、その点を返す
		return p;
	}
}

// ランダムな単位ベクトルを生成する関数
vec3 random_unit_vector()
{
	auto a = random_double(0, 2 * pi); // 0から2πまでのランダムな角度を生成する
	auto z = random_double(-1, 1);	   // -1から1までの間のランダムな値を生成する
	auto r = sqrt(1 - z * z);		   // 高さzで切った原点中心の単位球の半径rを計算する
	return vec3(r * cos(a), r * sin(a), z);
}

vec3 random_in_hemisphere(const vec3 &normal)
{
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0)
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

// 単位円盤上にランダムな点を生成する関数
// 焦点ボケを再現するときに用いる
vec3 random_in_unit_disk()
{
	while (true)
	{
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1)
			continue;
		return p;
	}
}

// 入射ベクトルvを受け取り、反射ベクトルを返す。nは法線ベクトル
vec3 reflect(const vec3 &v, const vec3 &n)
{
	return v - 2 * (dot(v, n) * n);
}

// 入射ベクトルuvを受け取り、スネルの法則に従う屈折ベクトルを返す
// nは法線ベクトル
vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat)
{
	auto cos_theta = dot(-uv, n);
	vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);		// 法線ベクトルと平行な成分
	vec3 r_out_perp = -sqrt(1.0 - r_out_parallel.length_squared()) * n; // 法線ベクトルと垂直な成分
	return r_out_parallel + r_out_perp;
}

// vec3 の型エイリアス
using point3 = vec3; // 3D point
using color = vec3;	 // RGB color