//
// TODO(gindia): add cpp support (op overload ..etc)
// TODO(gindia): fix the style.
// TODO(gindia): add versor_look_at(..) function.
//

#pragma once

#include <math.h>
#include <float.h>

#ifndef M_PI
#define M_PI  ((float)3.14159265358979323846)
#endif

// Forward declration {{{

#define  MIN(a, b) (a > b) ?  b : a
#define  MAX(a, b) (a < b) ?  b : a

#define  SIGN(a)  (((a) < 0  ) ? -1   : 1   )
#define  SIGNF(a) (((a) < 0.0) ? -1.0 : 1.0 )

typedef struct vec2  vec2;
typedef struct vec3  vec3;
typedef struct vec4  vec4;
typedef struct vec4  versor;
typedef union  mat4  mat4;
typedef union  ivec2 ivec2;
typedef struct ivec3 ivec3;
typedef struct quad  quad;

static vec2  vec2_zero  (void);
static vec2  vec2_add   (vec2 lhs,  vec2 rhs);
static void  vec2_addeq (vec2 *lhs, vec2 rhs);
static vec2  vec2_sub   (vec2 lhs,  vec2 rhs);
static void  vec2_subeq (vec2 *lhs, vec2 rhs);
static vec2  vec2_mul   (vec2 lhs,  vec2 rhs);
static vec2  vec2_div   (vec2 lhs,  vec2 rhs);
static vec2  vec2_scale (vec2 lhs,  float rhs);
static vec2  vec2_neg   (vec2 v);
static float vec2_dot   (vec2 lhs,  vec2 rhs);
static float vec2_cross (vec2 lhs,  vec2 rhs);
static float vec2_mag   (vec2 v);
static vec2  vec2_norm  (vec2 v);
static vec2  vec2_abs   (vec2 v);
static vec2  vec2_lerp  (vec2 from, vec2 to, float t);
static vec2  vec2_step  (vec2 from, vec2 to, float step);

static vec3  vec3_zero       (void);
static vec3  vec3_one        (void);
static vec3  vec3_add        (vec3 lhs,  vec3 rhs);
static vec3  vec3_sub        (vec3 lhs,  vec3 rhs);
static vec3  vec3_mul        (vec3 lhs,  vec3 rhs);
static vec3  vec3_div        (vec3 lhs,  vec3 rhs);
static float vec3_dot        (vec3 lhs,  vec3 rhs);
static vec3  vec3_cross      (vec3 lhs,  vec3 rhs);
static vec3  vec3_scale      (vec3 lhs,  float  rhs);
static vec3  vec3_lerp       (vec3 from, vec3 to, float t);
static vec3  vec3_neg        (vec3 v);
static float vec3_mag        (vec3 v);
static vec3  vec3_norm       (vec3 v);
static float vec3_angle      (vec3 v, vec3 u);
static vec3  vec3_project_on (vec3 v, vec3 on);
static int   vec3_eq         (vec3 lhs, vec3 rhs);

static vec4  vec4_zero (void);
static float vec4_dot  (vec4 lhs, vec4 rhs);

static versor versor_identity           (void);
static float  versor_real               (versor v);
static vec3   versor_imaginary          (versor v);
static float  versor_mag                (versor v);
static versor versor_norm               (versor v);
static versor versor_conj               (versor v);
static versor versor_mul                (versor lhs, versor rhs);
static versor versor_from_euler         (vec3 axis, float angle);
static versor versor_around_axis_degrees(float x, float y, float z);

static mat4   mat4_zero                   (void);
static mat4   mat4_identity               (void);
static void   mat4_translate              (mat4 *m, vec3 v);
static void   mat4_scale                  (mat4 *m, vec3 v);
static mat4   mat4_ortho                  (float left, float right, float bottom, float top, float n, float f);
static mat4   mat4_perspective            (float fov, float aspect, float n, float f);
static mat4   mat4_rotation               (float yaw, float pitch, float roll);
static mat4   mat4_rotation_deg           (float yaw, float pitch, float roll);
static mat4   mat4_mul                    (mat4 lhs,  mat4 rhs);
static mat4   mat4_from_versor            (versor v);
static mat4   mat4_scale_rotate_translate (vec3 scale, versor rotation, vec3 translate);
static mat4   mat4_lookat                 (vec3 eye, vec3 target, vec3 up);

static ivec2  ivec2_zero  (void);
static ivec2  ivec2_add   (ivec2 lhs,  ivec2 rhs);
static void   ivec2_addeq (ivec2 *lhs, ivec2 rhs);
static ivec2  ivec2_sub   (ivec2 lhs,  ivec2 rhs);
static void   ivec2_subeq (ivec2 *lhs, ivec2 rhs);
static ivec2  ivec2_mul   (ivec2 lhs,  ivec2 rhs);
static ivec2  ivec2_div   (ivec2 lhs,  ivec2 rhs);
static ivec2  ivec2_scale (ivec2 lhs,  int rhs);
static ivec2  ivec2_neg   (ivec2 v);
static vec2   ivec2_to_vec2 (ivec2 v);


static ivec3  ivec3_zero  (void);
static ivec3  ivec3_add   (ivec3 lhs,  ivec3 rhs);
static void   ivec3_addeq (ivec3 *lhs, ivec3 rhs);
static ivec3  ivec3_sub   (ivec3 lhs,  ivec3 rhs);
static void   ivec3_subeq (ivec3 *lhs, ivec3 rhs);
static ivec3  ivec3_mul   (ivec3 lhs,  ivec3 rhs);
static ivec3  ivec3_div   (ivec3 lhs,  ivec3 rhs);
static ivec3  ivec3_scale (ivec3 lhs,  int rhs);
static int    ivec3_eq    (ivec3 lhs,  ivec3 rhs);
static ivec3  ivec3_neg   (ivec3 v);

static float deg_to_rad(float deg);
static float rad_to_deg(float rad);

static int   i32_xy_to_index(int x, int y, int width);
static ivec2 i32_index_to_xy(int index, int width);

static void  f32_swap(float *__restrict a, float *__restrict b);
static float f32_lerp(float from, float to, float t);
static float f32_max (float a, float b);
static int   f32_eq  (float a, float b);
static float f32_step(float from, float to, float step);
static int   i32_step(int from, int to, int step);

static quad quad_from_size      (vec2 top_left, vec2 size);
static int  quad_collision_point(quad q,  vec2 p);
static int  quad_collision_quad (quad q0, quad q1);

// }}}

// vec2 {{{

struct vec2 {
    float x, y;
};

inline vec2
vec2_zero (void)
{
    vec2 r;
    r.x = 0.f;
    r.y = 0.f;
    return r;
}

inline vec2
vec2_add(vec2 lhs,  vec2 rhs)
{
    vec2 r;
    r.x = lhs.x + rhs.x;
    r.y = lhs.y + rhs.y;
    return r;
}

inline void
vec2_addeq(vec2 *lhs, vec2 rhs)
{
    lhs->x += rhs.x;
    lhs->y += rhs.y;
}

inline vec2
vec2_sub(vec2 lhs, vec2 rhs)
{
    vec2 r;
    r.x = lhs.x - rhs.x;
    r.y = lhs.y - rhs.y;
    return r;
}

inline void
vec2_subeq(vec2 *lhs, vec2 rhs)
{
    lhs->x -= rhs.x;
    lhs->y -= rhs.y;
}

inline vec2
vec2_mul(vec2 lhs, vec2 rhs)
{
    vec2 r;
    r.x = lhs.x * rhs.x;
    r.y = lhs.y * rhs.y;
    return r;
}

inline vec2
vec2_div(vec2 lhs,  vec2 rhs)
{
    vec2 r;
    r.x = (rhs.x <= FLT_EPSILON) ? 0.f : lhs.x / rhs.x;
    r.y = (rhs.y <= FLT_EPSILON) ? 0.f : lhs.y / rhs.y;
    return r;
}

inline vec2
vec2_scale(vec2 lhs, float rhs)
{
    vec2 r;
    r.x = lhs.x * rhs;
    r.y = lhs.y * rhs;
    return r;
}

inline vec2
vec2_neg(vec2 v)
{
    vec2 r;
    r.x = -v.x;
    r.y = -v.y;
    return r;
}

inline float
vec2_dot(vec2 lhs, vec2 rhs)
{
    return (lhs.x * rhs.x) + (lhs.y * rhs.y);
}

inline float
vec2_cross(vec2 lhs, vec2 rhs)
{
    return (lhs.x * rhs.y) - (lhs.y * rhs.x);
}

inline float
vec2_mag(vec2 v)
{
    return sqrtf(vec2_dot(v, v));
}

inline vec2
vec2_norm(vec2 v)
{
    vec2 r;
    float mag;

    mag = vec2_mag(v);
    if (mag <= FLT_EPSILON)
        return vec2_zero();

    r.x = v.x / mag;
    r.y = v.y / mag;

    return r;
}


inline vec2
vec2_abs(vec2 v)
{
    vec2 r;
    r.x = fabsf(v.x);
    r.y = fabsf(v.y);
    return r;
}

inline vec2
vec2_lerp(vec2 from, vec2 to, float t)
{
    vec2 r;
    r.x = f32_lerp(from.x, to.x, t);
    r.y = f32_lerp(from.y, to.y, t);
    return r;
}

inline vec2
vec2_step(vec2 from, vec2 to, float step)
{
    vec2 r;
    r.x = f32_step(from.x, to.x, step);
    r.y = f32_step(from.y, to.y, step);
    return r;
}

// }}}

// vec3 {{{

struct vec3 {
    float x, y, z;
};

inline vec3
vec3_zero(void)
{
    vec3 r;
    r.x = 0.f;
    r.y = 0.f;
    r.z = 0.f;
    return r;
}

inline vec3
vec3_one(void)
{
    vec3 r;
    r.x = 1.f;
    r.y = 1.f;
    r.z = 1.f;
    return r;
}

inline vec3
vec3_add(vec3 lhs, vec3 rhs)
{
    vec3 r;
    r.x = lhs.x + rhs.x;
    r.y = lhs.y + rhs.y;
    r.z = lhs.z + rhs.z;
    return r;
}

inline vec3
vec3_sub(vec3 lhs,  vec3 rhs)
{
    vec3 r;
    r.x = lhs.x - rhs.x;
    r.y = lhs.y - rhs.y;
    r.z = lhs.z - rhs.z;
    return r;
}

inline vec3
vec3_mul(vec3 lhs, vec3 rhs)
{
    vec3 r;
    r.x = lhs.x * rhs.x;
    r.y = lhs.y * rhs.y;
    r.z = lhs.z * rhs.z;
    return r;
}

inline vec3
vec3_div(vec3 lhs, vec3 rhs)
{
    vec3 r;
    r.x = f32_eq(rhs.x, 0.f) ? 0.f : lhs.x / rhs.x;
    r.y = f32_eq(rhs.y, 0.f) ? 0.f : lhs.y / rhs.y;
    r.z = f32_eq(rhs.z, 0.f) ? 0.f : lhs.z / rhs.z;
    return r;
}

inline float
vec3_dot(vec3 lhs, vec3 rhs)
{
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
}


inline vec3
vec3_cross(vec3 lhs, vec3 rhs)
{
    vec3 r;
    r.x = (lhs.y * rhs.z) - (lhs.z * rhs.y);
    r.y = (lhs.z * rhs.x) - (lhs.x * rhs.z);
    r.z = (lhs.x * rhs.y) - (lhs.y * rhs.x);

    return r;
}

inline vec3
vec3_scale(vec3 lhs, float rhs)
{
    vec3 r;
    r.x = lhs.x * rhs;
    r.y = lhs.y * rhs;
    r.z = lhs.z * rhs;
    return r;
}

inline vec3
vec3_lerp(vec3 from, vec3 to, float t)
{
    vec3 r;
    r.x = f32_lerp(from.x, to.x, t);
    r.y = f32_lerp(from.y, to.y, t);
    r.z = f32_lerp(from.z, to.z, t);
    return r;
}

inline vec3
vec3_neg(vec3 v)
{
    vec3 r;
    r.x = -v.x;
    r.y = -v.y;
    r.z = -v.z;
    return r;
}

inline float
vec3_mag(vec3 v)
{
    return sqrtf(vec3_dot(v, v));
}

inline vec3
vec3_norm(vec3 v)
{
    float m = vec3_mag(v);
    if (m == 0)
        return v;

    v.x /= m;
    v.y /= m;
    v.z /= m;

    return v;
}

inline float
vec3_angle(vec3 v, vec3 u)
{
    float v_mag, u_mag;
    v_mag = vec3_mag(v);
    u_mag = vec3_mag(u);
    if ((v_mag <= FLT_EPSILON) || (u_mag <= FLT_EPSILON))
        return 0.f;

    return acosf(vec3_dot(v, u) / (v_mag * u_mag));
}

inline vec3
vec3_project_on(vec3 v, vec3 on)
{
    float on_mag, scaler;

    on_mag = vec3_mag(on);

    if (on_mag <= FLT_EPSILON)
        return vec3_zero();

    scaler = vec3_dot(v, on)/(on_mag);

    return vec3_scale(on, scaler);
}

inline int
vec3_eq(vec3 lhs, vec3 rhs)
{
    return f32_eq(lhs.x, rhs.x)
        && f32_eq(lhs.y, rhs.y)
        && f32_eq(lhs.z, rhs.z);
}

// }}}

// vec4 {{{

struct vec4 {
    float x, y, z, w;
};

inline vec4
vec4_zero(void)
{
    vec4 r;
    r.x = 0.f;
    r.y = 0.f;
    r.z = 0.f;
    r.w = 0.f;
    return r;
}

inline float
vec4_dot(vec4 lhs, vec4 rhs)
{
    return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z) + (lhs.w * rhs.w);
}

// }}}

// versor {{{

inline versor
versor_identity(void)
{
    versor v;
    v.x = 0.f;
    v.y = 0.f;
    v.z = 0.f;
    v.w = 1.f;
    return v;
}

inline float
versor_real(versor v)
{
    return v.w;
}

inline vec3
versor_imaginary(versor v)
{
    vec3 r;
    r.x = v.x;
    r.y = v.y;
    r.z = v.z;
    return r;
}

inline float
versor_mag(versor v)
{
    return sqrtf(vec4_dot(v, v));
}

inline versor
versor_norm(versor v)
{
    float m = versor_mag(v);
    if (m == 0)
        return versor_identity();

    v.x /= m;
    v.y /= m;
    v.z /= m;
    v.w /= m;

    return v;
}

inline versor
versor_conj(versor v)
{
    versor r;
    r.x = -v.x;
    r.y = -v.y;
    r.z = -v.z;
    r.w = v.w;
    return r;
}

inline versor
versor_mul(versor lhs, versor rhs)
{
    float l_real = versor_real(lhs);
    vec3    l_img  = versor_imaginary(lhs);

    float r_real = versor_real(rhs);
    vec3    r_img  = versor_imaginary(rhs);

    float real = (l_real * r_real) - vec3_dot(r_img, l_img);

    vec3 a, b, c;

    a = vec3_cross(r_img, l_img);
    b = vec3_scale(r_img, l_real);
    c = vec3_scale(l_img, r_real);

    vec3 imaginary;
    imaginary = vec3_add(a, b);
    imaginary = vec3_add(imaginary, c);

    versor result = {imaginary.x, imaginary.y, imaginary.z, real};
    return result;
}

inline versor
versor_from_euler(vec3 axis, float angle)
{
    angle = deg_to_rad(angle);

    versor v = (versor) {
        sinf(angle / 2.f) * axis.x,
        sinf(angle / 2.f) * axis.y,
        sinf(angle / 2.f) * axis.z,
        cosf(angle / 2.f),
    };

    return versor_norm(v);
}

inline versor
versor_around_axis_degrees(float x, float y, float z)
{
    vec3 x_axis = (vec3){1.0f, 0.0f, 0.0f};
    vec3 y_axis = (vec3){0.0f, 1.0f, 0.0f};
    vec3 z_axis = (vec3){0.0f, 0.0f, 1.0f};
    versor vx = versor_from_euler(x_axis, x);
    versor vy = versor_from_euler(y_axis, y);
    versor vz = versor_from_euler(z_axis, z);
    return versor_mul(vx, versor_mul(vy, vz));
}

#if 0
// NOTE: Here is an implementation from the GLM-lib for reference to implement versor_look_at(..)
//       https://github.com/g-truc/glm/blob/b3f87720261d623986f164b2a7f6a0a938430271/glm/gtc/quaternion.inl
template<typename T, qualifier Q>
GLM_FUNC_QUALIFIER qua<T, Q>
quatLookAtRH(vec<3, T, Q> const& direction, vec<3, T, Q> const& up)
{
    mat<3, 3, T, Q> Result;

    Result[2] = -direction;
    vec<3, T, Q> const& Right = cross(up, Result[2]);
    Result[0] = Right * inversesqrt(max(static_cast<T>(0.00001), dot(Right, Right)));
    Result[1] = cross(Result[2], Result[0]);

    return quat_cast(Result);
}

template<typename T, qualifier Q>
GLM_FUNC_QUALIFIER qua<T, Q>
quatLookAtLH(vec<3, T, Q> const& direction, vec<3, T, Q> const& up)
{
    mat<3, 3, T, Q> Result;

    Result[2] = direction;
    vec<3, T, Q> const& Right = cross(up, Result[2]);
    Result[0] = Right * inversesqrt(max(static_cast<T>(0.00001), dot(Right, Right)));
    Result[1] = cross(Result[2], Result[0]);

    return quat_cast(Result);
}
#endif

// }}}

// mat4 {{{
union mat4 {
    float m[4][4];
    vec4    v[4];
};

inline mat4
mat4_zero(void)
{
    mat4 r;
    r.v[0] = vec4_zero();
    r.v[1] = vec4_zero();
    r.v[2] = vec4_zero();
    r.v[3] = vec4_zero();
    return r;
}

inline mat4
mat4_identity(void)
{
    mat4 r = mat4_zero();
    r.m[0][0] = 1.f;
    r.m[1][1] = 1.f;
    r.m[2][2] = 1.f;
    r.m[3][3] = 1.f;
    return r;
}

inline void
mat4_translate(mat4 *m, vec3 v)
{
    m->m[3][0] += v.x;
    m->m[3][1] += v.y;
    m->m[3][2] += v.z;
}

inline void
mat4_scale(mat4 *m, vec3 v)
{
    m->m[0][0] *= v.x;
    m->m[1][1] *= v.y;
    m->m[2][2] *= v.z;
}

inline mat4
mat4_ortho(float left, float right, float bottom, float top, float n, float f)
{
    mat4 r;
    float right_left, top_bot, far_near;

    r = mat4_zero();

    right_left =  1.f / (right - left);
    top_bot    =  1.f / (top - bottom);
    far_near   = -1.f / (f - n);

    r.m[0][0] = 2.f * right_left;
    r.m[1][1] = 2.f * top_bot;
    r.m[2][2] = -2.f * far_near;
    r.m[3][0] = -(right + left) * right_left;
    r.m[3][1] = -(top + bottom) * top_bot;
    r.m[3][2] = -(f + n) * far_near;
    r.m[3][3] = 1.f;

    return r;
}

inline mat4
mat4_perspective(float fov, float aspect, float n, float f)
{
    mat4 r;
    float focal, near_far;

    r = mat4_zero();

    focal = 1.f / tanf(fov / 2.f);
    near_far = 1.f / (n - f);

    r.m[0][0] = focal / aspect;
    r.m[1][1] = focal;
    r.m[2][2] = (f + n) * near_far;
    r.m[2][3] = -1.f;
    r.m[3][2] = 2.f * f * n * near_far;

    return r;
}


inline mat4
mat4_rotation(float yaw, float pitch, float roll)
{
    //mat4 r = mat4_zero();
    mat4 r;
    float cosy, siny, cosb, sinb, cosa, sina;

    cosy = cosf(yaw);
    siny = sinf(yaw);

    cosb = cosf(pitch);
    sinb = sinf(pitch);

    cosa = cosf(roll);
    sina = sinf(roll);

    r.m[0][0] = (cosy * cosb * cosa) - (siny * sina);
    r.m[0][1] = (siny * cosb * cosa) + (cosy * sina);
    r.m[0][2] = -sinb * cosa;
    r.m[0][3] = 0.f;

    r.m[1][0] = (-cosy * cosb * sina) - (siny * cosa);
    r.m[1][1] = (-siny * cosb * sina) + (cosy * cosa);
    r.m[1][2] = (sinb * sina);
    r.m[1][3] = 0.f;

    r.m[2][0] = cosy * sina;
    r.m[2][1] = siny * sinb;
    r.m[2][2] = cosb;
    r.m[2][3] = 0.f;

    r.m[3][0] = 0.f;
    r.m[3][1] = 0.f;
    r.m[3][2] = 0.f;
    r.m[3][3] = 1.f;

    return r;
}

inline mat4
mat4_rotation_deg(float yaw, float pitch, float roll)
{
    return mat4_rotation(deg_to_rad(yaw), deg_to_rad(pitch), deg_to_rad(roll));
}

inline mat4
mat4_mul(mat4 lhs, mat4 rhs)
{
    mat4 r;

    float
    a00 = lhs.m[0][0], a01 = lhs.m[0][1], a02 = lhs.m[0][2], a03 = lhs.m[0][3],
    a10 = lhs.m[1][0], a11 = lhs.m[1][1], a12 = lhs.m[1][2], a13 = lhs.m[1][3],
    a20 = lhs.m[2][0], a21 = lhs.m[2][1], a22 = lhs.m[2][2], a23 = lhs.m[2][3],
    a30 = lhs.m[3][0], a31 = lhs.m[3][1], a32 = lhs.m[3][2], a33 = lhs.m[3][3],

    b00 = rhs.m[0][0], b01 = rhs.m[0][1], b02 = rhs.m[0][2],
    b10 = rhs.m[1][0], b11 = rhs.m[1][1], b12 = rhs.m[1][2],
    b20 = rhs.m[2][0], b21 = rhs.m[2][1], b22 = rhs.m[2][2],
    b30 = rhs.m[3][0], b31 = rhs.m[3][1], b32 = rhs.m[3][2], b33 = rhs.m[3][3];

    r.m[0][0] = a00 * b00 + a10 * b01 + a20 * b02;
    r.m[0][1] = a01 * b00 + a11 * b01 + a21 * b02;
    r.m[0][2] = a02 * b00 + a12 * b01 + a22 * b02;
    r.m[0][3] = a03 * b00 + a13 * b01 + a23 * b02;

    r.m[1][0] = a00 * b10 + a10 * b11 + a20 * b12;
    r.m[1][1] = a01 * b10 + a11 * b11 + a21 * b12;
    r.m[1][2] = a02 * b10 + a12 * b11 + a22 * b12;
    r.m[1][3] = a03 * b10 + a13 * b11 + a23 * b12;

    r.m[2][0] = a00 * b20 + a10 * b21 + a20 * b22;
    r.m[2][1] = a01 * b20 + a11 * b21 + a21 * b22;
    r.m[2][2] = a02 * b20 + a12 * b21 + a22 * b22;
    r.m[2][3] = a03 * b20 + a13 * b21 + a23 * b22;

    r.m[3][0] = a00 * b30 + a10 * b31 + a20 * b32 + a30 * b33;
    r.m[3][1] = a01 * b30 + a11 * b31 + a21 * b32 + a31 * b33;
    r.m[3][2] = a02 * b30 + a12 * b31 + a22 * b32 + a32 * b33;
    r.m[3][3] = a03 * b30 + a13 * b31 + a23 * b32 + a33 * b33;

    return r;
}

inline mat4
mat4_from_versor(versor v)
{
    mat4 r;

    //float mag = versor_mag(v);
    //float s   = (mag > 0) ? 2.0 / mag : 0;
    float s   = 2.f;

    float xx = s * v.x * v.x;
    float yy = s * v.y * v.y;
    float zz = s * v.z * v.z;

    float xy = s * v.x * v.y;
    float yz = s * v.y * v.z;
    float xz = s * v.x * v.z;

    float wx = s * v.w * v.x;
    float wy = s * v.w * v.y;
    float wz = s * v.w * v.z;

    r.m[0][0] = 1.f - yy - zz;
    r.m[0][1] = xy + wz;
    r.m[0][2] = xz - wy;
    r.m[0][3] = 0.f;

    r.m[1][0] = xy - wz;
    r.m[1][1] = 1.f - xx - zz;
    r.m[1][2] = yz + wx;
    r.m[1][3] = 0.f;

    r.m[2][0] = xz + wy;
    r.m[2][1] = yz - wx;
    r.m[2][2] = 1.f - xx - yy;
    r.m[2][3] = 0.f;

    r.m[3][0] = 0.f;
    r.m[3][1] = 0.f;
    r.m[3][2] = 0.f;
    r.m[3][3] = 1.f;

    return r;
}

inline mat4
mat4_scale_rotate_translate(vec3 scale, versor rotation, vec3 translate)
{
    mat4 s = mat4_identity();
    mat4_scale(&s, scale);
    mat4 r = mat4_from_versor(rotation);
    mat4 t = mat4_mul(s, r);
    mat4_translate(&t, translate);
    return t;
}

inline mat4
mat4_lookat(vec3 eye, vec3 target, vec3 up)
{
    vec3 f, s, u;
    f = vec3_sub(target, eye);
    f = vec3_norm(f);

    s = vec3_cross(f, vec3_norm(up));
    s = vec3_norm(s);

    u = vec3_cross(s, f);

    mat4 result;
    result.m[0][0] = s.x;
    result.m[0][1] = u.x;
    result.m[0][2] =-f.x;
    result.m[1][0] = s.y;
    result.m[1][1] = u.y;
    result.m[1][2] =-f.y;
    result.m[2][0] = s.z;
    result.m[2][1] = u.z;
    result.m[2][2] =-f.z;
    result.m[3][0] =-vec3_dot(s, eye);
    result.m[3][1] =-vec3_dot(u, eye);
    result.m[3][2] = vec3_dot(f, eye);
    result.m[0][3] = result.m[1][3] = result.m[2][3] = 0.0f;
    result.m[3][3] = 1.0f;
    return result;
}

// }}}

//ivec2 {{{

union ivec2 {
    struct {int x, y;};
    struct {int w, h;};
};

inline ivec2
ivec2_zero (void)
{
    ivec2 r;
    r.x = 0;
    r.y = 0;
    return r;
}

inline ivec2
ivec2_add(ivec2 lhs,  ivec2 rhs)
{
    ivec2 r;
    r.x = lhs.x + rhs.x;
    r.y = lhs.y + rhs.y;
    return r;
}

inline void
ivec2_addeq(ivec2 *lhs, ivec2 rhs)
{
    lhs->x += rhs.x;
    lhs->y += rhs.y;
}

inline ivec2
ivec2_sub(ivec2 lhs, ivec2 rhs)
{
    ivec2 r;
    r.x = lhs.x - rhs.x;
    r.y = lhs.y - rhs.y;
    return r;
}

inline void
ivec2_subeq(ivec2 *lhs, ivec2 rhs)
{
    lhs->x -= rhs.x;
    lhs->y -= rhs.y;
}

inline ivec2
ivec2_mul(ivec2 lhs, ivec2 rhs)
{
    ivec2 r;
    r.x = lhs.x * rhs.x;
    r.y = lhs.y * rhs.y;
    return r;
}

inline ivec2
ivec2_div(ivec2 lhs, ivec2 rhs)
{
    ivec2 r;
    r.x = (rhs.x == 0) ? 0 : lhs.x / rhs.x;
    r.y = (rhs.y == 0) ? 0 : lhs.y / rhs.y;
    return r;
}

inline ivec2
ivec2_scale(ivec2 lhs, int rhs)
{
    ivec2 r;
    r.x = lhs.x * rhs;
    r.y = lhs.y * rhs;
    return r;
}

inline ivec2
ivec2_neg(ivec2 v)
{
    ivec2 r;
    r.x = -v.x;
    r.y = -v.y;
    return r;
}

inline vec2
ivec2_to_vec2(ivec2 v)
{
    vec2 r;
    r.x = (float)v.x;
    r.y = (float)v.y;
    return r;
}

//}}}

// ivec3 {{{

struct ivec3 {
    int x, y, z;
};

inline ivec3
ivec3_zero (void)
{
    ivec3 r;
    r.x = 0;
    r.y = 0;
    r.z = 0;
    return r;
}

inline ivec3
ivec3_add(ivec3 lhs,  ivec3 rhs)
{
    ivec3 r;
    r.x = lhs.x + rhs.x;
    r.y = lhs.y + rhs.y;
    r.z = lhs.z + rhs.z;
    return r;
}

inline void
ivec3_addeq(ivec3 *lhs, ivec3 rhs)
{
    lhs->x += rhs.x;
    lhs->y += rhs.y;
    lhs->z += rhs.z;
}

inline ivec3
ivec3_sub(ivec3 lhs, ivec3 rhs)
{
    ivec3 r;
    r.x = lhs.x - rhs.x;
    r.y = lhs.y - rhs.y;
    r.z = lhs.z - rhs.z;
    return r;
}

inline void
ivec3_subeq(ivec3 *lhs, ivec3 rhs)
{
    lhs->x -= rhs.x;
    lhs->y -= rhs.y;
    lhs->z -= rhs.z;
}

inline ivec3
ivec3_mul(ivec3 lhs, ivec3 rhs)
{
    ivec3 r;
    r.x = lhs.x * rhs.x;
    r.y = lhs.y * rhs.y;
    r.z = lhs.z * rhs.z;
    return r;
}

inline ivec3
ivec3_div(ivec3 lhs, ivec3 rhs)
{
    ivec3 r;
    r.x = (rhs.x == 0) ? 0 : lhs.x / rhs.x;
    r.y = (rhs.y == 0) ? 0 : lhs.y / rhs.y;
    r.z = (rhs.z == 0) ? 0 : lhs.z / rhs.z;
    return r;
}

inline ivec3
ivec3_scale(ivec3 lhs, int rhs)
{
    ivec3 r;
    r.x = lhs.x * rhs;
    r.y = lhs.y * rhs;
    r.z = lhs.z * rhs;
    return r;
}

inline int
ivec3_eq(ivec3 lhs, ivec3 rhs)
{
    return (lhs.x == rhs.x)
        && (lhs.y == rhs.y)
        && (lhs.z == rhs.z);
}

inline ivec3
ivec3_neg(ivec3 v)
{
    ivec3 r;
    r.x = -v.x;
    r.y = -v.y;
    r.z = -v.z;
    return r;
}
// }}}

// utils {{{

inline float
deg_to_rad(float deg)
{
    return deg * M_PI / 180.0f;
}

inline float
rad_to_deg(float rad)
{
    return rad * 180.f / M_PI;
}

inline int
i32_xy_to_index(int x, int y, int width)
{
    return x + (width*y);
}

inline ivec2
i32_index_to_xy(int index, int width)
{
    ivec2 result;
    result.x = index % width;
    result.y = index / width;
    return result;
}

inline void
f32_swap(float *__restrict a, float *__restrict b)
{
    float t;
    t  = *a;
    *a = *b;
    *b = t;
}

inline float
f32_lerp(float from, float to, float t)
{
    return from + t * (to - from);
}

inline float
f32_max(float a, float b)
{
    return (a > b) ? a : b;
}

inline int
f32_eq(float a, float b)
{
    return fabsf(a - b) <= FLT_EPSILON;
    ///return fabsf(a - b) <= 0.00000000001f;
}

inline float
f32_step(float from, float to, float step)
{
    float diff = to - from;

    if (f32_eq(diff, 0.0f) || (fabsf(diff) < fabsf(step)))
    {
        return to;
    }
    else
    {
        return from + (SIGNF(diff) * step);
    }
}

inline int
i32_step(int from, int to, int step)
{
    int diff = to - from;
    if (diff == 0)
    {
        return to;
    }
    else
    {
        return from + (SIGN(diff) * step);
    }
}

// }}}

// quad {{{

struct quad {
  vec2 min;
  vec2 max;
};

static quad
quad_from_size(vec2 top_left, vec2 size)
{
  quad q;
  q.min = top_left;
  q.max = vec2_add(top_left, size);
  return q;
}

static int
quad_collision_point(quad q, vec2 p)
{
  return q.min.x < p.x
      && q.max.x > p.x
      && q.min.y < p.y
      && q.max.y > p.y;
}

static int
quad_collision_quad(quad q0, quad q1)
{
  return q0.min.x < q1.max.x
      && q0.max.x > q1.min.x
      && q0.min.y < q1.max.y
      && q0.max.y > q1.min.y;
}

// }}} quad

