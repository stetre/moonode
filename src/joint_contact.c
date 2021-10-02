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
#include "joint.h"

static int freejoint(lua_State *L, ud_t *ud)
    {
    joint_t joint = (joint_t)ud->handle;
    if(!freeuserdata(L, ud, "joint_contact")) return 0;
    jointdestroy(L, joint);
    return 0;
    }

static int newjoint(lua_State *L, joint_t joint, ud_t *world_ud, int groupid)
    {
    ud_t *ud;
    ud = newuserdata(L, joint, JOINT_CONTACT_MT, "joint_contact");
    ud->parent_ud = world_ud;
    ud->destructor = freejoint;
    ud->groupid = groupid;
    return 1;
    }

static int PackSurfaceParameters(lua_State *L)
    {
    surface_parameters_t surface;
    checksurfaceparameters(L, 1, &surface);
    lua_pushlstring(L, (char*)&surface, sizeof(surface_parameters_t));
    return 1;
    }

static int DestroyIfInGroup(lua_State *L, const void *mem, const char *mt, const void *info)
    {
    ud_t *ud = (ud_t*)mem;
    int groupid = *((int*)info);
    if(IsValid(ud)&&(ud->groupid==groupid))
        ud->destructor(L, ud);
    (void)mt;
    return 0;
    }

static int GroupDestroy(lua_State *L)
/* Destroy all contact joints with the given groupid */
    {
    int groupid = luaL_checkinteger(L, 1);
    udata_scan(L, JOINT_CONTACT_MT, &groupid, DestroyIfInGroup);
    return 1;
    }

static int Create(lua_State *L)
    {
    int t;
    ud_t *world_ud;
    contact_t contact;
    world_t world = checkworld(L, 1, &world_ud);
    int groupid = luaL_checkinteger(L, 2);
    memset(&contact, 0, sizeof(contact_t));
    checkcontactpoint(L, 3, &contact.geom);
    t = lua_type(L, 4);
    switch(t)
        {
        case LUA_TNIL:
        case LUA_TNONE: break;
        case LUA_TSTRING: /* binary string previously packed with pack_surface_parameters() */
                {
                size_t len;
                const char *s = lua_tolstring(L, 4, &len);
                if(len!=sizeof(surface_parameters_t)) return argerror(L, 4, ERR_LENGTH);
                memcpy(&contact.surface, s, sizeof(surface_parameters_t));
                break;
                }
        default:
            checksurfaceparameters(L, 4, &contact.surface);
        }
    if(!lua_isnoneornil(L, 5))
        {
        checkvec3(L, 5, contact.fdir1);
        contact.surface.mode = contact.surface.mode | dContactFDir1;
        }
    joint_t joint = dJointCreateContact(world, 0 /* native ODE groups not used here */, &contact);
    return newjoint(L, joint, world_ud, groupid);
    }

DESTROY_FUNC(joint)

static const struct luaL_Reg Methods[] = 
    {
        { "destroy", Destroy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc", Destroy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "pack_surfaceparameters", PackSurfaceParameters },
        { "create_contact_joint", Create },
        { "destroy_joint_group", GroupDestroy },
        { NULL, NULL } /* sentinel */
    };

void moonode_open_joint_contact(lua_State *L)
    {
    udata_define(L, JOINT_CONTACT_MT, Methods, MetaMethods);
    udata_inherit(L, JOINT_CONTACT_MT, JOINT_MT);
    luaL_setfuncs(L, Functions, 0);
    }

