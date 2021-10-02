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
    
static int cb_ref = LUA_NOREF;

static void NearCallback(void *data, geom_t o1, geom_t o2)
/* This version implements the logic as in the ODE manual, and invokes
 * the user callback only when the two objects are both geoms (not spaces)
 */
    {
#define L moonode_L
    (void)data; /* not used */
    if(cb_ref==LUA_NOREF) return;
    if(dGeomIsSpace(o1) || dGeomIsSpace(o2))
        {
        if(dGeomIsSpace(o1)) dSpaceCollide((space_t)o1, data, NearCallback);
        if(dGeomIsSpace(o2)) dSpaceCollide((space_t)o2, data, NearCallback);
        }
    else /* two geometries, so handle them to the user callback */
        {
        if(!userdata(o1)) { unexpected(L); return; }
        if(!userdata(o2)) { unexpected(L); return; }
        lua_rawgeti(L, LUA_REGISTRYINDEX, cb_ref);
        pushxxx(L, o1);
        pushxxx(L, o2);
        if(lua_pcall(L, 2, 0, 0) != LUA_OK)
            { lua_error(L); return; }
        }
    return;
#undef L
    }

static void NearCallback2(void *data, geom_t o1, geom_t o2)
    {
#define L moonode_L
    (void)data; /* not used */
    if(cb_ref==LUA_NOREF) return;
    if(!userdata(o1)) { unexpected(L); return; }
    if(!userdata(o2)) { unexpected(L); return; }
    lua_rawgeti(L, LUA_REGISTRYINDEX, cb_ref);
    pushxxx(L, o1);
    pushxxx(L, o2);
    if(lua_pcall(L, 2, 0, 0) != LUA_OK)
        { lua_error(L); return; }
    return;
#undef L
    }

static int SetNearCallback(lua_State *L)
    {
    if(lua_isnoneornil(L, 1)) /* remove callback */
        {
        if(cb_ref!=LUA_NOREF)
            Unreference(L, cb_ref);
        return 0;
        }
    if(!lua_isfunction(L, 1))
        return argerror(L, 1, ERR_FUNCTION);
    Reference(L, 1, cb_ref);
    return 0;
    }

#define MAX_CONTACTS        256

static int Collide(lua_State *L)
    {
    int n, i;
    contact_point_t contacts[MAX_CONTACTS];
    geom_t o1 = checkgeomorspace(L, 1, NULL);
    geom_t o2 = checkgeomorspace(L, 2, NULL);
    int max_contacts = luaL_checkinteger(L, 3);
    int flags = optflags(L, 4, 0); /* collideflags */
    if(max_contacts<1 || max_contacts > MAX_CONTACTS)
        return argerror(L, 3, ERR_RANGE);
    n = dCollide(o1, o2, (flags&0xffff0000) | max_contacts, contacts, sizeof(contact_point_t));
    if(n==0)
        {
        lua_pushboolean(L, 0);
        return 1;
        }
    lua_pushboolean(L, 1);
    lua_newtable(L);
    for(i=0; i<n; i++)
        {
        pushcontactpoint(L, &contacts[i]);
        lua_rawseti(L, -2, i+1);
        }
    return 2;
    }

static int SpaceCollide(lua_State *L)
    {
    space_t space;
    space = checkspace(L, 1, NULL);
    dSpaceCollide(space, NULL, NearCallback);
    return 0;
    }

static int SpaceCollide2(lua_State *L)
    {
    space_t space;
    geom_t o1, o2;
    if(lua_isnoneornil(L, 2))
        {
        space = checkspace(L, 1, NULL);
        dSpaceCollide(space, NULL, NearCallback2);
        }
    else
        {
        o1 = checkgeomorspace(L, 1, NULL);
        o2 = checkgeomorspace(L, 2, NULL);
        dSpaceCollide2(o1, o2, NULL, NearCallback2);
        }
    return 0;
    }

static const struct luaL_Reg Functions[] = 
    {
        { "set_near_callback", SetNearCallback },
        { "collide", Collide },
        { "space_collide", SpaceCollide },
        { "space_collide2", SpaceCollide2 },
        { NULL, NULL } /* sentinel */
    };

void moonode_open_collide(lua_State *L)
    {
    luaL_setfuncs(L, Functions, 0);
    }

#if 0
// Are these needed?
//void dClosestLineSegmentPoints (const dVector3 a1, const dVector3 a2, const dVector3 b1, const dVector3 b2, dVector3 cp1, dVector3 cp2);
//int dBoxTouchesBox (const dVector3 _p1, const dMatrix3 R1, const dVector3 side1, const dVector3 _p2, const dMatrix3 R2, const dVector3 side2);
//int dBoxBox(const vec3_t p1, const mat3_t R1, const vec3_t side1, const vec3_t p2, const mat3_t R2, const vec3_t side2, vec3_t normal, double *depth, int *return_code, int flags, dContactGeom *contact, int skip);
#endif

