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

/* We need to keep data around because ODE doesn't seem to copy it */
typedef struct {
    double *planes;
    double *points;
    unsigned int *polygons;
} info_t;

#define FreeInfo(L, info) do {                                  \
    if(info) {                                                  \
        Free((L), (info)->points); (info)->points = NULL;       \
        Free((L), (info)->planes); (info)->planes = NULL;       \
        Free((L), (info)->polygons); (info)->polygons = NULL;   \
    }                                                           \
} while(0)

static int freegeom(lua_State *L, ud_t *ud)
    {
    geom_t geom = (geom_t)ud->handle;
    info_t *info = (info_t*)ud->info;
    ud->info = NULL; /* to prevent automatic release */
    if(!freeuserdata(L, ud, "geom_convex")) return 0;
    FreeInfo(L, info);
    Free(L, info);
    geomdestroy(L, geom);
    return 0;
    }

static int newgeom(lua_State *L, geom_t geom, info_t* info)
    {
    ud_t *ud;
    ud = newuserdata(L, geom, GEOM_CONVEX_MT, "geom_convex");
    ud->info = info;
    ud->parent_ud = NULL;
    ud->destructor = freegeom;
    return 1;
    }

static int Create(lua_State *L)
    {
    int err;
    int planecount, pointcount;
    unsigned int polygoncount;
    geom_t geom = NULL;
    space_t space = checkspace(L, 1, NULL);
    info_t* info = Malloc(L, sizeof(info_t));
#define Cleanup() do { FreeInfo(L, info); Free(L, info); } while(0)
    info->planes = (double*)checkvec4list(L, 2, &planecount, &err);
    if(err) { Cleanup(); return argerror(L, 2, err); }
    info->points = (double*)checkvec3list(L, 3, &pointcount, &err);
    if(err) { Cleanup(); return argerror(L, 3, err); }
    info->polygons = checkpolygons(L, 4, &polygoncount, &err);
    if(err) { Cleanup(); return argerror(L, 4, err); }
    geom = dCreateConvex(space, info->planes, planecount, info->points, pointcount, info->polygons);
#undef Cleanup
    return newgeom(L, geom, info);
    }

static int Set(lua_State *L)
    {
    int err;
    int planecount, pointcount;
    unsigned int polygoncount;
    ud_t *ud;
    geom_t geom = checkgeom_convex(L, 1, &ud);
    info_t *info = (info_t*)ud->info;
    FreeInfo(L, info);
    info->planes = (double*)checkvec4list(L, 2, &planecount, &err);
    if(err) return argerror(L, 2, err);
    info->points = (double*)checkvec3list(L, 3, &pointcount, &err);
    if(err) return argerror(L, 3, err);
    info->polygons = checkpolygons(L, 4, &polygoncount, &err);
    if(err) return argerror(L, 4, err);
    dGeomSetConvex(geom, info->planes, planecount, info->points, pointcount, info->polygons);
    return 0;
    }

DESTROY_FUNC(geom)

static const struct luaL_Reg Methods[] = 
    {
        { "set", Set },
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
        { "create_convex", Create },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_geom_convex(lua_State *L)
    {
    udata_define(L, GEOM_CONVEX_MT, Methods, MetaMethods);
    udata_inherit(L, GEOM_CONVEX_MT, GEOM_MT);
    luaL_setfuncs(L, Functions, 0);
    }

