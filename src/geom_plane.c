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
    if(!freeuserdata(L, ud, "geom_plane")) return 0;
    geomdestroy(L, geom);
    return 0;
    }

static int newgeom(lua_State *L, geom_t geom)
    {
    ud_t *ud;
    ud = newuserdata(L, geom, GEOM_PLANE_MT, "geom_plane");
    ud->parent_ud = NULL;
    ud->destructor = freegeom;
    return 1;
    }

static int Create(lua_State *L) // NOTE: this is a 'non-placeable' class
    {
    space_t space = optspace(L, 1, NULL);
    double a = luaL_checknumber(L, 2);
    double b = luaL_checknumber(L, 3);
    double c = luaL_checknumber(L, 4);
    double d = luaL_checknumber(L, 5);
    geom_t geom = dCreatePlane(space, a, b, c, d);
    return newgeom(L, geom);
    }

static int SetParams(lua_State *L)
    {
    geom_t geom = checkgeom_plane(L, 1, NULL);
    double a = luaL_checknumber(L, 2);
    double b = luaL_checknumber(L, 3);
    double c = luaL_checknumber(L, 4);
    double d = luaL_checknumber(L, 5);
    dGeomPlaneSetParams(geom, a, b, c, d);
    return 0;
    }

static int GetParams(lua_State *L)
    {
    vec4_t params;
    geom_t geom = checkgeom_plane(L, 1, NULL);
    dGeomPlaneGetParams(geom, params);
    lua_pushnumber(L, params[0]);
    lua_pushnumber(L, params[1]);
    lua_pushnumber(L, params[2]);
    lua_pushnumber(L, params[3]);
    return 4;
    }

static int PointDepth(lua_State *L)
    {
    vec3_t pos;
    geom_t geom = checkgeom_plane(L, 1, NULL);
    checkvec3(L, 2, pos);
    lua_pushnumber(L, dGeomPlanePointDepth(geom, pos[0], pos[1], pos[2]));
    return 1;
    }

static int Basis(lua_State *L) 
// we don't call it 'space' to avoid confusion with space objects
    {
    vec4_t params;
    vec3_t n, p, q;
    geom_t geom = checkgeom_plane(L, 1, NULL);
    dGeomPlaneGetParams(geom, params);
    n[0] = params[0];
    n[1] = params[1];
    n[2] = params[2];
    dPlaneSpace(n, p, q);
    pushvec3(L, n);
    pushvec3(L, p);
    pushvec3(L, q);
    return 3;
    }

DESTROY_FUNC(geom)

static const struct luaL_Reg Methods[] = 
    {
        { "set", SetParams },
        { "get", GetParams },
        { "point_depth", PointDepth },
        { "basis", Basis },
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
        { "create_plane", Create },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_geom_plane(lua_State *L)
    {
    udata_define(L, GEOM_PLANE_MT, Methods, MetaMethods);
    udata_inherit(L, GEOM_PLANE_MT, GEOM_MT);
    luaL_setfuncs(L, Functions, 0);
    }

