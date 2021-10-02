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

int spacedestroy(lua_State *L, space_t space)
/* Wrapper for dSpaceDestroy(), also destroys the contained geoms in case the
 * cleanup mode is set to 1 ('true'), which is the default.
 * From the ODE manual:
 *      "When a space is destroyed, if its cleanup mode is 1 (the default)
 *      then all the geoms in that space are automatically destroyed as well."
 * If this is the case, we need to destroy the contained geoms before destroyng the
 * space, otherwise we'd keep around userdata for geoms that are already destroyed
 * at the ODE lib level, which will eventually cause a segv (because the pointers
 * stored in the userdata are no longer valid).
 */
    {
    int n;
    geom_t geom;
    ud_t *ud;
    if(dSpaceGetCleanup(space))
        {
        while(1)
            {
            /* Note that we can not iterate for i=1 to n, here, because
             * we are destroying the geoms. Instead, we keep deleting the
             * first geom (i=0) and until n becomes 0. */
            n = dSpaceGetNumGeoms(space);
            if(n==0) break;
            geom = dSpaceGetGeom(space, 0);
            ud = userdata(geom);
            printf("spacedestroy geom=%p ud=%p\n", (void*)geom, (void*)ud);
            if(ud) ud->destructor(L, ud);
            }
        }
    dSpaceDestroy(space);
    return 0;
    }

static int SetCleanup(lua_State *L)
    {
    space_t space = checkspace(L, 1, NULL);
    int mode = checkboolean(L, 2);
    dSpaceSetCleanup(space, mode);
    return 0;
    }

static int GetCleanup(lua_State *L)
    {
    space_t space = checkspace(L, 1, NULL);
    int mode = dSpaceGetCleanup(space);
    lua_pushboolean(L, mode);
    return 1;
    }

static int IsSpace(lua_State *L)
    {
    (void)checkspace(L, 1, NULL);
    lua_pushboolean(L, 1); // no need to call dGeomIsSpace() ...
    return 1;
    }

static int GetClass(lua_State *L)
    {
    space_t space = checkspace(L, 1, NULL);
    pushgeomtype(L, dSpaceGetClass(space));
    return 1;
    }

static int SetSublevel(lua_State *L)
    {
    space_t space = checkspace(L, 1, NULL);
    int sublevel = luaL_checkinteger(L, 2);
    dSpaceSetSublevel(space, sublevel);
    return 0;
    }

static int GetSublevel(lua_State *L)
    {
    space_t space = checkspace(L, 1, NULL);
    lua_pushinteger(L, dSpaceGetSublevel(space));
    return 1;
    }

static int Add(lua_State *L)
    {
    space_t space = checkspace(L, 1, NULL);
    geom_t geom = checkgeom(L, 2, NULL);
    dSpaceAdd(space, geom);
    return 0;
    }

static int Remove(lua_State *L)
    {
    space_t space = checkspace(L, 1, NULL);
    geom_t geom = checkgeom(L, 2, NULL);
    dSpaceRemove(space, geom);
    return 0;
    }

static int Query(lua_State *L)
    {
    space_t space = checkspace(L, 1, NULL);
    geom_t geom = checkgeom(L, 2, NULL);
    lua_pushboolean(L, dSpaceQuery(space , geom));
    return 1;
    }

static int GetNumGeoms(lua_State *L)
    {
    space_t space = checkspace(L, 1, NULL);
    lua_pushinteger(L, dSpaceGetNumGeoms(space));
    return 1;
    }

static int GetGeom(lua_State *L)
    {
    space_t space = checkspace(L, 1, NULL);
    int i = checkindex(L, 2);
    geom_t geom = dSpaceGetGeom(space , i);
    ud_t *ud = userdata(geom);
    if(!ud) lua_pushnil(L);
    else pushuserdata(L, ud);
    return 1;
    }

static int GetGeoms(lua_State *L)
// returns all geoms at once, in a table
    {
    int i, j;
    geom_t geom;
    ud_t *ud;
    space_t space = checkspace(L, 1, NULL);
    int n = dSpaceGetNumGeoms(space);
    lua_newtable(L);
    j=1;
    for(i=0; i<n; i++)
        {
        geom = dSpaceGetGeom(space , i);
        ud = userdata(geom);
        if(ud) 
            {
            pushuserdata(L, ud);
            lua_rawseti(L, -2, j++);
            }
        }
    return 1;
    }

RAW_FUNC(space)
DESTROY_FUNC(space)

static const struct luaL_Reg Methods[] = 
    {
        { "raw", Raw },
        { "is_space", IsSpace },
        { "get_type", GetClass },
        { "set_cleanup", SetCleanup },
        { "get_cleanup", GetCleanup },
        { "add", Add },
        { "remove", Remove },
        { "query", Query },
        { "get_num_geoms", GetNumGeoms },
        { "get_geom", GetGeom },
        { "get_geoms", GetGeoms },
        { "set_sublevel", SetSublevel },
        { "get_sublevel", GetSublevel },
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


void moonode_open_space(lua_State *L)
    {
    udata_define(L, SPACE_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

#if 0 
//Not used:
//void dSpaceSetManualCleanup(space_t space, int mode);
//int dSpaceGetManualCleanup(space_t space);
//void dSpaceClean(space_t); (not documented)
#endif
