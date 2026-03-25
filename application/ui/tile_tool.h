#ifndef COORD_FLOAT_H
#define COORD_FLOAT_H

#include <math.h>
#include <stdio.h>
#include <stdint.h>

// 定义浮点类型
typedef float coord_float;
#define COORD_EPSILON 1e-6f
#define COORD_PI 3.14159265358979323846f
#define TILE_SIZE 256  // 瓦片尺寸256x256

// 坐标结构体
typedef struct {
    coord_float lng;  // 经度
    coord_float lat;  // 纬度
} GeoCoord;

// 瓦片坐标结构体
typedef struct {
    int x;            // 瓦片X编号
    int y;            // 瓦片Y编号
    int z;            // 缩放级别
} TileCoord;

// 像素坐标结构体
typedef struct {
    coord_float x;    // 像素X坐标
    coord_float y;    // 像素Y坐标
} PixelCoord;

// 米制坐标结构体（Web墨卡托）
typedef struct {
    coord_float mx;   // 米制X坐标
    coord_float my;   // 米制Y坐标
} MercatorCoord;

// 常量定义
#define EARTH_RADIUS 6378245.0f      // 地球长半轴
#define EARTH_E 0.00669342162296594323f  // 扁率
#define EARTH_POLAR_RADIUS 6356752.3142f  // 极半径
#define WEB_MERCATOR_MAX 20037508.3427892f  // Web墨卡托最大坐标

// 坐标转换函数
int wgs84_to_gcj02_float(coord_float wgs_lng, coord_float wgs_lat,
                        coord_float *gcj_lng, coord_float *gcj_lat);
int gcj02_to_wgs84_float(coord_float gcj_lng, coord_float gcj_lat,
                        coord_float *wgs_lng, coord_float *wgs_lat);

// 瓦片计算函数
void latlng_to_tile_float(coord_float lng, coord_float lat, int zoom,
                         int *tile_x, int *tile_y);
void latlng_to_pixel_float(coord_float lng, coord_float lat, int zoom,
                          coord_float *pixel_x, coord_float *pixel_y);
void pixel_to_tile_float(coord_float pixel_x, coord_float pixel_y,
                        int *tile_x, int *tile_y);
void tile_to_bounds_float(int tile_x, int tile_y, int zoom,
                         coord_float *min_lon, coord_float *min_lat,
                         coord_float *max_lon, coord_float *max_lat);

coord_float get_zoom_scale_factor_float(int from_zoom, int to_zoom) ;
void pixel_zoom_convert_float(coord_float pixel_x, coord_float pixel_y,
                             int from_zoom, int to_zoom,
                             coord_float *new_pixel_x, coord_float *new_pixel_y);

// 墨卡托投影函数
MercatorCoord latlng_to_mercator_float(coord_float lng, coord_float lat);
GeoCoord mercator_to_latlng_float(coord_float mx, coord_float my);

// 坐标验证函数
int is_valid_coord_float(coord_float lng, coord_float lat);
int is_in_china_float(coord_float lng, coord_float lat);

// 工具函数
coord_float deg_to_rad_float(coord_float degrees);
coord_float rad_to_deg_float(coord_float radians);
coord_float transform_lat_float(coord_float x, coord_float y);
coord_float transform_lon_float(coord_float x, coord_float y);

#endif
