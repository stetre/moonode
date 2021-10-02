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
    if(!freeuserdata(L, ud, "geom_cylinder")) return 0;
    geomdestroy(L, geom);
    return 0;
    }

static int newgeom(lua_State *L, geom_t geom)
    {
    ud_t *ud;
    ud = newuserdata(L, geom, GEOM_CYLINDER_MT, "geom_cylinder");
    ud->parent_ud = NULL;
    ud->destructor = freegeom;
    return 1;
    }

static int Create(lua_State *L)
    {
    space_t space = optspace(L, 1, NULL);
    double radius = luaL_checknumber(L, 2);
    double length = luaL_checknumber(L, 3);
    geom_t geom = dCreateCylinder(space, radius, length);
    return newgeom(L, geom);
    }

static int SetParams(lua_State *L)
    {
    geom_t geom = checkgeom_cylinder(L, 1, NULL);
    double radius = luaL_checknumber(L, 2);
    double length = luaL_checknumber(L, 3);
    dGeomCylinderSetParams(geom, radius, length);
    return 0;
    }

static int GetParams(lua_State *L)
    {
    double radius, length;
    geom_t geom = checkgeom_cylinder(L, 1, NULL);
    dGeomCylinderGetParams(geom, &radius, &length);
    lua_pushnumber(L, radius);
    lua_pushnumber(L, length);
    return 2;
    }

DESTROY_FUNC(geom)

static const struct luaL_Reg Methods[] = 
    {
        { "set", SetParams },
        { "get", GetParams },
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
        { "create_cylinder", Create },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_geom_cylinder(lua_State *L)
    {
    udata_define(L, GEOM_CYLINDER_MT, Methods, MetaMethods);
    udata_inherit(L, GEOM_CYLINDER_MT, GEOM_MT);
    luaL_setfuncs(L, Functions, 0);
    }

