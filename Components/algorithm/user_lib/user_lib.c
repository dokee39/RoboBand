#include "user_lib.h"

void ramp_int32_init(ramp_int32_t *ramp, int32_t init_val, int32_t in_min, int32_t in_max, int32_t val_min, int32_t val_max)
{
    ramp->in_min = in_min;
    ramp->in_max = in_max;
    ramp->val_min = val_min;
    ramp->val_max = val_max;
    ramp->val = init_val;
}

int32_t ramp_int32_calc(ramp_int32_t *ramp, int32_t in)
{
    int32_t input;
    if (in < ramp->in_min) {
        input = ramp->in_min;
    } else if (in > ramp->in_max) {
        input = ramp->in_max;
    } else {
        input = in;
    }

    int32_t val = ramp->val + input;
    if (val < ramp->val_min) {
        ramp->val = ramp->val_min;
    } else if (val > ramp->val_max) {
        ramp->val = ramp->val_max;
    } else {
        ramp->val = val;
    }

    return ramp->val;
}

void ramp_int32_update(ramp_int32_t *ramp, int32_t set)
{
    if (set - ramp->val < ramp->in_min) {
        set = ramp->val + ramp->in_min; 
    } else if (set - ramp->val > ramp->in_max) {
        set = ramp->val + ramp->in_max;
    }
    if (set < ramp->val_min) {
        ramp->val = ramp->val_min;
    } else if (set > ramp->val_max) {
        ramp->val = ramp->val_max;
    } else {
        ramp->val = set;
    }
}

void ramp_float_init(ramp_float_t *ramp, float init_val, float in_min, float in_max, float val_min, float val_max)
{
    ramp->in_min = in_min;
    ramp->in_max = in_max;
    ramp->val_min = val_min;
    ramp->val_max = val_max;
    ramp->val = init_val;
}

float ramp_float_calc(ramp_float_t *ramp, float in)
{
    float input;
    if (in < ramp->in_min) {
        input = ramp->in_min;
    } else if (in > ramp->in_max) {
        input = ramp->in_max;
    } else {
        input = in;
    }

    float val = ramp->val + input;
    if (val < ramp->val_min) {
        ramp->val = ramp->val_min;
    } else if (val > ramp->val_max) {
        ramp->val = ramp->val_max;
    } else {
        ramp->val = val;
    }

    return ramp->val;
}

void ramp_float_update(ramp_float_t *ramp, float set)
{
    if (set - ramp->val < ramp->in_min) {
        set = ramp->val + ramp->in_min; 
    } else if (set - ramp->val > ramp->in_max) {
        set = ramp->val + ramp->in_max;
    }
    if (set < ramp->val_min) {
        ramp->val = ramp->val_min;
    } else if (set > ramp->val_max) {
        ramp->val = ramp->val_max;
    } else {
        ramp->val = set;
    }
}

void first_order_filter_init(
    first_order_filter_type_t* first_order_filter_type,
    float frame_period,
    const float num[1]
) {
    first_order_filter_type->frame_period = frame_period;
    first_order_filter_type->num[0] = num[0];
    first_order_filter_type->in = 0.0f;
    first_order_filter_type->out = 0.0f;
}

void first_order_filter_cali(first_order_filter_type_t* first_order_filter_type, float in) {
    first_order_filter_type->in = in;
    first_order_filter_type->out = first_order_filter_type->num[0]
                                   / (first_order_filter_type->num[0] + first_order_filter_type->frame_period)
                                   * first_order_filter_type->out
                                   + first_order_filter_type->frame_period
                                   / (first_order_filter_type->num[0] + first_order_filter_type->frame_period)
                                   * first_order_filter_type->in;
}

