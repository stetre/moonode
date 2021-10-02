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
    if(!freeuserdata(L, ud, "joint_hinge2")) return 0;
    jointdestroy(L, joint);
    return 0;
    }

static int newjoint(lua_State *L, joint_t joint, ud_t *world_ud)
    {
    ud_t *ud;
    ud = newuserdata(L, joint, JOINT_HINGE2_MT, "joint_hinge2");
    ud->parent_ud = world_ud;
    ud->destructor = freejoint;
    return 1;
    }

static int Create(lua_State *L)
    {
    ud_t *world_ud;
    world_t world = checkworld(L, 1, &world_ud);
    joint_t joint = dJointCreateHinge2(world, 0);
    return newjoint(L, joint, world_ud);
    }

SETVEC3(SetAnchor, dJointSetHinge2Anchor, hinge2)
GETVEC3(GetAnchor, dJointGetHinge2Anchor, hinge2)
GETVEC3(GetAnchor2, dJointGetHinge2Anchor2, hinge2)
GETVEC3(GetAxis1, dJointGetHinge2Axis1, hinge2)
GETVEC3(GetAxis2, dJointGetHinge2Axis2, hinge2)
GETNUM(GetAngle1, dJointGetHinge2Angle1, hinge2)
GETNUM(GetAngle2, dJointGetHinge2Angle2, hinge2)
GETNUM(GetAngle1Rate, dJointGetHinge2Angle1Rate, hinge2)
GETNUM(GetAngle2Rate, dJointGetHinge2Angle2Rate, hinge2)
SETNUMNUM(AddTorques, dJointAddHinge2Torques, hinge2)

#if 0
static int SetAxes(lua_State *L)
    {
    double *axis1=NULL, *axis2=NULL;
    vec3_t val1, val2;
    joint_t joint = checkjoint_pu(L, 1, NULL);
    if(!lua_isnoneornil(L, 2)) { checkvec3(L, 2, val1); axis1 = val1; }
    if(!lua_isnoneornil(L, 3)) { checkvec3(L, 2, val2); axis2 = val2; }
    dJointSetHinge2Axes (joint, axis1, axis2);
    return 0;
    }
#endif

SET_PARAM(SetParam, dJointSetHinge2Param, hinge2)
GET_PARAM(GetParam, dJointGetHinge2Param, hinge2)
DESTROY_FUNC(joint)

static const struct luaL_Reg Methods[] = 
    {
        { "set_param", SetParam  },
        { "get_param", GetParam  },
        { "set_anchor1", SetAnchor },
        { "get_anchor1", GetAnchor },
        { "get_anchor2", GetAnchor2 },
//      { "set_axes", SetAxes  },
        { "get_axis1", GetAxis1  },
        { "get_axis2", GetAxis2  },
        { "get_angle1", GetAngle1 },
        { "get_angle2", GetAngle2 },
        { "get_angle1_rate", GetAngle1Rate },
        { "get_angle2_rate", GetAngle2Rate },
        { "add_torques", AddTorques },
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
        { "create_hinge2_joint", Create },
        { NULL, NULL } /* sentinel */
    };

void moonode_open_joint_hinge2(lua_State *L)
    {
    udata_define(L, JOINT_HINGE2_MT, Methods, MetaMethods);
    udata_inherit(L, JOINT_HINGE2_MT, JOINT_MT);
    luaL_setfuncs(L, Functions, 0);
    }

