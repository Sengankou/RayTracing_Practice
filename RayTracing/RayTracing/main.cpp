#include "rtweekend.h"
#include "hittable_list.h"
#include "color.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <iostream>

// レイがシーン内のオブジェクトに衝突するかどうかを判断し、衝突した場合にはそのオブジェクトのマテリアルの散乱を計算して、
// 次のレイを生成することにより、トレースするレイのカラーを計算する関数（再帰的に計算）
color ray_color(const ray &r, const hittable &world, int depth)
{
    hit_record rec;
    // 再帰の最大深度に達したら(depthが0より小さくなったら)、黒を返す
    if (depth <= 0)
        return color(0, 0, 0);

    // パラメータt=0.001 から無限大の間のレイと衝突するオブジェクトを判定する
    // 0.001 は、レイの始点がオブジェクトの表面上にある場合に、レイが自分自身に衝突してしまうのを防ぐための微小なオフセット
    if (world.hit(r, 0.001, infinity, rec))
    {
        ray scattered;     // 散乱後のレイ
        color attenuation; // 減衰率
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            // 衝突したオブジェクトのマテリアルの散乱を計算できた場合は、散乱後のレイのカラーを再帰的に計算する
            return attenuation * ray_color(scattered, world, depth - 1);
        // 衝突したオブジェクトのマテリアルの散乱を計算できなかった場合は、黒を返す
        return color(0, 0, 0);
    }

    // レイがシーン内のオブジェクトに衝突しなかった場合は、背景色を返す
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0); // 青空風のグラデーション
}

// シーンを生成する
hittable_list make_scene()
{
    // world
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));         // 地面のマテリアル
    world.add(make_shared<sphere>(point3(0, -10000, 0), 10000, ground_material)); // 地面となるオブジェクトを追加

    /* とあるシーン用の設定
    for (int a = -12; a < 12; a += 2) {
        for (int b = -12; b < 12; b += 2) {
            auto choose_mat = random_double();
            point3 center(a + 2.0 * random_double(), 0.2, b + 2.0 * random_double());

           if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
               shared_ptr<material> sphere_material;
               if (choose_mat < 0.8) {
                   //diffuse
                   auto albedo = color::random() * color::random();
                   sphere_material = make_shared<lambertian>(albedo);
                   world.add(make_shared<sphere>(center, 0.2, sphere_material));
               }
               else if (choose_mat < 0.95) {
                   //metal
                   auto albedo = color::random(0.5, 1);
                   auto fuzz = random_double(0, 0.5);
                   sphere_material = make_shared<metal>(albedo, fuzz);
                   world.add(make_shared<sphere>(center, 0.2, sphere_material));
               }
               else {
                   //glass
                   sphere_material = make_shared<dielectric>(1.5);
                   world.add(make_shared<sphere>(center, 0.2, sphere_material));
               }
           }
       }
    }
    */

    // また別のシーン用の設定
    for (int a = 0; a < 10; a++)
    {
        auto material0 = make_shared<dielectric>(1.5);
        world.add(make_shared<sphere>(point3(-4 * a, 2, 0), 1.0, material0));
        // auto material5 = make_shared<dielectric>(1.5);
        // world.add(make_shared<sphere>(point3(-4 * a, 2, -2), 1.0, material5));
        // auto material7 = make_shared<dielectric>(1.5);
        // world.add(make_shared<sphere>(point3(-4 * a, 2, 2), 1.0, material7));
        auto material2 = make_shared<metal>(color(0.7, 0.7, 0.7), 0.0);
        world.add(make_shared<sphere>(point3(2 - 4 * a, 0, -2), 1.5, material2));
        auto material4 = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
        world.add(make_shared<sphere>(point3(2 - 4 * a, 0, 2), 1.5, material4));
    }

    /* またまた別のシーン用の設定
    auto material0 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 2, 0), 1.0, material0));
    auto material1 = make_shared<metal>(color(0.2, 0.5, 0.7), 0.0);
    world.add(make_shared<sphere>(point3(2, 0, -2), 1.5, material1));
    auto material2 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(-2, 0, -2), 1.5, material2));
    auto material3 = make_shared<metal>(color(0.5, 0.7, 0.2), 0.0);
    world.add(make_shared<sphere>(point3(-2, 0, 2), 1.5, material3));
    auto material4 = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
    world.add(make_shared<sphere>(point3(2, 0, 2), 1.5, material4));
    */

    return world;
}

int main()
{
    const auto aspect_ratio = 16.0 / 9.0;                                  // アスペクト比指定
    const int image_width = 384;                                           // 画像の幅指定
    const int image_height = static_cast<int>(image_width / aspect_ratio); // 画像の高さ指定
    const int samples_per_pixel = 50;                                      // 1ピクセルあたりのサンプル数
    const int max_depth = 40;                                              // レイの最大反射回数

    // ASCIIデータのPPM形式（P3）として画像を保存するための設定
    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    // シーンの生成
    auto world = make_scene();
    /*
    auto R = cos(pi / 4);
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5, make_shared<lambertian>(color(0.1, 0.2, 0.5))));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, make_shared<lambertian>(color(0.8, 0.8, 0.0))));
    world.add(make_shared<sphere>(point3(1, 0, -1), 0.5, make_shared<metal>(color(.6, .8, .2), 0.2)));
    world.add(make_shared<sphere>(point3(-1, 0, -1), 0.5, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(point3(-1, 0, -1), -0.45, make_shared<dielectric>(1.5)));
    //world.add(make_shared<sphere>(point3(-R, 0, -1), R, make_shared<lambertian>(color(0, 0, 1))));
    //world.add(make_shared<sphere>(point3(R, 0, -1), R, make_shared<lambertian>(color(1, 0, 0))));
    */

    // カメラの設定
    point3 lookfrom(3.2, 1, 0); // カメラの位置
    point3 lookat(0, 0.3, 0);   // カメラの焦点
    vec3 vup(0, 1, 0);          // カメラの上方向(view up)
    auto dist_to_focus = 10;    // 焦点距離
    auto aperture = 0.1;        // レンズの開口率
    // カメラの生成
    camera cam(lookfrom, lookat, vup, 70, aspect_ratio, aperture, dist_to_focus);

    for (int j = image_height - 1; j >= 0; --j)
    {
        // 画像の上から下に向かって走査し、残りのスキャンライン数（走査する行数）を表示する
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush; // 進捗インジケーター
        for (int i = 0; i < image_width; ++i)
        {
            // 画像の左から右に向かって走査する
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            // 結果をPPMファイルとして保存するために、標準出力をリダイレクトする必要があることに注意してください。
            // 例えば、Visual Studio C++では、プロジェクトのプロパティを変更する必要があります。
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}
