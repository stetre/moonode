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
    if(!freeuserdata(L, ud, "geom_sphere")) return 0;
    geomdestroy(L, geom);
    return 0;
    }

static int newgeom(lua_State *L, geom_t geom)
    {
    ud_t *ud;
    ud = newuserdata(L, geom, GEOM_SPHERE_MT, "geom_sphere");
    ud->parent_ud = NULL;
    ud->destructor = freegeom;
    return 1;
    }

static int Create(lua_State *L)
    {
    space_t space = optspace(L, 1, NULL);
    double radius = luaL_checknumber(L, 2);
    geom_t geom = dCreateSphere(space, radius);
    return newgeom(L, geom);
    }

static int SetRadius(lua_State *L)
    {
    geom_t geom = checkgeom_sphere(L, 1, NULL);
    double radius = luaL_checknumber(L, 2);
    dGeomSphereSetRadius(geom, radius);
    return 0;
    }

static int GetRadius(lua_State *L)
    {
    geom_t geom = checkgeom_sphere(L, 1, NULL);
    double radius = dGeomSphereGetRadius(geom);
    lua_pushnumber(L, radius);
    return 1;
    }

static int PointDepth(lua_State *L)
    {
    vec3_t pos;
    geom_t geom = checkgeom_sphere(L, 1, NULL);
    checkvec3(L, 2, pos);
    lua_pushnumber(L, dGeomSpherePointDepth(geom, pos[0], pos[1], pos[2]));
    return 1;
    }

DESTROY_FUNC(geom)

static const struct luaL_Reg Methods[] = 
    {
        { "set_radius", SetRadius },
        { "get_radius", GetRadius },
        { "point_depth", PointDepth },
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
        { "create_sphere", Create },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_geom_sphere(lua_State *L)
    {
    udata_define(L, GEOM_SPHERE_MT, Methods, MetaMethods);
    udata_inherit(L, GEOM_SPHERE_MT, GEOM_MT);
    luaL_setfuncs(L, Functions, 0);
    }

