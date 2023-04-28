#pragma once
#include "rtweekend.h"

// 仮想的なカメラの管理と、シーンのサンプリングに関する処理を行うカメラクラス
class camera
{
public:
	// カメラの位置、スクリーンの大きさ、スクリーンとカメラの距離などを指定するコンストラクタ
	camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist)
	{
		auto theta = degrees_to_radians(vfov); // vfovは垂直方向の視野角 (弧度法)
		auto h = tan(theta / 2);
		auto viewport_height = 2.0 * h;
		auto viewport_width = aspect_ratio * viewport_height;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		origin = lookfrom;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		/
			lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

		lens_radius = aperture / 2;
	}

	// スクリーン上の位置を表すパラメータs, tを受け取り、その位置からレイを生成して返す
	ray get_ray(double s, double t) const
	{
		// s, tはスクリーン上の位置を表すパラメータ
		vec3 rd = lens_radius * random_in_unit_disk(); // レンズ上のランダムな点を生成する
		vec3 offset = u * rd.x() + v * rd.y();		   // レンズ上のランダムな点をカメラの位置にオフセットする

		return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
	}

public:
	point3 origin;			  // カメラの位置
	point3 lower_left_corner; // スクリーンの左下の点
	vec3 horizontal;		  // スクリーンの横幅
	vec3 vertical;			  // スクリーンの縦幅
	vec3 u, v, w;			  // カメラの位置を原点とする座標系の基底ベクトル
	double lens_radius;		  // レンズの半径
};