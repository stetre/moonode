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
    if(!freeuserdata(L, ud, "geom_ray")) return 0;
    geomdestroy(L, geom);
    return 0;
    }

static int newgeom(lua_State *L, geom_t geom)
    {
    ud_t *ud;
    ud = newuserdata(L, geom, GEOM_RAY_MT, "geom_ray");
    ud->parent_ud = NULL;
    ud->destructor = freegeom;
    return 1;
    }

static int Create(lua_State *L)
    {
    space_t space = optspace(L, 1, NULL);
    double length = luaL_checknumber(L, 2);
    geom_t geom = dCreateRay(space, length);
    return newgeom(L, geom);
    }

static int SetLength(lua_State *L)
    {
    geom_t geom = checkgeom_ray(L, 1, NULL);
    double length = luaL_checknumber(L, 2);
    dGeomRaySetLength(geom, length);
    return 0;
    }

static int GetLength(lua_State *L)
    {
    geom_t geom = checkgeom_ray(L, 1, NULL);
    double length = dGeomRayGetLength(geom);
    lua_pushnumber(L, length);
    return 1;
    }

static int Set(lua_State *L)
    {
    vec3_t p, dir;
    geom_t geom = checkgeom_ray(L, 1, NULL);
    checkvec3(L, 2, p);
    checkvec3(L, 3, dir);
    dGeomRaySet(geom, p[0], p[1], p[2], dir[0], dir[1], dir[2]);
    return 0;
    }

static int Get(lua_State *L)
    {
    vec3_t p, dir;
    geom_t geom = checkgeom_ray(L, 1, NULL);
    dGeomRayGet(geom, p, dir);
    pushvec3(L, p);
    pushvec3(L, dir);
    return 2;
    }


#define F(Func, func)                           \
static int Func(lua_State *L)                   \
    {                                           \
    geom_t geom = checkgeom_ray(L, 1, NULL);    \
    int val = checkboolean(L, 2);               \
    func(geom, val);                            \
    return 0;                                   \
    }
F(SetFirstContact, dGeomRaySetFirstContact)
F(SetBackfaceCull, dGeomRaySetBackfaceCull)
F(SetClosestHit, dGeomRaySetClosestHit)
#undef F


#define F(Func, func)                           \
static int Func(lua_State *L)                   \
    {                                           \
    geom_t geom = checkgeom_ray(L, 1, NULL);    \
    lua_pushboolean(L, func(geom));             \
    return 1;                                   \
    }
F(GetFirstContact, dGeomRayGetFirstContact)
F(GetBackfaceCull, dGeomRayGetBackfaceCull)
F(GetClosestHit, dGeomRayGetClosestHit)
#undef F


DESTROY_FUNC(geom)

static const struct luaL_Reg Methods[] = 
    {
        { "set_length", SetLength },
        { "get_length", GetLength },
        { "set", Set },
        { "get", Get },
        { "set_first_contact", SetFirstContact },
        { "set_backface_cull", SetBackfaceCull },
        { "set_closest_hit", SetClosestHit },
        { "get_first_contact", GetFirstContact },
        { "get_backface_cull", GetBackfaceCull },
        { "get_closest_hit", GetClosestHit },
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
        { "create_ray", Create },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_geom_ray(lua_State *L)
    {
    udata_define(L, GEOM_RAY_MT, Methods, MetaMethods);
    udata_inherit(L, GEOM_RAY_MT, GEOM_MT);
    luaL_setfuncs(L, Functions, 0);
    }

