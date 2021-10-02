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
    if(!freeuserdata(L, ud, "joint_pu")) return 0;
    jointdestroy(L, joint);
    return 0;
    }

static int newjoint(lua_State *L, joint_t joint, ud_t *world_ud)
    {
    ud_t *ud;
    ud = newuserdata(L, joint, JOINT_PU_MT, "joint_pu");
    ud->parent_ud = world_ud;
    ud->destructor = freejoint;
    return 1;
    }

static int Create(lua_State *L)
    {
    ud_t *world_ud;
    world_t world = checkworld(L, 1, &world_ud);
    joint_t joint = dJointCreatePU(world, 0);
    return newjoint(L, joint, world_ud);
    }

SETVEC3(SetAnchor, dJointSetPUAnchor, pu)
//SETVEC3VEC3(SetAnchorOffset, dJointSetPUAnchorOffset, pu)
SETVEC3(SetAxis1, dJointSetPUAxis1, pu)
SETVEC3(SetAxis2, dJointSetPUAxis2, pu)
SETVEC3(SetAxis3, dJointSetPUAxis3, pu) // same as dJointSetPUAxisP
//SETNUM(AddTorque, dJointAddPUTorque, pu)  UNDEFINED SYMBOL
GETVEC3(GetAnchor, dJointGetPUAnchor, pu)
GETNUM(GetPosition, dJointGetPUPosition, pu)
GETNUM(GetPositionRate, dJointGetPUPositionRate, pu)
GETVEC3(GetAxis1, dJointGetPUAxis1, pu)
GETVEC3(GetAxis2, dJointGetPUAxis2, pu)
GETVEC3(GetAxis3, dJointGetPUAxis3, pu) // same as dJointGetPUAxisP
//GETVEC3VEC3(GetAngles, dJointGetPUAngles, pu)
GETNUM(GetAngle1, dJointGetPUAngle1, pu)
GETNUM(GetAngle2, dJointGetPUAngle2, pu)
GETNUM(GetAngle1Rate, dJointGetPUAngle1Rate, pu)
GETNUM(GetAngle2Rate, dJointGetPUAngle2Rate, pu)

SET_PARAM(SetParam, dJointSetPUParam, pu)
GET_PARAM(GetParam, dJointGetPUParam, pu)
DESTROY_FUNC(joint)

static const struct luaL_Reg Methods[] = 
    {
        { "set_param", SetParam },
        { "get_param", GetParam },
        { "set_anchor", SetAnchor },
//      { "set_anchor_offset", SetAnchorOffset },
        { "set_axis1", SetAxis1 },
        { "set_axis2", SetAxis2 },
        { "set_axis3", SetAxis3 },
//      { "add_torque", AddTorque },
        { "get_anchor", GetAnchor },
        { "get_position", GetPosition },
        { "get_position_rate", GetPositionRate },
        { "get_axis1", GetAxis1 },
        { "get_axis2", GetAxis2 },
        { "get_axis3", GetAxis3 },
//      { "get_angles", GetAngles },
        { "get_angle1", GetAngle1 },
        { "get_angle2", GetAngle2 },
        { "get_angle1_rate", GetAngle1Rate },
        { "get_angle2_rate", GetAngle2Rate },
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
        { "create_pu_joint", Create },
        { NULL, NULL } /* sentinel */
    };

void moonode_open_joint_pu(lua_State *L)
    {
    udata_define(L, JOINT_PU_MT, Methods, MetaMethods);
    udata_inherit(L, JOINT_PU_MT, JOINT_MT);
    luaL_setfuncs(L, Functions, 0);
    }

