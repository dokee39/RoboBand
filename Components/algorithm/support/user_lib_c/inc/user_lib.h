#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "compiler.h"

#define flaot0           1e-6
#define PI               3.141592653589793238462643383279f
#define HALF_PI          1.570796326794896619231321691639f
#define TWO_PI           6.283185307179586476925286766559f
#define DEG_TO_RAD_RATIO 0.017453292519943295769236907684f
#define RAD_TO_DEG_RATIO 57.29577951308232087679815481410f
#define EULER            2.718281828459045235360287471352f

#define DEG2RAD(x) ((x) * DEG_TO_RAD)
#define RAD2DEG(x) ((x) * RAD_TO_DEG)

#define isIndexOutofBounds(index, length)        (((index) < 0) || ((index) > (length) - 1))
#define isFloatEqual0(x)                         (((float)(x) < (float)(float0)) ? 1 : 0)

#define map(x, in_min, in_max, out_min, out_max) (((x) - (in_min)) * ((out_max) - (out_min)) / ((in_max) - (in_min)) + (out_min));

#define sign(x)             (((x) >= 0) ? 1 : -1)
#define max(x,y) ({         \
    typeof(x) _x = (x);     \
    typeof(y) _y = (y);     \
    (void) (&_x == &_y);    \
    _x > _y ? _x : _y; })
#define min(x,y) ({         \
    typeof(x) _x = (x);     \
    typeof(y) _y = (y);     \
    (void) (&_x == &_y);    \
    _x < _y ? _x : _y; })
#define min3(a, b, c)		min(a, min(b, c))
#define max3(a, b, c)		max(a, max(b, c))
#define min4(a, b, c, d)	min(min(a, b), min(c, d))
#define max4(a, b, c, d)	max(max(a, b), max(b, d))
#define min5(a, b, c, d, e) min3(min(a, b), min(c, d), e)
#define max5(a, b, c, d, e) max3(max(a, b), max(b, d), e)

#define constrain_0_2pi(amt)           (((amt) > 2 * PI) ? ((amt) -= 2 * PI) : (((amt) < 0) ? ((amt) += 2 * PI) : ((amt) = (amt))))
#define constrain_npi_pi(amt)          (((amt) > PI) ? ((amt) -= 2 * PI) : (((amt) < -PI) ? ((amt) += 2 * PI) : ((amt) = (amt))))
#define constrain_0_360(amt)           (((amt) > 360) ? ((amt) -= 360) : (((amt) < 0) ? ((amt) += 360) : ((amt) = (amt))))
#define constrain_n180_180(amt)        (((amt) > 180) ? ((amt) -= 360) : (((amt) < -180) ? ((amt) += 360) : ((amt) = (amt))))
#define constrain_min(amt, low)        ((amt) < (low) ? ((amt) = (low)) : ((amt) = (amt)))
#define constrain_max(amt, high)       ((amt) > (high) ? ((amt) = (high)) : ((amt) = (amt)))
#define constrain(amt, low, high)      ((amt) < (low) ? ((amt) = (low)) : ((amt) > (high) ? ((amt) = (high)) : ((amt) = (amt))))
#define loop_constrain(amt, low, high) ((amt) < (low) ? ((amt) = (high)) : ((amt) > (high) ? ((amt) = (low)) : ((amt) = (amt))))

#define deadline(val, min, max)   ((((val) > (min)) && ((val) < (max))) ? 0 : (val))
#define limit(val, min, max)     \
do {                             \
    if((val) <= (min)) {         \
        (val) = (min);           \
    } else if ((val) >= (max)) { \
        (val) = (max);           \
    }                            \
} while(0)

#define getBit(value, pos)    (((value) >> (pos)) & 1)
#define setBit(value, pos)    ((value) | (1 << (pos)))
#define clrBit(value, pos)    ((value) & (~(1 << (pos))))
#define toggleBit(value, pos) ((value) ^ (1 << (pos)))

typedef union
{
    uint8_t value;
    unsigned char data[1];
} hex_uint8_u;

typedef union
{
    uint16_t value;
    unsigned char data[2];
} hex_uint16_u;

typedef union
{
    int16_t value;
    unsigned char data[2];
} hex_int16_u;

typedef union
{
    float value;
    unsigned char data[4];
} hex_float_u;

typedef struct {
    int32_t val;
    int32_t in_min;
    int32_t in_max;
    int32_t val_min;
    int32_t val_max;
} ramp_int32_t;

typedef struct {
    float val;
    float in_min;
    float in_max;
    float val_min;
    float val_max;
} ramp_float_t;

typedef struct {
    float in; //输入数据
    float out; //滤波输出的数据
    float num[1]; //滤波参数
    float frame_period; //滤波的时间间隔 单位 s
} __packed first_order_filter_type_t;

extern float invSqrt(float num); // in MahonyAHRS.c

void ramp_int32_init(ramp_int32_t *ramp, int32_t init_val, int32_t in_min, int32_t in_max, int32_t val_min, int32_t val_max);
int32_t ramp_int32_calc(ramp_int32_t *ramp, int32_t in);
void ramp_int32_update(ramp_int32_t *ramp, int32_t set);
void ramp_float_init(ramp_float_t *ramp, float init_val, float in_min, float in_max, float val_min, float val_max);
float ramp_float_calc(ramp_float_t *ramp, float in);
void ramp_float_update(ramp_float_t *ramp, float set);



void first_order_filter_init(
    first_order_filter_type_t* first_order_filter_type,
    float frame_period,
    const float num[1]
);
void first_order_filter_cali(first_order_filter_type_t* first_order_filter_type, float input);

#ifdef __cplusplus
}
#endif

