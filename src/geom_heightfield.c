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
    if(!freeuserdata(L, ud, "geom_heightfield")) return 0;
    geomdestroy(L, geom);
    return 0;
    }

static int newgeom(lua_State *L, geom_t geom)
    {
    ud_t *ud;
    ud = newuserdata(L, geom, GEOM_HEIGHTFIELD_MT, "geom_heightfield");
    ud->parent_ud = NULL;
    ud->destructor = freegeom;
    return 1;
    }

static int Create(lua_State *L)
    {
    space_t space = optspace(L, 1, NULL);
    hfdata_t hfdata = checkhfdata(L, 2, NULL);
    int placeable = checkboolean(L, 3);
    geom_t geom = dCreateHeightfield(space, hfdata, placeable);
    return newgeom(L, geom);
    }

static int SetHfData(lua_State *L)
    {
    geom_t geom = checkgeom_heightfield(L, 1, NULL);
    hfdata_t hfdata = opthfdata(L, 2, NULL);
    dGeomHeightfieldSetHeightfieldData(geom, hfdata);
    return 0;
    }

static int GetHfData(lua_State *L)
    {
    geom_t geom = checkgeom_heightfield(L, 1, NULL);
    hfdata_t hfdata = dGeomHeightfieldGetHeightfieldData(geom);
    if(!hfdata) lua_pushnil(L);
    else pushhfdata(L, hfdata);
    return 1;
    }


DESTROY_FUNC(geom)

static const struct luaL_Reg Methods[] = 
    {
        { "set_data", SetHfData },
        { "get_data", GetHfData },
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
        { "create_heightfield", Create },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_geom_heightfield(lua_State *L)
    {
    udata_define(L, GEOM_HEIGHTFIELD_MT, Methods, MetaMethods);
    udata_inherit(L, GEOM_HEIGHTFIELD_MT, GEOM_MT);
    luaL_setfuncs(L, Functions, 0);
    }

