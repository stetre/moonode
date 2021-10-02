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

int geomdestroy(lua_State *L, geom_t geom)
/* Wrapper for dGeomDestroy() */
    {
    (void)L;
    dGeomDestroy(geom);
    return 0;
    }
 
static int IsSpace(lua_State *L)
    {
    (void)checkgeom(L, 1, NULL);
    lua_pushboolean(L, 0); // no need to call dGeomIsSpace() ...
    return 1;
    }

static int GetClass(lua_State *L)
    {
    geom_t geom = checkgeom(L, 1, NULL);
    pushgeomtype(L, dGeomGetClass(geom));
    return 1;
    }

static int GetSpace(lua_State *L)
    {
    geom_t geom = checkgeom(L, 1, NULL);
    space_t space = dGeomGetSpace(geom);
    if(!space) lua_pushnil(L);
    else pushspace(L, space);
    return 1;
    }

static int Enable(lua_State *L)
    {
    geom_t geom = checkgeom(L, 1, NULL);
    dGeomEnable(geom);
    return 0;
    }

static int Disable(lua_State *L)
    {
    geom_t geom = checkgeom(L, 1, NULL);
    dGeomDisable(geom);
    return 0;
    }

static int IsEnabled(lua_State *L)
    {
    geom_t geom = checkgeom(L, 1, NULL);
    lua_pushboolean(L, dGeomIsEnabled(geom));
    return 1;
    }

#define F(Func, func)                               \
static int Func(lua_State *L)                       \
    {                                               \
    geom_t geom = checkgeom(L, 1, NULL);            \
    unsigned long bits = luaL_checkinteger(L, 2);   \
    func(geom, bits);                               \
    return 0;                                       \
    }
F(SetCategoryBits, dGeomSetCategoryBits)
F(SetCollideBits, dGeomSetCollideBits)
#undef F

#define F(Func, func)                               \
static int Func(lua_State *L)                       \
    {                                               \
    geom_t geom = checkgeom(L, 1, NULL);            \
    unsigned long bits = func(geom);                \
    lua_pushinteger(L, bits);                       \
    return 1;                                       \
    }
F(GetCategoryBits, dGeomGetCategoryBits)
F(GetCollideBits, dGeomGetCollideBits)
#undef F

static int SetBody(lua_State *L)
    {
    geom_t geom = checkgeom(L, 1, NULL);
    body_t body = optbody(L, 2, NULL);
    dGeomSetBody(geom, body);
    return 0;
    }

static int GetBody(lua_State *L)
    {
    geom_t geom = checkgeom(L, 1, NULL);
    body_t body = dGeomGetBody(geom);
    if(!body) lua_pushnil(L);
    else pushbody(L, body);
    return 1;
    }

static int SetPosition(lua_State *L)
    {
    vec3_t val;
    geom_t geom = checkgeom(L, 1, NULL);
    checkvec3(L, 2, val);
    dGeomSetPosition(geom, val[0], val[1], val[2]);
    return 0;
    }

static int GetPosition(lua_State *L)
    {
    vec3_t res;
    geom_t geom = checkgeom(L, 1, NULL);
    dGeomCopyPosition(geom, res);
    pushvec3(L, res);
    return 1;
    }

static int SetRotation(lua_State *L)
    {
    mat3_t val;
    geom_t geom = checkgeom(L, 1, NULL);
    checkmat3(L, 2, val);
    dGeomSetRotation(geom, val);
    return 0;
    }

static int GetRotation(lua_State *L)
    {
    mat3_t res;
    geom_t geom = checkgeom(L, 1, NULL);
    dGeomCopyRotation(geom, res);
    pushmat3(L, res);
    return 1;
    }

static int SetQuaternion(lua_State *L)
    {
    quat_t val;
    geom_t geom = checkgeom(L, 1, NULL);
    checkquat(L, 2, val);
    dGeomSetQuaternion(geom, val);
    return 0;
    }

static int GetQuaternion(lua_State *L)
    {
    quat_t res;
    geom_t geom = checkgeom(L, 1, NULL);
    dGeomGetQuaternion(geom, res);
    pushquat(L, res);
    return 1;
    }

static int GetAABB(lua_State *L)
    {
    box3_t aabb;
    geom_t geom = checkgeom(L, 1, NULL);
    dGeomGetAABB(geom, aabb);
    pushbox3(L, aabb);
    return 1;
    }

#define F(Func, func)                           \
static int Func(lua_State *L)                   \
    {                                           \
    vec3_t val, res;                            \
    geom_t geom = checkgeom(L, 1, NULL);        \
    checkvec3(L, 2, val);                       \
    func(geom, val[0], val[1], val[2], res);    \
    pushvec3(L, res);                           \
    return 0;                                   \
    }
F(GetRelPointPos, dGeomGetRelPointPos)
F(GetPosRelPoint, dGeomGetPosRelPoint)
F(VectorToWorld, dGeomVectorToWorld)
F(VectorFromWorld, dGeomVectorFromWorld)
#undef F

static int SetOffsetPosition(lua_State *L)
    {
    vec3_t val;
    geom_t geom = checkgeom(L, 1, NULL);
    checkvec3(L, 2, val);
    dGeomSetOffsetPosition(geom, val[0], val[1], val[2]);
    return 0;
    }

static int GetOffsetPosition(lua_State *L)
    {
    vec3_t res;
    geom_t geom = checkgeom(L, 1, NULL);
    dGeomCopyOffsetPosition(geom, res);
    pushvec3(L, res);
    return 1;
    }

static int SetOffsetRotation(lua_State *L)
    {
    mat3_t val;
    geom_t geom = checkgeom(L, 1, NULL);
    checkmat3(L, 2, val);
    dGeomSetOffsetRotation(geom, val);
    return 0;
    }

static int GetOffsetRotation(lua_State *L)
    {
    mat3_t res;
    geom_t geom = checkgeom(L, 1, NULL);
    dGeomCopyOffsetRotation(geom, res);
    pushmat3(L, res);
    return 1;
    }

static int SetOffsetQuaternion(lua_State *L)
    {
    quat_t val;
    geom_t geom = checkgeom(L, 1, NULL);
    checkquat(L, 2, val);
    dGeomSetOffsetQuaternion(geom, val);
    return 0;
    }

static int GetOffsetQuaternion(lua_State *L)
    {
    quat_t res;
    geom_t geom = checkgeom(L, 1, NULL);
    dGeomGetOffsetQuaternion(geom, res);
    pushquat(L, res);
    return 1;
    }


static int SetOffsetWorldPosition(lua_State *L)
    {
    vec3_t val;
    geom_t geom = checkgeom(L, 1, NULL);
    checkvec3(L, 2, val);
    dGeomSetOffsetWorldPosition(geom, val[0], val[1], val[2]);
    return 0;
    }

static int SetOffsetWorldRotation(lua_State *L)
    {
    mat3_t val;
    geom_t geom = checkgeom(L, 1, NULL);
    checkmat3(L, 2, val);
    dGeomSetOffsetWorldRotation(geom, val);
    return 0;
    }

static int SetOffsetWorldQuaternion(lua_State *L)
    {
    quat_t val;
    geom_t geom = checkgeom(L, 1, NULL);
    checkquat(L, 2, val);
    dGeomSetOffsetWorldQuaternion(geom, val);
    return 0;
    }

static int ClearOffset(lua_State *L)
    {
    geom_t geom = checkgeom(L, 1, NULL);
    dGeomClearOffset(geom);
    return 0;
    }

static int IsOffset(lua_State *L)
    {
    geom_t geom = checkgeom(L, 1, NULL);
    lua_pushboolean(L, dGeomIsOffset(geom));
    return 1;
    }

RAW_FUNC(geom)
DESTROY_FUNC(geom)

static const struct luaL_Reg Methods[] = 
    {
        { "raw", Raw },
        { "is_space", IsSpace },
        { "get_type", GetClass },
#if 1 //@@ Are these functions supposed to be used also on spaces?
        { "get_space", GetSpace },
        { "enable", Enable },
        { "disable", Disable },
        { "is_enabled", IsEnabled },
        { "set_category_bits", SetCategoryBits },
        { "set_collide_bits", SetCollideBits },
        { "get_category_bits", GetCategoryBits },
        { "get_collide_bits", GetCollideBits },
#endif
        { "set_body", SetBody },
        { "get_body", GetBody },
        { "set_position", SetPosition },
        { "get_position", GetPosition },
        { "set_rotation", SetRotation },
        { "get_rotation", GetRotation },
        { "set_quaternion", SetQuaternion },
        { "get_quaternion", GetQuaternion },
        { "get_aabb", GetAABB },
        { "get_rel_point_pos", GetRelPointPos },
        { "get_pos_rel_point", GetPosRelPoint },
        { "vector_to_world", VectorToWorld },
        { "vector_from_world", VectorFromWorld },
        { "set_offset_position", SetOffsetPosition },
        { "get_offset_position", GetOffsetPosition },
        { "set_offset_rotation", SetOffsetRotation },
        { "get_offset_rotation", GetOffsetRotation },
        { "set_offset_quaternion", SetOffsetQuaternion },
        { "get_offset_quaternion", GetOffsetQuaternion },
        { "set_offset_world_position", SetOffsetWorldPosition },
        { "set_offset_world_rotation", SetOffsetWorldRotation },
        { "set_offset_world_quaternion", SetOffsetWorldQuaternion },
        { "clear_offset", ClearOffset },
        { "is_offset", IsOffset },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc", Destroy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { NULL, NULL } /* sentinel */
    };

void moonode_open_geom(lua_State *L)
    {
    udata_define(L, GEOM_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

#if 0
//Not used:
//void dGeomSetData(geom_t geom, void* data);
//void *dGeomGetData(geom_t geom);
//int dGeomLowLevelControl(geom_t geom, int controlClass, int controlCode, void *dataValue, int *dataSize);
//Custom geom classes
typedef void dGetAABBFn(geom_t , double aabb[6]);
typedef int dColliderFn(geom_t o1, geom_t o2, int flags, dContactGeom *contact, int skip);
typedef dColliderFn * dGetColliderFnFn(int num);
typedef void dGeomDtorFn(geom_t o);
typedef int dAABBTestFn(geom_t o1, geom_t o2, double aabb[6]);
typedef struct dGeomClass {
  int bytes;
  dGetColliderFnFn *collider;
  dGetAABBFn *aabb;
  dAABBTestFn *aabb_test;
  dGeomDtorFn *dtor;
} dGeomClass;
int dCreateGeomClass(const dGeomClass *classptr);
void * dGeomGetClassData(geom_t);
geom_t dCreateGeom(int classnum);
void dSetColliderOverride(int i, int j, dColliderFn *fn);
void dInfiniteAABB(geom_t geom, double aabb[6]); (see manual)
#endif
