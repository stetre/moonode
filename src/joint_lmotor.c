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
    if(!freeuserdata(L, ud, "joint_lmotor")) return 0;
    jointdestroy(L, joint);
    return 0;
    }

static int newjoint(lua_State *L, joint_t joint, ud_t *world_ud)
    {
    ud_t *ud;
    ud = newuserdata(L, joint, JOINT_LMOTOR_MT, "joint_lmotor");
    ud->parent_ud = world_ud;
    ud->destructor = freejoint;
    return 1;
    }

static int Create(lua_State *L)
    {
    ud_t *world_ud;
    world_t world = checkworld(L, 1, &world_ud);
    joint_t joint = dJointCreateLMotor(world, 0);
    return newjoint(L, joint, world_ud);
    }

#define F(Func, anum)                                   \
static int Func(lua_State *L)                           \
    {                                                   \
    vec3_t axis;                                        \
    joint_t joint = checkjoint_lmotor(L, 1, NULL);      \
    int rel = checkrelativeorientation(L, 3);           \
    checkvec3(L, 2, axis);                              \
    dJointSetLMotorAxis(joint, anum, rel, axis[0], axis[1], axis[2]);   \
    return 0;                                           \
    }
F(SetAxis0, 0)
F(SetAxis1, 1)
F(SetAxis2, 2)
#undef F


#define F(Func, anum)                               \
static int Func(lua_State *L)                       \
    {                                               \
    vec3_t axis;                                    \
    joint_t joint = checkjoint_lmotor(L, 1, NULL);  \
    dJointGetLMotorAxis(joint, anum, axis);         \
    pushvec3(L, axis);                              \
    return 1;                                       \
    }
F(GetAxis0, 0)
F(GetAxis1, 1)
F(GetAxis2, 2)
#undef F


SETINT(SetNumAxes, dJointSetLMotorNumAxes, lmotor)
GETINT(GetNumAxes, dJointGetLMotorNumAxes, lmotor)

SET_PARAM(SetParam, dJointSetLMotorParam, lmotor)
GET_PARAM(GetParam, dJointGetLMotorParam, lmotor)
DESTROY_FUNC(joint)

static const struct luaL_Reg Methods[] = 
    {
        { "set_param", SetParam },
        { "get_param", GetParam },
        { "set_num_axes", SetNumAxes },
        { "set_axis1", SetAxis0 },
        { "set_axis2", SetAxis1 },
        { "set_axis3", SetAxis2 },
        { "get_num_axes", GetNumAxes },
        { "get_axis1", GetAxis0 },
        { "get_axis2", GetAxis1 },
        { "get_axis3", GetAxis2 },
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
        { "create_lmotor_joint", Create },
        { NULL, NULL } /* sentinel */
    };

void moonode_open_joint_lmotor(lua_State *L)
    {
    udata_define(L, JOINT_LMOTOR_MT, Methods, MetaMethods);
    udata_inherit(L, JOINT_LMOTOR_MT, JOINT_MT);
    luaL_setfuncs(L, Functions, 0);
    }

