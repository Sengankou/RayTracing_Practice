#pragma once

#include "vec3.h"

// レイを表すクラス
class ray
{
public:
	ray() {}
	// レイの原点と方向を指定するコンストラクタ
	ray(const point3 &origin, const vec3 &direction) : orig(origin), dir(direction) {}

	point3 origin() const { return orig; } // レイの原点を返す関数
	vec3 direction() const { return dir; } // レイの方向を返す関数

	// レイの原点からパラメータt分だけ離れたの点を返す。実際の距離は、dir*tの長さ。
	point3 at(double t) const
	{
		return orig + dir * t;
	}

public:
	point3 orig; // レイの原点
	vec3 dir;	 // レイの方向
};