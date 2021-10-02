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
    if(!freeuserdata(L, ud, "joint_transmission")) return 0;
    jointdestroy(L, joint);
    return 0;
    }

static int newjoint(lua_State *L, joint_t joint, ud_t *world_ud)
    {
    ud_t *ud;
    ud = newuserdata(L, joint, JOINT_TRANSMISSION_MT, "joint_transmission");
    ud->parent_ud = world_ud;
    ud->destructor = freejoint;
    return 1;
    }

static int Create(lua_State *L)
    {
    ud_t *world_ud;
    world_t world = checkworld(L, 1, &world_ud);
    joint_t joint = dJointCreateTransmission(world, 0);
    return newjoint(L, joint, world_ud);
    }

static int SetTransmissionMode(lua_State *L)
    {
    joint_t joint = checkjoint_transmission(L, 1, NULL);
    int mode = checktransmissionmode(L, 2);
    dJointSetTransmissionMode(joint, mode);
    return 0;
    }

static int GetTransmissionMode(lua_State *L)
    {
    joint_t joint = checkjoint_transmission(L, 1, NULL);
    int mode = dJointGetTransmissionMode(joint);
    pushtransmissionmode(L, mode);
    return 1;
    }


SETVEC3(SetAxis, dJointSetTransmissionAxis, transmission)
SETVEC3(SetAxis1, dJointSetTransmissionAxis1, transmission)
SETVEC3(SetAxis2, dJointSetTransmissionAxis2, transmission)
SETVEC3(SetAnchor1, dJointSetTransmissionAnchor1, transmission)
SETVEC3(SetAnchor2, dJointSetTransmissionAnchor2, transmission)
GETVEC3(GetAxis, dJointGetTransmissionAxis, transmission)
GETVEC3(GetAxis1, dJointGetTransmissionAxis1, transmission)
GETVEC3(GetAxis2, dJointGetTransmissionAxis2, transmission)
GETVEC3(GetAnchor1, dJointGetTransmissionAnchor1, transmission)
GETVEC3(GetAnchor2, dJointGetTransmissionAnchor2, transmission)
GETVEC3(GetContactPoint1, dJointGetTransmissionContactPoint1, transmission)
GETVEC3(GetContactPoint2, dJointGetTransmissionContactPoint2, transmission)

SETNUM(SetRatio, dJointSetTransmissionRatio, transmission)
SETNUM(SetRadius1, dJointSetTransmissionRadius1, transmission)
SETNUM(SetRadius2, dJointSetTransmissionRadius2, transmission)
SETNUM(SetBacklash, dJointSetTransmissionBacklash, transmission)
GETNUM(GetRatio, dJointGetTransmissionRatio, transmission)
GETNUM(GetAngle1, dJointGetTransmissionAngle1, transmission)
GETNUM(GetAngle2, dJointGetTransmissionAngle2, transmission)
GETNUM(GetRadius1, dJointGetTransmissionRadius1, transmission)
GETNUM(GetRadius2, dJointGetTransmissionRadius2, transmission)
GETNUM(GetBacklash, dJointGetTransmissionBacklash, transmission)

SET_PARAM(SetParam, dJointSetTransmissionParam, transmission)
GET_PARAM(GetParam, dJointGetTransmissionParam, transmission)
DESTROY_FUNC(joint)

static const struct luaL_Reg Methods[] = 
    {
        { "set_param", SetParam },
        { "get_param", GetParam },
        { "set_axis1", SetAxis1 },
        { "set_axis2", SetAxis2 },
        { "set_anchor1", SetAnchor1 },
        { "set_anchor2", SetAnchor2 },
        { "get_axis1", GetAxis1 },
        { "get_axis2", GetAxis2 },
        { "get_anchor1", GetAnchor1 },
        { "get_anchor2", GetAnchor2 },
        { "get_contact_point1", GetContactPoint1 },
        { "get_contact_point2", GetContactPoint2 },
        { "set_mode", SetTransmissionMode },
        { "get_mode", GetTransmissionMode },
        { "set_ratio", SetRatio },
        { "get_ratio", GetRatio },
        { "set_axis", SetAxis },
        { "get_axis", GetAxis },
        { "get_angle1", GetAngle1 },
        { "get_angle2", GetAngle2 },
        { "get_radius1", GetRadius1 },
        { "get_radius2", GetRadius2 },
        { "set_radius1", SetRadius1 },
        { "set_radius2", SetRadius2 },
        { "get_backlash", GetBacklash },
        { "set_backlash", SetBacklash },
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
        { "create_transmission_joint", Create },
        { NULL, NULL } /* sentinel */
    };

void moonode_open_joint_transmission(lua_State *L)
    {
    udata_define(L, JOINT_TRANSMISSION_MT, Methods, MetaMethods);
    udata_inherit(L, JOINT_TRANSMISSION_MT, JOINT_MT);
    luaL_setfuncs(L, Functions, 0);
    }

