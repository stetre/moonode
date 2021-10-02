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

#ifndef jointDEFINED
#define jointDEFINED

/* Common functions for joints */

#define SET_PARAM(Func, func, what) /* SetParam */                  \
static int Func(lua_State *L)                                       \
    {                                                               \
    joint_t joint = checkjoint_##what(L, 1, NULL);                  \
    int param = checkparam(L, 2);                                   \
    double val = luaL_checknumber(L, 3);                            \
    int group = luaL_optinteger(L, 4, 1); /* 1, 2, or 3 */          \
    if(group < 1 || group > 3)                                      \
        return argerror(L, 4, ERR_RANGE);                           \
    param = param + dParamGroup*(group-1);                          \
    func(joint, param, val);                                        \
    return 0;                                                       \
    }

#define GET_PARAM(Func, func, what) /* GetParam */                  \
static int Func(lua_State *L)                                       \
    {                                                               \
    double val;                                                     \
    joint_t joint = checkjoint_##what(L, 1, NULL);                  \
    int param = checkparam(L, 2);                                   \
    int group = luaL_optinteger(L, 3, 1); /* 1, 2, or 3 */          \
    if(group < 1 || group > 3)                                      \
        return argerror(L, 3, ERR_RANGE);                           \
    param = param + dParamGroup*(group-1);                          \
    val = func(joint, param);                                       \
    lua_pushnumber(L, val);                                         \
    return 1;                                                       \
    }                                                               \

#define VOIDVOID(Func, func, what) /* void f(joint) */      \
static int Func(lua_State *L)                               \
    {                                                       \
    joint_t joint = checkjoint_##what(L, 1, NULL);          \
    func(joint);                                            \
    return 0;                                               \
    }

#define SETINT(Func, func, what) /* void f(joint, int) */\
static int Func(lua_State *L)                               \
    {                                                       \
    joint_t joint = checkjoint_##what(L, 1, NULL);          \
    int val = luaL_checkinteger(L, 2);                      \
    func(joint, val);                                       \
    return 0;                                               \
    }

#define SETNUM(Func, func, what) /* void f(joint, double) */\
static int Func(lua_State *L)                               \
    {                                                       \
    joint_t joint = checkjoint_##what(L, 1, NULL);          \
    double val = luaL_checknumber(L, 2);                    \
    func(joint, val);                                       \
    return 0;                                               \
    }

#define SETNUMNUM(Func, func, what) /* void f(joint, double, double) */\
static int Func(lua_State *L)                               \
    {                                                       \
    joint_t joint = checkjoint_##what(L, 1, NULL);          \
    double val1 = luaL_checknumber(L, 2);                   \
    double val2 = luaL_checknumber(L, 3);                   \
    func(joint, val1, val2);                                \
    return 0;                                               \
    }

#define SETVEC3(Func, func, what) /* void f(joint, x, y, z) */\
static int Func(lua_State *L)                               \
    {                                                       \
    vec3_t val;                                             \
    joint_t joint = checkjoint_##what(L, 1, NULL);          \
    checkvec3(L, 2, val);                                   \
    func(joint, val[0], val[1], val[2]);                    \
    return 0;                                               \
    }

#define SETVEC3VEC3(Func, func, what) /* void f(joint, x, y, z, x1, y1, z1) */\
static int Func(lua_State *L)                               \
    {                                                       \
    vec3_t val1, val2;                                      \
    joint_t joint = checkjoint_##what(L, 1, NULL);          \
    checkvec3(L, 2, val1);                                  \
    checkvec3(L, 3, val2);                                  \
    func(joint, val1[0], val1[1], val1[2], val2[0], val2[1], val2[2]);  \
    return 0;                                               \
    }

#define SETVEC3NUM(Func, func, what) /* void f(joint, x, y, z, double) */\
static int Func(lua_State *L)                               \
    {                                                       \
    vec3_t val1;                                            \
    double val2;                                            \
    joint_t joint = checkjoint_##what(L, 1, NULL);          \
    checkvec3(L, 2, val1);                                  \
    val2 = luaL_checknumber(L, 3);                          \
    func(joint, val1[0], val1[1], val1[2], val2);           \
    return 0;                                               \
    }

#define SETVEC3NUMNUM(Func, func, what) /* void f(joint, x, y, z, double, double) */\
static int Func(lua_State *L)                               \
    {                                                       \
    vec3_t val1;                                            \
    double val2, val3;                                      \
    joint_t joint = checkjoint_##what(L, 1, NULL);          \
    checkvec3(L, 2, val1);                                  \
    val2 = luaL_checknumber(L, 3);                          \
    val3 = luaL_checknumber(L, 4);                          \
    func(joint, val1[0], val1[1], val1[2], val2, val3);     \
    return 0;                                               \
    }

#define GETINT(Func, func, what) /* int f(joint)    */      \
static int Func(lua_State *L)                               \
    {                                                       \
    joint_t joint = checkjoint_##what(L, 1, NULL);          \
    int res = func(joint);                                  \
    lua_pushinteger(L, res);                                \
    return 1;                                               \
    }

#define GETNUM(Func, func, what) /* double f(joint) */      \
static int Func(lua_State *L)                               \
    {                                                       \
    joint_t joint = checkjoint_##what(L, 1, NULL);          \
    double res =    func(joint);                            \
    lua_pushnumber(L, res);                                 \
    return 1;                                               \
    }

#define GETNUMNUM(Func, func, what) /* void f(joint, double*, double*)  */  \
static int Func(lua_State *L)                               \
    {                                                       \
    double res1, res2;                                      \
    joint_t joint = checkjoint_##what(L, 1, NULL);          \
    func(joint, &res1, &res2);                              \
    lua_pushnumber(L, res1);                                \
    lua_pushnumber(L, res2);                                \
    return 2;                                               \
    }

#define GETVEC3(Func, func, what) /* void f(joint, vec3)    */  \
static int Func(lua_State *L)                               \
    {                                                       \
    vec3_t res;                                             \
    joint_t joint = checkjoint_##what(L, 1, NULL);          \
    func(joint, res);                                       \
    pushvec3(L, res);                                       \
    return 1;                                               \
    }

#define GETVEC3VEC3(Func, func, what) /* void f(joint, vec3, vec3)  */  \
static int Func(lua_State *L)                               \
    {                                                       \
    vec3_t res1, res2;                                      \
    joint_t joint = checkjoint_##what(L, 1, NULL);          \
    func(joint, res1, res2);                                \
    pushvec3(L, res1);                                      \
    pushvec3(L, res2);                                      \
    return 2;                                               \
    }

#endif /* jointDEFINED */
