#include "tile_tool.h"

// 浮点数比较
static int coord_float_eq(coord_float a, coord_float b) {
    return fabsf(a - b) < COORD_EPSILON;
}

// 浮点数限制范围
static coord_float clamp_float(coord_float value, coord_float min_val, coord_float max_val) {
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

// 度转弧度
coord_float deg_to_rad_float(coord_float degrees) {
    return degrees * COORD_PI / 180.0f;
}

// 弧度转度
coord_float rad_to_deg_float(coord_float radians) {
    return radians * 180.0f / COORD_PI;
}

// 判断是否在中国境内
int is_in_china_float(coord_float lng, coord_float lat) {
    return (lng >= 72.004f && lng <= 137.8347f &&
            lat >= 0.8293f && lat <= 55.8271f);
}

// 坐标有效性验证
int is_valid_coord_float(coord_float lng, coord_float lat) {
    return (lng >= -180.0f && lng <= 180.0f &&
            lat >= -90.0f && lat <= 90.0f);
}

// 纬度偏移计算
coord_float transform_lat_float(coord_float x, coord_float y) {
    coord_float ret = -100.0f + 2.0f * x + 3.0f * y + 0.2f * y * y + 0.1f * x * y;
    ret += 0.2f * sqrtf(fabsf(x));

    ret += (20.0f * sinf(6.0f * x * COORD_PI) +
            20.0f * sinf(2.0f * x * COORD_PI)) * 2.0f / 3.0f;
    ret += (20.0f * sinf(y * COORD_PI) +
            40.0f * sinf(y / 3.0f * COORD_PI)) * 2.0f / 3.0f;
    ret += (160.0f * sinf(y / 12.0f * COORD_PI) +
            320.0f * sinf(y * COORD_PI / 30.0f)) * 2.0f / 3.0f;

    return ret;
}

// 经度偏移计算
coord_float transform_lon_float(coord_float x, coord_float y) {
    coord_float ret = 300.0f + x + 2.0f * y + 0.1f * x * x + 0.1f * x * y;
    ret += 0.1f * sqrtf(fabsf(x));

    ret += (20.0f * sinf(6.0f * x * COORD_PI) +
            20.0f * sinf(2.0f * x * COORD_PI)) * 2.0f / 3.0f;
    ret += (20.0f * sinf(x * COORD_PI) +
            40.0f * sinf(x / 3.0f * COORD_PI)) * 2.0f / 3.0f;
    ret += (150.0f * sinf(x / 12.0f * COORD_PI) +
            300.0f * sinf(x / 30.0f * COORD_PI)) * 2.0f / 3.0f;

    return ret;
}

// WGS84转GCJ02
int wgs84_to_gcj02_float(coord_float wgs_lng, coord_float wgs_lat,
                        coord_float *gcj_lng, coord_float *gcj_lat) {

    if (!is_valid_coord_float(wgs_lng, wgs_lat)) {
        return -1;  // 无效坐标
    }

    if (!is_in_china_float(wgs_lng, wgs_lat)) {
        *gcj_lng = wgs_lng;
        *gcj_lat = wgs_lat;
        return 0;  // 境外坐标，无需转换
    }

    // 使用float进行计算
    coord_float dlat = transform_lat_float(wgs_lng - 105.0f, wgs_lat - 35.0f);
    coord_float dlng = transform_lon_float(wgs_lng - 105.0f, wgs_lat - 35.0f);

    coord_float radlat = wgs_lat * COORD_PI / 180.0f;
    coord_float magic = sinf(radlat);
    magic = 1.0f - EARTH_E * magic * magic;

    coord_float sqrtmagic = sqrtf(magic);
    dlat = (dlat * 180.0f) / ((EARTH_RADIUS * (1.0f - EARTH_E)) / (magic * sqrtmagic) * COORD_PI);
    dlng = (dlng * 180.0f) / (EARTH_RADIUS / sqrtmagic * cosf(radlat) * COORD_PI);

    *gcj_lng = wgs_lng + dlng;
    *gcj_lat = wgs_lat + dlat;

    return 0;
}

// GCJ02转WGS84
int gcj02_to_wgs84_float(coord_float gcj_lng, coord_float gcj_lat,
                        coord_float *wgs_lng, coord_float *wgs_lat) {

    if (!is_valid_coord_float(gcj_lng, gcj_lat)) {
        return -1;
    }

    if (!is_in_china_float(gcj_lng, gcj_lat)) {
        *wgs_lng = gcj_lng;
        *wgs_lat = gcj_lat;
        return 0;
    }

    coord_float dlat = transform_lat_float(gcj_lng - 105.0f, gcj_lat - 35.0f);
    coord_float dlng = transform_lon_float(gcj_lng - 105.0f, gcj_lat - 35.0f);

    coord_float radlat = gcj_lat * COORD_PI / 180.0f;
    coord_float magic = sinf(radlat);
    magic = 1.0f - EARTH_E * magic * magic;

    coord_float sqrtmagic = sqrtf(magic);
    dlat = (dlat * 180.0f) / ((EARTH_RADIUS * (1.0f - EARTH_E)) / (magic * sqrtmagic) * COORD_PI);
    dlng = (dlng * 180.0f) / (EARTH_RADIUS / sqrtmagic * cosf(radlat) * COORD_PI);

    *wgs_lng = gcj_lng - dlng;
    *wgs_lat = gcj_lat - dlat;

    return 0;
}

// 经纬度转墨卡托投影坐标
MercatorCoord latlng_to_mercator_float(coord_float lng, coord_float lat) {
    MercatorCoord mc;

    // 经度直接转换
    mc.mx = lng * WEB_MERCATOR_MAX / 180.0f;

    // 纬度转换（限制在有效范围内）
    lat = clamp_float(lat, -85.05112878f, 85.05112878f);

    coord_float sin_lat = sinf(lat * COORD_PI / 180.0f);
    mc.my = logf((1.0f + sin_lat) / (1.0f - sin_lat)) * WEB_MERCATOR_MAX / (2.0f * COORD_PI);

    return mc;
}

// 墨卡托坐标转经纬度
GeoCoord mercator_to_latlng_float(coord_float mx, coord_float my) {
    GeoCoord coord;

    coord.lng = mx * 180.0f / WEB_MERCATOR_MAX;
    coord.lat = 180.0f / COORD_PI * (2.0f * atanf(expf(my * COORD_PI / WEB_MERCATOR_MAX)) - COORD_PI / 2.0f);

    return coord;
}

// 经纬度转像素坐标
void latlng_to_pixel_float(coord_float lng, coord_float lat, int zoom,
                          coord_float *pixel_x, coord_float *pixel_y) {

    // 将经纬度限制在有效范围内
    lng = clamp_float(lng, -180.0f, 180.0f);
    lat = clamp_float(lat, -85.05112878f, 85.05112878f);

    // 计算缩放因子
    coord_float scale = powf(2.0f, (coord_float)zoom) * TILE_SIZE / 360.0f;

    // 计算像素坐标
    *pixel_x = (180.0f + lng) * scale;

    // 使用墨卡托投影公式计算Y坐标
    coord_float sin_lat = sinf(lat * COORD_PI / 180.0f);
    coord_float merc_y = logf((1.0f + sin_lat) / (1.0f - sin_lat)) / 2.0f;
    *pixel_y = (180.0f - merc_y * 180.0f / COORD_PI) * scale;
}

// 像素坐标转瓦片坐标
void pixel_to_tile_float(coord_float pixel_x, coord_float pixel_y,
                        int *tile_x, int *tile_y) {
    *tile_x = (int)floorf(pixel_x / TILE_SIZE);
    *tile_y = (int)floorf(pixel_y / TILE_SIZE);
}

// 经纬度直接转瓦片坐标
void latlng_to_tile_float(coord_float lng, coord_float lat, int zoom,
                         int *tile_x, int *tile_y) {

    coord_float pixel_x, pixel_y;
    latlng_to_pixel_float(lng, lat, zoom, &pixel_x, &pixel_y);
    pixel_to_tile_float(pixel_x, pixel_y, tile_x, tile_y);

    // 计算最大瓦片编号
    int max_tile = (int)powf(2.0f, (coord_float)zoom) - 1;

    // 确保瓦片编号在有效范围内
    *tile_x = clamp_float(*tile_x, 0, max_tile);
    *tile_y = clamp_float(*tile_y, 0, max_tile);
}

// 获取缩放比例因子
coord_float get_zoom_scale_factor_float(int from_zoom, int to_zoom) {
    if (from_zoom == to_zoom) {
        return 1.0f;
    }
    int zoom_diff = to_zoom - from_zoom;
    return powf(2.0f, (coord_float)zoom_diff);
}

// 像素坐标缩放转换
void pixel_zoom_convert_float(coord_float pixel_x, coord_float pixel_y,
                             int from_zoom, int to_zoom,
                             coord_float *new_pixel_x, coord_float *new_pixel_y) {

    coord_float scale_factor = get_zoom_scale_factor_float(from_zoom, to_zoom);
    *new_pixel_x = pixel_x * scale_factor;
    *new_pixel_y = pixel_y * scale_factor;
}

// 瓦片坐标转边界经纬度
void tile_to_bounds_float(int tile_x, int tile_y, int zoom,
                         coord_float *min_lon, coord_float *min_lat,
                         coord_float *max_lon, coord_float *max_lat) {

    // 计算像素坐标边界
    coord_float pixel_min_x = tile_x * TILE_SIZE;
    coord_float pixel_min_y = tile_y * TILE_SIZE;
    coord_float pixel_max_x = (tile_x + 1) * TILE_SIZE;
    coord_float pixel_max_y = (tile_y + 1) * TILE_SIZE;

    // 计算缩放因子
    coord_float scale = powf(2.0f, (coord_float)zoom) * TILE_SIZE / 360.0f;

    // 计算经度边界
    *min_lon = pixel_min_x / scale - 180.0f;
    *max_lon = pixel_max_x / scale - 180.0f;

    // 计算纬度边界
    coord_float merc_min_y = (180.0f - pixel_min_y / scale) * COORD_PI / 180.0f;
    coord_float merc_max_y = (180.0f - pixel_max_y / scale) * COORD_PI / 180.0f;

    *min_lat = 180.0f / COORD_PI * (2.0f * atanf(expf(merc_max_y)) - COORD_PI / 2.0f);
    *max_lat = 180.0f / COORD_PI * (2.0f * atanf(expf(merc_min_y)) - COORD_PI / 2.0f);
}
