#pragma once

// 定数やユーティリティ関数をまとめたヘッダファイル

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <random>

// using
using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// 定数
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// ユーティリティ関数
inline double degrees_to_radians(double degrees)
{
	return degrees * pi / 180;
}

inline double random_double()
{
	// [0, 1)の実数乱数を返す
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
	//[min, max)の実数乱数を返す
	return min + (max - min) * random_double();
}

inline double random_double_cpp()
{
	// <random>ライブラリを使って[0, 1)の実数乱数を返す関数
	static std::uniform_real_distribution<double> distribution(0.0, 1.0); // [0.0, 1.0)の一様分布
	static std::mt19937 generator;										  // メルセンヌ・ツイスタ（疑似乱数生成器）のインスタンスを作成
	return distribution(generator);										  // 一様分布から乱数を生成
}

inline double clamp(double x, double min, double max)
{
	// xを[min, max]の範囲に切り捨てる
	if (x < min)
		return min;
	if (x > max)
		return max;
	return x;
}

// 共通ヘッダー
#include "ray.h"
#include "vec3.h"
