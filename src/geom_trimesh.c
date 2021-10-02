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

static int freegeom(lua_State *L, ud_t *ud)
    {
    geom_t geom = (geom_t)ud->handle;
    if(!freeuserdata(L, ud, "geom_trimesh")) return 0;
    geomdestroy(L, geom);
    return 0;
    }

static int newgeom(lua_State *L, geom_t geom)
    {
    ud_t *ud;
    ud = newuserdata(L, geom, GEOM_TRIMESH_MT, "geom_trimesh");
    ud->parent_ud = NULL;
    ud->destructor = freegeom;
    return 1;
    }

static int Create(lua_State *L)
    {
    space_t space = checkspace(L, 1, NULL);
    tmdata_t tmdata = checktmdata(L, 2, NULL);
    geom_t geom = dCreateTriMesh(space, tmdata, NULL, NULL, NULL); /* set callbacks separately */
    return newgeom(L, geom);
    }

static int SetData(lua_State *L)
    {
    geom_t geom = checkgeom_trimesh(L, 1, NULL);
    tmdata_t tmdata = checktmdata(L, 2, NULL);
    dGeomTriMeshSetData(geom, tmdata);
    return 0;
    }

static int GetData(lua_State *L)
    {
    geom_t geom = checkgeom_trimesh(L, 1, NULL);
    tmdata_t tmdata = dGeomTriMeshGetData(geom);
    pushtmdata(L, tmdata);
    return 1;
    }

static int SetLastTransform(lua_State *L)
    {
    mat4_t transform;
    geom_t geom = checkgeom_trimesh(L, 1, NULL);
    checkmat4(L, 2, transform);
    dGeomTriMeshSetLastTransform(geom, transform);
    return 0;
    }

static int GetLastTransform(lua_State *L)
    {
    geom_t geom = checkgeom_trimesh(L, 1, NULL);
    const double *transform = dGeomTriMeshGetLastTransform(geom);
    pushmat4(L, transform);
    return 1;
    }

int dGeomTriMeshGetTriangleCount(geom_t g);  /* missing prototype in headers? */
static int GetTriangleCount(lua_State *L)
    {
    geom_t geom = checkgeom_trimesh(L, 1, NULL);
    int count = dGeomTriMeshGetTriangleCount(geom);
    lua_pushinteger(L, count);
    return 1;
    }

static int GetTriangle(lua_State *L)
    {
    vec3_t v0, v1, v2;
    geom_t geom = checkgeom_trimesh(L, 1, NULL);
    int index = checkindex(L, 2);
    dGeomTriMeshGetTriangle(geom, index, &v0, &v1, &v2);
    pushvec3(L, v0);
    pushvec3(L, v1);
    pushvec3(L, v2);
    return 3;
    }

static int GetPoint(lua_State *L)
    {
    vec3_t point;
    geom_t geom = checkgeom_trimesh(L, 1, NULL);
    int index = checkindex(L, 2);
    double u = luaL_checknumber(L, 3);
    double v = luaL_checknumber(L, 4);
    dGeomTriMeshGetPoint(geom, index, u, v, point);
    pushvec3(L, point);
    return 1;
    }

#if 0 //@@ do we need these?
static int EnableTC(lua_State *L)
    {
    geom_t geom = checkgeom_trimesh(L, 1, NULL);
    int geomclass = checkgeomtype(L, 2);
    dGeomTriMeshEnableTC(geom, geomclass, 1);
    return 0;
    }

static int DisableTC(lua_State *L)
    {
    geom_t geom = checkgeom_trimesh(L, 1, NULL);
    int geomclass = checkgeomtype(L, 2);
    dGeomTriMeshEnableTC(geom, geomclass, 0);
    return 0;
    }

static int IsTCEnabled(lua_State *L)
    {
    geom_t geom = checkgeom_trimesh(L, 1, NULL);
    int geomclass = checkgeomtype(L, 2);
    lua_pushboolean(L, dGeomTriMeshIsTCEnabled(geom, geomclass));
    return 1;
    }

static int ClearTCCache(lua_State *L)
    {
    geom_t geom = checkgeom_trimesh(L, 1, NULL);
    dGeomTriMeshClearTCCache(geom);
    return 0;
    }

static int Callback(geom_t geom, geom_t object, int index)
    {
#define L moonode_L
    int retval;
    int top = lua_gettop(L);
    ud_t *ud = userdata(geom);
    if(!ud) { unexpected(L); return 0; }
    lua_rawgeti(L, LUA_REGISTRYINDEX, ud->ref1);
    pushgeom(L, geom);
    pushgeom(L, object);
    pushindex(L, index);
    if(lua_pcall(L, 3, 1, 0) != LUA_OK)
        { lua_error(L); return 0; }
    retval = lua_toboolean(L, -1);
    lua_settop(L, top);
    return retval;
#undef L
    }

static int SetCallback(lua_State *L)
    {
    ud_t *ud;
    geom_t geom = checkgeom_trimesh(L, 1, &ud);
    if(lua_isnoneornil(L, 2)) /* remove callback */
        {
        if(ud->ref1!=LUA_NOREF)
            {
            dGeomTriMeshSetCallback(geom, NULL);
            Unreference(L, ud->ref1);
            }
        return 0;
        }
    if(!lua_isfunction(L, 2))
        return argerror(L, 2, ERR_FUNCTION);
    Reference(L, 2, ud->ref1);
    dGeomTriMeshSetCallback(geom, Callback);
    return 0;
    }

static int GetCallback(lua_State *L)
    {
    ud_t *ud;
    (void)checkgeom_trimesh(L, 1, &ud);
    lua_rawgeti(L, LUA_REGISTRYINDEX, ud->ref1);
    return 1;
    }


static int RayCallback(geom_t geom, geom_t ray, int index, double u, double v)
    {
#define L moonode_L
    int retval;
    int top = lua_gettop(L);
    ud_t *ud = userdata(geom);
    if(!ud) { unexpected(L); return 0; }
    lua_rawgeti(L, LUA_REGISTRYINDEX, ud->ref2);
    pushgeom(L, geom);
    pushgeom(L, ray);
    pushindex(L, index);
    lua_pushnumber(L, u);
    lua_pushnumber(L, v);
    if(lua_pcall(L, 5, 1, 0) != LUA_OK)
        { lua_error(L); return 0; }
    retval = lua_toboolean(L, -1);
    lua_settop(L, top);
    return retval;
#undef L
    }

static int SetRayCallback(lua_State *L)
    {
    ud_t *ud;
    geom_t geom = checkgeom_trimesh(L, 1, &ud);
    if(lua_isnoneornil(L, 2)) /* remove callback */
        {
        if(ud->ref2!=LUA_NOREF)
            {
            dGeomTriMeshSetRayCallback(geom, NULL);
            Unreference(L, ud->ref2);
            }
        return 0;
        }
    if(!lua_isfunction(L, 2))
        return argerror(L, 2, ERR_FUNCTION);
    Reference(L, 2, ud->ref2);
    dGeomTriMeshSetRayCallback(geom, RayCallback);
    return 0;
    }

static int GetRayCallback(lua_State *L)
    {
    ud_t *ud;
    (void)checkgeom_trimesh(L, 1, &ud);
    lua_rawgeti(L, LUA_REGISTRYINDEX, ud->ref2);
    return 1;
    }
#endif

DESTROY_FUNC(geom)

static const struct luaL_Reg Methods[] = 
    {
        { "set_data", SetData },
        { "get_data", GetData },
        { "set_last_transform", SetLastTransform },
        { "get_last_transform", GetLastTransform },
        { "get_triangle_count", GetTriangleCount },
        { "get_triangle", GetTriangle },
        { "get_point", GetPoint },
//      { "set_callback", SetCallback },
//      { "get_callback", GetCallback },
//      { "set_ray_callback", SetRayCallback },
//      { "get_ray_callback", GetRayCallback },
//      { "enable_tc", EnableTC },
//      { "disable_tc", DisableTC },
//      { "is_tc_enabled", IsTCEnabled },
//      { "clear_tc_cache", ClearTCCache },
        { "destroy", Destroy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  Destroy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "create_trimesh", Create },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_geom_trimesh(lua_State *L)
    {
    udata_define(L, GEOM_TRIMESH_MT, Methods, MetaMethods);
    udata_inherit(L, GEOM_TRIMESH_MT, GEOM_MT);
    luaL_setfuncs(L, Functions, 0);
    }

