#include "GPS_Transform.h"
#include <stdbool.h>
#include <math.h>

#define ABS(x) (((x)>0)?(x):-(x))

static const float pi = 3.14159265358979324f;
static const float a = 6378245.0f;
static const float ee = 0.00669342162296594323f;

static float transformLat(float x, float y)
{
    float ret = -100.0f + 2.0f * x + 3.0f * y + 0.2f * y * y + 0.1f * x * y + 0.2f * sqrtf(ABS(x));
    ret += (20.0f * sinf(6.0f * x * pi) + 20.0f * sinf(2.0f * x * pi)) * 2.0f / 3.0f;
    ret += (20.0f * sinf(y * pi) + 40.0f * sinf(y / 3.0f * pi)) * 2.0f / 3.0f;
    ret += (160.0f * sinf(y / 12.0f * pi) + 320.0f * sinf(y * pi / 30.0f)) * 2.0f / 3.0f;
    return ret;
}

static float transformLon(float x, float y)
{
    float ret = 300.0f + x + 2.0f * y + 0.1f * x * x + 0.1f * x * y + 0.1f * sqrtf(ABS(x));
    ret += (20.0f * sinf(6.0f * x * pi) + 20.0f * sinf(2.0f * x * pi)) * 2.0f / 3.0f;
    ret += (20.0f * sinf(x * pi) + 40.0f * sinf(x / 3.0f * pi)) * 2.0f / 3.0f;
    ret += (150.0f * sinf(x / 12.0f * pi) + 300.0f * sinf(x / 30.0f * pi)) * 2.0f / 3.0f;
    return ret;
}

void GPS_Transform(float wgLat, float wgLon, float* mgLat, float* mgLon)
{
    float dLat = transformLat(wgLon - 105.0f, wgLat - 35.0f);
    float dLon = transformLon(wgLon - 105.0f, wgLat - 35.0f);
    float radLat = wgLat / 180.0f * pi;
    float magic = sinf(radLat);
    magic = 1.0f - ee * magic * magic;
    float sqrtMagic = sqrtf(magic);
    dLat = (dLat * 180.0f) / ((a * (1.0f - ee)) / (magic * sqrtMagic) * pi);
    dLon = (dLon * 180.0f) / (a / sqrtMagic * cosf(radLat) * pi);
    *mgLat = wgLat + dLat;
    *mgLon = wgLon + dLon;
};
