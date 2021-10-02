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
    if(!freeuserdata(L, ud, "joint_universal")) return 0;
    jointdestroy(L, joint);
    return 0;
    }

static int newjoint(lua_State *L, joint_t joint, ud_t *world_ud)
    {
    ud_t *ud;
    ud = newuserdata(L, joint, JOINT_UNIVERSAL_MT, "joint_universal");
    ud->parent_ud = world_ud;
    ud->destructor = freejoint;
    return 1;
    }

static int Create(lua_State *L)
    {
    ud_t *world_ud;
    world_t world = checkworld(L, 1, &world_ud);
    joint_t joint = dJointCreateUniversal(world, 0);
    return newjoint(L, joint, world_ud);
    }

SETVEC3(SetAnchor, dJointSetUniversalAnchor, universal)
SETVEC3(SetAxis1, dJointSetUniversalAxis1, universal)
SETVEC3(SetAxis2, dJointSetUniversalAxis2, universal)
//SETVEC3NUMNUM(SetAxis1Offset, dJointSetUniversalAxis1Offset, universal)
//SETVEC3NUMNUM(SetAxis2Offset, dJointSetUniversalAxis2Offset, universal)
SETNUMNUM(AddTorques, dJointAddUniversalTorques, universal)
GETVEC3(GetAnchor, dJointGetUniversalAnchor, universal)
GETVEC3(GetAnchor2, dJointGetUniversalAnchor2, universal)
GETVEC3(GetAxis1, dJointGetUniversalAxis1, universal)
GETVEC3(GetAxis2, dJointGetUniversalAxis2, universal)
//GETNUMNUM(GetAngles, dJointGetUniversalAngles, universal)
GETNUM(GetAngle1, dJointGetUniversalAngle1, universal)
GETNUM(GetAngle2, dJointGetUniversalAngle2, universal)
GETNUM(GetAngle1Rate, dJointGetUniversalAngle1Rate, universal)
GETNUM(GetAngle2Rate, dJointGetUniversalAngle2Rate, universal)

SET_PARAM(SetParam, dJointSetUniversalParam, universal)
GET_PARAM(GetParam, dJointGetUniversalParam, universal)
DESTROY_FUNC(joint)

static const struct luaL_Reg Methods[] = 
    {
        { "set_param", SetParam },
        { "get_param", GetParam },
        { "set_anchor1", SetAnchor },
        { "set_axis1", SetAxis1 },
        { "set_axis2", SetAxis2 },
//      { "set_axis1_offset", SetAxis1Offset },
//      { "set_axis2_offset", SetAxis2Offset },
        { "add_torques", AddTorques },
        { "get_anchor", GetAnchor },
        { "get_anchor2", GetAnchor2 },
        { "get_axis1", GetAxis1 },
        { "get_axis2", GetAxis2 },
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
        { "create_universal_joint", Create },
        { NULL, NULL } /* sentinel */
    };

void moonode_open_joint_universal(lua_State *L)
    {
    udata_define(L, JOINT_UNIVERSAL_MT, Methods, MetaMethods);
    udata_inherit(L, JOINT_UNIVERSAL_MT, JOINT_MT);
    luaL_setfuncs(L, Functions, 0);
    }

