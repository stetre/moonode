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

int jointdestroy(lua_State *L, joint_t joint)
/* Wrapper for dJointDestroy() */
    {
    jointfeedback_t *fb;
    /* disable and free the feedback struct, if any */
    fb = dJointGetFeedback(joint);
    if(fb) 
        {
        dJointSetFeedback(joint, 0);
        Free(L, fb);
        }
    dJointDestroy(joint);
    return 0;
    }
 
static int Enable(lua_State *L)
    {
    joint_t joint = checkjoint(L, 1, NULL);
    dJointEnable(joint);
    return 0;
    }

static int Disable(lua_State *L)
    {
    joint_t joint = checkjoint(L, 1, NULL);
    dJointDisable(joint);
    return 0;
    }

static int IsEnabled(lua_State *L)
    {
    joint_t joint = checkjoint(L, 1, NULL);
    lua_pushboolean(L, dJointIsEnabled(joint));
    return 1;
    }

static int GetType(lua_State *L)
    {
    joint_t joint = checkjoint(L, 1, NULL);
    pushjointtype(L, dJointGetType(joint));
    return 1;
    }

static int GetBody1(lua_State *L)
    {
    joint_t joint = checkjoint(L, 1, NULL);
    body_t body = dJointGetBody(joint, 0);
    if(body) pushbody(L, body); else lua_pushnil(L);
    return 1;
    }

static int GetBody2(lua_State *L)
    {
    joint_t joint = checkjoint(L, 1, NULL);
    body_t body = dJointGetBody(joint, 1);
    if(body) pushbody(L, body); else lua_pushnil(L);
    return 1;
    }

static int GetBodies(lua_State *L)
    {
    joint_t joint = checkjoint(L, 1, NULL);
    body_t body1 = dJointGetBody(joint, 0);
    body_t body2 = dJointGetBody(joint, 1);
    if(body1) pushbody(L, body1); else lua_pushnil(L);
    if(body2) pushbody(L, body2); else lua_pushnil(L);
    return 2;
    }

static int GetNumBodies(lua_State *L)
    {
    joint_t joint = checkjoint(L, 1, NULL);
    lua_pushinteger(L, dJointGetNumBodies(joint));
    return 1;
    }

static int Attach(lua_State *L)
    {
    joint_t joint = checkjoint(L, 1, NULL);
    body_t body1 = optbody(L, 2, NULL);
    body_t body2 = optbody(L, 3, NULL);
    dJointAttach(joint, body1, body2);
    return 0;
    }

static int ConnectingJoint(lua_State *L)
    {
    body_t body1 = checkbody(L, 1, NULL);
    body_t body2 = checkbody(L, 2, NULL);
    joint_t joint = dConnectingJoint(body1, body2);
    if(!joint) lua_pushnil(L);
    else pushjoint(L, joint);
    return 1;
    }

static int ConnectingJointList(lua_State *L)
    {
    joint_t *joint;
    int i, n, n1, n2, sz;
    body_t body1 = checkbody(L, 1, NULL);
    body_t body2 = checkbody(L, 2, NULL);
    /* We are supposed to pass an array to be filled with the n joints
     * that connect the two bodies, but without knowing in advance how
     * many of them there are. 
     * However, if body1 and body2 are connected respectively to n1 and
     * n2 joints, we know for sure that n <= min(n1, n2).
     */
    n1 = dBodyGetNumJoints(body1);
    n2 = dBodyGetNumJoints(body2);
    sz = (n1 > n2) ? n2 : n1;
    lua_newtable(L);
    if(sz==0) return 1; /* both bodies have no joints */
    joint = Malloc(L, sz*sizeof(joint_t));
    n = dConnectingJointList(body1, body2, joint);
    for(i=0; i<n; i++)
        {
        pushjoint(L, joint[i]);
        lua_rawseti(L, -2, i+1);
        }
    Free(L, joint);
    return 1;
    }

static int AreConnected(lua_State *L)
    {
    body_t body1 = checkbody(L, 1, NULL);
    body_t body2 = checkbody(L, 2, NULL);
    lua_pushboolean(L, dAreConnected(body1, body2));
    return 1;
    }

static int AreConnectedExcluding(lua_State *L)
    {
    body_t body1 = checkbody(L, 1, NULL);
    body_t body2 = checkbody(L, 2, NULL);
    int joint_type = checkjointtype(L, 3);
    lua_pushboolean(L, dAreConnectedExcluding(body1, body2, joint_type));
    return 1;
    }

static int SetFeedback(lua_State *L)
    {
    joint_t joint = checkjoint(L, 1, NULL);
    int on = checkboolean(L, 2);
    jointfeedback_t *fb = dJointGetFeedback(joint);
    if(on)
        {
        if(fb) return 0; // already on
        // allocate the struct and enable
        fb = Malloc(L, sizeof(jointfeedback_t ));
        dJointSetFeedback(joint, fb);
        }
    else
        {
        if(!fb) return 0; // already off
        // disable and free the struct
        dJointSetFeedback(joint, 0);
        Free(L, fb);
        }
    return 0;
    }

static int GetFeedback(lua_State *L)
    {
    joint_t joint = checkjoint(L, 1, NULL);
    jointfeedback_t *fb = dJointGetFeedback(joint);
    if(!fb) return 0; // or error?
    pushvec3(L, fb->f1);
    pushvec3(L, fb->t1);
    pushvec3(L, fb->f2);
    pushvec3(L, fb->t2);
    return 4;
    }

RAW_FUNC(joint)
DESTROY_FUNC(joint)

static const struct luaL_Reg Methods[] = 
    {
        { "raw", Raw },
        { "enable", Enable },
        { "disable", Disable },
        { "is_enabled", IsEnabled  },
        { "get_type", GetType },
        { "get_body1", GetBody1 },
        { "get_body2", GetBody2 },
        { "get_bodies", GetBodies },
        { "get_num_bodies", GetNumBodies },
        { "attach", Attach },
        { "set_feedback", SetFeedback },
        { "get_feedback", GetFeedback },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc", Destroy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "connecting_joint", ConnectingJoint },
        { "connecting_joint_list", ConnectingJointList },
        { "are_connected", AreConnected },
        { "are_connected_excluding", AreConnectedExcluding },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_joint(lua_State *L)
    {
    udata_define(L, JOINT_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

#if 0
// Not used:
//void dJointSetData(joint_t, void *data);
//void *dJointGetData(joint_t);
// Note: We don't use native ODE jointgroups here. Since the purpose of a jointgroup
// is mainly to efficiently destroy a group of temporary contacts, we implement it for 
// contacts joints only, on top of the ODE API (see joint_contact.c).
//dJointGroupID dJointGroupCreate(int max_size);
//void dJointGroupDestroy(dJointGroupID);
//void dJointGroupEmpty(dJointGroupID);
#endif

