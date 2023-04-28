#pragma once
#include "ray.h"

// hittable というレイが衝突しうるオブジェクトを表す一般的なクラスを定義する
// このクラスは、レイがオブジェクトに衝突したかどうかを判定するための hit 関数を持つ
// hit 関数は、レイがオブジェクトに衝突した場合には、衝突点の情報を収めた hit_record 構造体を返す

class material;

// hit関数で計算したデータを収める構造体
struct hit_record
{
	point3 p;					  // 衝突点
	vec3 normal;				  // 衝突点の法線ベクトル
	shared_ptr<material> mat_ptr; // 衝突したオブジェクトのマテリアル
	double t;					  // 衝突点のパラメータt
	bool front_face;			  // 衝突点の法線ベクトルがレイの方向と同じ向きかどうか

	// 法線ベクトルの向きをレイの方向と同じにする
	inline void set_face_normal(const ray &r, const vec3 &outward_normal)
	{
		front_face = dot(r.direction(), outward_normal) < 0;	// レイの方向と外向き法線ベクトルが反対方向ならfalse
		normal = front_face ? outward_normal : -outward_normal; // 法線ベクトルの向きをレイの方向と同じにする
	}
};

class hittable
{
public:
	virtual ~hittable() {}
	virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
};