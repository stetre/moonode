/* The MIT License (MIT)
 *
 * Copyright (c) 2021 Stefano Trettel
 *
 * Software repository: MoonODE, https://github.com/stetre/moonode
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "internal.h"

/*-----------------------------------------------------------------------------*
 | Rotations utilities
 *-----------------------------------------------------------------------------*/

static int RIdentity(lua_State *L)
    {
    mat3_t r;
    dRSetIdentity(r);
    pushmat3(L, r);
    return 1;
    }

static int RFromAxisAndAngle(lua_State *L)
    {
    mat3_t r;
    vec3_t axis;
    double angle;
    checkvec3(L, 1, axis);
    angle = luaL_checknumber(L, 2);
    dRFromAxisAndAngle(r, axis[0], axis[1], axis[2], angle);
    pushmat3(L, r);
    return 1;
    }

static int RFromEulerAngles(lua_State *L)
    {
    mat3_t r;
    double phi = luaL_checknumber(L, 1);
    double theta = luaL_checknumber(L, 2);
    double psi = luaL_checknumber(L, 3);
    dRFromEulerAngles(r, phi, theta, psi);
    pushmat3(L, r);
    return 1;
    }

static int RFrom2Axes(lua_State *L)
    {
    mat3_t r;
    vec3_t a, b;
    checkvec3(L, 1, a);
    checkvec3(L, 2, b);
    dRFrom2Axes(r, a[0], a[1], a[2], b[0], b[1], b[2]);
    pushmat3(L, r);
    return 1;
    }

static int RFromZAxis(lua_State *L)
    {
    mat3_t r;
    vec3_t a;
    checkvec3(L, 1, a);
    dRFromZAxis(r, a[0], a[1], a[2]);
    pushmat3(L, r);
    return 1;
    }

static int QSetIdentity(lua_State *L)
    {
    quat_t q;
    dQSetIdentity(q);
    pushquat(L, q);
    return 1;
    }

static int QFromAxisAndAngle(lua_State *L)
    {
    quat_t q;
    vec3_t axis;
    double angle;
    checkvec3(L, 1, axis);
    angle = luaL_checknumber(L, 2);
    dQFromAxisAndAngle(q, axis[0], axis[1], axis[2], angle);
    pushquat(L, q);
    return 1;
    }

#define F(Func, func)                       \
static int Func(lua_State *L)               \
    {                                       \
    quat_t q, a, b;                         \
    checkquat(L, 1, a);                     \
    checkquat(L, 2, b);                     \
    func(q, a, b);                          \
    pushquat(L, q);                         \
    return 1;                               \
    }
F(QMultiply0, dQMultiply0)
F(QMultiply1, dQMultiply1)
F(QMultiply2, dQMultiply2)
F(QMultiply3, dQMultiply3)
#undef F


static int RfromQ(lua_State *L)
    {
    mat3_t r;
    quat_t q;
    checkquat(L, 1, q);
    dRfromQ(r, q);
    pushmat3(L, r);
    return 1;
    }

static int QfromR(lua_State *L)
    {
    mat3_t r;
    quat_t q;
    checkmat3(L, 1, r);
    dQfromR(q, r);
    pushquat(L, q);
    return 1;
    }

static int DQfromW(lua_State *L)
    {
    vec3_t v;
    quat_t q, dq;
    checkvec3(L, 1, v);
    checkquat(L, 2, q);
    dDQfromW((double*)dq, v, q);
    pushquat(L, dq);
    return 1;
    }

static int OrthogonalizeR(lua_State *L)
    {
    int res;
    mat3_t r;
    checkmat3(L, 1, r);
    res = dOrthogonalizeR(r);
    if(!res) 
        lua_pushnil(L);
    else
        pushmat3(L, r);
    return 1;
    }

/*-----------------------------------------------------------------------------*
 | Math utilities
 *-----------------------------------------------------------------------------*/

static int SafeNormalize3(lua_State *L)
    {
    int res;
    vec3_t v;
    checkvec3(L, 1, v);
    res = dSafeNormalize3(v);
    if(!res)
        lua_pushnil(L);
    else
        pushvec3(L, v);
    return 1;
    }

static int SafeNormalize4(lua_State *L)
    {
    int res;
    vec4_t v;
    checkvec4(L, 1, v);
    res = dSafeNormalize4(v);
    if(!res)
        lua_pushnil(L);
    else
        pushvec4(L, v);
    return 1;
    }

static int Normalize3(lua_State *L)
    {
    vec3_t v;
    checkvec3(L, 1, v);
    dNormalize3(v);
    pushvec3(L, v);
    return 1;
    }

static int Normalize4(lua_State *L)
    {
    vec4_t v;
    checkvec4(L, 1, v);
    dNormalize4(v);
    pushvec4(L, v);
    return 1;
    }

static int PlaneSpace(lua_State *L)
    {
    vec3_t n, p, q;
    checkvec3(L, 1, n);
    dPlaneSpace(n, p, q);
    pushvec3(L, p);
    pushvec3(L, q);
    return 2;
    }

static int MZero3(lua_State *L)
    {
    mat3_t m;
    dZeroMatrix3(m);
    pushmat3(L, m);
    return 1;
    }

static int MZero4(lua_State *L)
    {
    mat4_t m;
    dZeroMatrix4(m);
    pushmat4(L, m);
    return 1;
    }

static int MIdentity3(lua_State *L)
    {
    mat3_t m;
    dZeroMatrix3(m);
    m[0] = m[5] = m[10] = 1.0f;
    pushmat3(L, m);
    return 1;
    }

static int MIdentity4(lua_State *L)
    {
    mat4_t m;
    dZeroMatrix4(m);
    m[0] = m[5] = m[10] = m[15] = 1.0f;
    pushmat4(L, m);
    return 1;
    }

static int VAdd3(lua_State *L)
    {
    vec3_t a, b, r;
    checkvec3(L, 1, a);
    checkvec3(L, 2, b);
    dAddVectors3(r, a, b);
    pushvec3(L, r);
    return 1;
    }

static int VSub3(lua_State *L)
    {
    vec3_t a, b, r;
    checkvec3(L, 1, a);
    checkvec3(L, 2, b);
    dSubtractVectors3(r, a, b);
    pushvec3(L, r);
    return 1;
    }

static int VAddScaled3(lua_State *L)
    {
    vec3_t a, b, r;
    double sa, sb;
    checkvec3(L, 1, a);
    checkvec3(L, 2, b);
    sa = luaL_checknumber(L, 3);
    sb = luaL_checknumber(L, 4);
    dAddScaledVectors3(r, a, b, sa, sb);
    pushvec3(L, r);
    return 1;
    }

static int VAddThreeScaled3(lua_State *L)
    {
    vec3_t a, b, c, r;
    double sa, sb, sc;
    checkvec3(L, 1, a);
    checkvec3(L, 2, b);
    checkvec3(L, 3, c);
    sa = luaL_checknumber(L, 4);
    sb = luaL_checknumber(L, 5);
    sc = luaL_checknumber(L, 6);
    dAddThreeScaledVectors3(r, a, b, c, sa, sb, sc);
    pushvec3(L, r);
    return 1;
    }

static int VScale3(lua_State *L)
    {
    vec3_t a;
    double s;
    checkvec3(L, 1, a);
    s = luaL_checknumber(L, 2);
    dScaleVector3(a, s);
    pushvec3(L, a);
    return 1;
    }

static int VNegate3(lua_State *L)
    {
    vec3_t a;
    checkvec3(L, 1, a);
    dNegateVector3(a);
    pushvec3(L, a);
    return 1;
    }

static int VCopy3(lua_State *L)
    {
    vec3_t r, a;
    checkvec3(L, 1, a);
    dCopyVector3(r, a);
    pushvec3(L, r);
    return 1;
    }

static int VCopy4(lua_State *L)
    {
    vec4_t r, a;
    checkvec4(L, 1, a);
    dCopyVector4(r, a);
    pushvec4(L, r);
    return 1;
    }

static int MCopy3(lua_State *L)
    {
    mat3_t r, a;
    checkmat3(L, 1, a);
    memcpy(r, a, sizeof(mat3_t));
    pushmat3(L, r);
    return 1;
    }

static int MCopy4(lua_State *L)
    {
    mat4_t r, a;
    checkmat4(L, 1, a);
    memcpy(r, a, sizeof(mat4_t));
    pushmat4(L, r);
    return 1;
    }

static int MGetColumn3(lua_State *L)
    {
    mat3_t m;
    vec3_t r;
    int n;
    checkmat3(L, 1, m);
    n = luaL_checkinteger(L, 2);
    if(n<0 || n > 3) return argerror(L, 2, ERR_RANGE);
    dGetMatrixColumn3(r, m, n-1);
    pushvec3(L, r);
    return 1;
    }

static int VLength3(lua_State *L)
    {
    vec3_t v;
    double r;
    checkvec3(L, 1, v);
    r = dCalcVectorLength3(v);
    lua_pushnumber(L, r);
    return 1;
    }

static int VLengthSquare3(lua_State *L)
    {
    vec3_t v;
    double r;
    checkvec3(L, 1, v);
    r = dCalcVectorLengthSquare3(v);
    lua_pushnumber(L, r);
    return 1;
    }

#if 0
static int PointDepth3(lua_State *L)
    {
    double r;
    vec3_t p;
    vec4_t plane_p, plane_n;
    checkvec3(L, 1, p);
    checkvec4(L, 2, plane_p);
    checkvec4(L, 3, plane_n);
    r = dCalcPointDepth3(p, plane_p, plane_n);
    lua_pushnumber(L, r);
    return 1;
    }
#endif

static int PointsDistance3(lua_State *L)
    {
    double r;
    vec3_t a, b;
    checkvec3(L, 1, a);
    checkvec3(L, 2, b);
    r = dCalcPointsDistance3(a, b);
    lua_pushnumber(L, r);
    return 1;
    }

static int VDot3(lua_State *L)
    {
    double r;
    vec3_t a, b;
    checkvec3(L, 1, a);
    checkvec3(L, 2, b);
    r = dCalcVectorDot3(a, b);
    lua_pushnumber(L, r);
    return 1;
    }


static int VCross3(lua_State *L)
    {
    vec3_t a, b, r;
    checkvec3(L, 1, a);
    checkvec3(L, 2, b);
    dCalcVectorCross3(r, a, b);
    pushvec3(L, r);
    return 1;
    }



static int MDet3(lua_State *L)
    {
    double r;
    mat3_t m;
    checkmat3(L, 1, m);
    r = dCalcMatrix3Det(m);
    lua_pushnumber(L, r);
    return 1;
    }

static int MInverse3(lua_State *L)
    {
    mat3_t r, m;
    checkmat3(L, 1, m);
    if(dInvertMatrix3(r, m) == 0) 
        lua_pushnil(L); // singular matrix
    else
        pushmat3(L, r);
    return 1;
    }

static int VMaxDifference3(lua_State *L)
    {
    double r;
    vec3_t a, b;
    checkvec3(L, 1, a);
    checkvec3(L, 2, b);
    r = dMaxDifference(a, b, 1, 3);
    lua_pushnumber(L, r);
    return 1;
    }

static int VMaxDifference4(lua_State *L)
    {
    double r;
    vec4_t a, b;
    checkvec4(L, 1, a);
    checkvec4(L, 2, b);
    r = dMaxDifference(a, b, 1, 4);
    lua_pushnumber(L, r);
    return 1;
    }

static int MMaxDifference3(lua_State *L)
    {
    double r;
    mat3_t a, b;
    checkmat3(L, 1, a);
    checkmat3(L, 2, b);
    r = dMaxDifference(a, b, 1, 3);
    lua_pushnumber(L, r);
    return 1;
    }

static int MMaxDifference4(lua_State *L)
    {
    double r;
    mat4_t a, b;
    checkmat4(L, 1, a);
    checkmat4(L, 2, b);
    r = dMaxDifference(a, b, 1, 4);
    lua_pushnumber(L, r);
    return 1;
    }

static const struct luaL_Reg Functions[] = 
    {
        { "r_identity", RIdentity },
        { "r_from_axis_and_angle", RFromAxisAndAngle },
        { "r_from_euler_angles", RFromEulerAngles },
        { "r_from_2_axes", RFrom2Axes },
        { "r_from_z_axis", RFromZAxis },
        { "q_identity", QSetIdentity },
        { "q_from_axis_and_angle", QFromAxisAndAngle },
        { "q_multiply0", QMultiply0 },
        { "q_multiply1", QMultiply1 },
        { "q_multiply2", QMultiply2 },
        { "q_multiply3", QMultiply3 },
        { "r_from_q", RfromQ },
        { "q_from_r", QfromR },
        { "dq_from_w", DQfromW },
        { "r_orthogonalize", OrthogonalizeR },
        { "v_safe_normalize3", SafeNormalize3 },
        { "v_safe_normalize4", SafeNormalize4 },
        { "v_normalize3", Normalize3 },
        { "v_normalize4", Normalize4 },
        { "plane_basis", PlaneSpace },
        { "m_zero3", MZero3 },
        { "m_zero4", MZero4 },
        { "m_identity3", MIdentity3 },
        { "m_identity4", MIdentity4 },
        { "v_add3", VAdd3 },
        { "v_sub3", VSub3 },
        { "v_add_scaled3", VAddScaled3 },
        { "v_add_three_scaled3", VAddThreeScaled3 },
        { "v_scale3", VScale3 },
        { "v_negate3", VNegate3 },
        { "v_copy3", VCopy3 },
        { "v_copy4", VCopy4 },
        { "m_copy3", MCopy3 },
        { "m_copy4", MCopy4 },
        { "m_get_column3", MGetColumn3 },
        { "v_length3", VLength3 },
        { "v_length_square3", VLengthSquare3 },
//      { "point_depth3", PointDepth3 },
        { "v_distance3", PointsDistance3 },
        { "v_dot3", VDot3 },
        { "v_cross3", VCross3 },
        { "m_det3", MDet3 },
        { "m_inverse3", MInverse3 },
        { "v_max_difference3", VMaxDifference3 },
        { "v_max_difference4", VMaxDifference4 },
        { "m_max_difference3", MMaxDifference3 },
        { "m_max_difference4", MMaxDifference4 },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_misc(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

#if 0
// Are these needed?
// dReal dMaxDifferenceLowerTriangle(const dReal *A, const dReal *B, int n);
// void dClearUpperTriangle(dReal *A, int n);
// void dAddVectorCross3(dReal *res, const dReal *a, const dReal *b)
// void dSubtractVectorCross3(dReal *res, const dReal *a, const dReal *b)
// void dSetCrossMatrixPlus(dReal *res, const dReal *a, unsigned skip)
// void dSetCrossMatrixMinus(dReal *res, const dReal *a, unsigned skip)
// void dSetZero(dReal *a, int n);
// void dSetValue(dReal *a, int n, dReal value);
// dReal dDot(const dReal *a, const dReal *b, int n);
// void dMultiply0(dReal *A, const dReal *B, const dReal *C, int p,int q,int r);
// void dMultiply1(dReal *A, const dReal *B, const dReal *C, int p,int q,int r);
// void dMultiply2(dReal *A, const dReal *B, const dReal *C, int p,int q,int r);
// int dFactorCholesky(dReal *A, int n);
// void dSolveCholesky(const dReal *L, dReal *b, int n);
// int dInvertPDMatrix(const dReal *A, dReal *Ainv, int n);
// int dIsPositiveDefinite(const dReal *A, int n);
// void dFactorLDLT(dReal *A, dReal *d, int n, int nskip);
// void dSolveL1(const dReal *L, dReal *b, int n, int nskip);
// void dSolveL1T(const dReal *L, dReal *b, int n, int nskip);
// void dScaleVector(dReal *a, const dReal *d, int n);
// void dSolveLDLT(const dReal *L, const dReal *d, dReal *b, int n, int nskip);
// void dLDLTAddTL(dReal *L, dReal *d, const dReal *a, int n, int nskip);
// void dLDLTRemove(dReal **A, const int *p, dReal *L, dReal *d,
// void dRemoveRowCol(dReal *A, int n, int nskip, int r);
#endif
