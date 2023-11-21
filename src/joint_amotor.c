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
    if(!freeuserdata(L, ud, "joint_amotor")) return 0;
    jointdestroy(L, joint);
    return 0;
    }

static int newjoint(lua_State *L, joint_t joint, ud_t *world_ud)
    {
    ud_t *ud;
    ud = newuserdata(L, joint, JOINT_AMOTOR_MT, "joint_amotor");
    ud->parent_ud = world_ud;
    ud->destructor = freejoint;
    return 1;
    }

static int Create(lua_State *L)
    {
    ud_t *world_ud;
    world_t world = checkworld(L, 1, &world_ud);
    joint_t joint = dJointCreateAMotor(world, 0);
    return newjoint(L, joint, world_ud);
    }

#define F(Func, anum)                                   \
static int Func(lua_State *L)                           \
    {                                                   \
    vec3_t axis;                                        \
    joint_t joint = checkjoint_amotor(L, 1, NULL);      \
    int rel = checkrelativeorientation(L, 3);           \
    checkvec3(L, 2, axis);                              \
    dJointSetAMotorAxis(joint, anum, rel, axis[0], axis[1], axis[2]);   \
    return 0;                                           \
    }
F(SetAxis0, 0)
F(SetAxis1, 1)
F(SetAxis2, 2)
#undef F

#define F(Func, anum)                                   \
static int Func(lua_State *L)                           \
    {                                                   \
    joint_t joint = checkjoint_amotor(L, 1, NULL);      \
    double angle = luaL_checknumber(L, 2);              \
    dJointSetAMotorAngle(joint, anum, angle);           \
    return 0;                                           \
    }
F(SetAngle0, 0)
F(SetAngle1, 1)
F(SetAngle2, 2)
#undef F

static int SetMode(lua_State *L)
    {
    joint_t joint = checkjoint_amotor(L, 1, NULL);
    int mode = checkamotormode(L, 2);
    dJointSetAMotorMode(joint, mode);
    return 0;
    }

static int AddTorques(lua_State *L)
    {
    joint_t joint = checkjoint_amotor(L, 1, NULL);
    double torque0 = luaL_optnumber(L, 2, 0);
    double torque1 = luaL_optnumber(L, 3, 0);
    double torque2 = luaL_optnumber(L, 4, 0);
    dJointAddAMotorTorques(joint, torque0, torque1, torque2);
    return 0;
    }

#define F(Func, anum)                               \
static int Func(lua_State *L)                       \
    {                                               \
    vec3_t axis;                                    \
    int rel;                                        \
    joint_t joint = checkjoint_amotor(L, 1, NULL);  \
    dJointGetAMotorAxis(joint, anum, axis);         \
    rel = dJointGetAMotorAxisRel(joint, anum);      \
    pushvec3(L, axis);                              \
    pushrelativeorientation(L, rel);                \
    return 2;                                       \
    }
F(GetAxis0, 0)
F(GetAxis1, 1)
F(GetAxis2, 2)
#undef F

SETINT(SetNumAxes, dJointSetAMotorNumAxes, amotor)
GETINT(GetNumAxes, dJointGetAMotorNumAxes, amotor)

#define F(Func, anum)                                   \
static int Func(lua_State *L)                           \
    {                                                   \
    joint_t joint = checkjoint_amotor(L, 1, NULL);      \
    double result = dJointGetAMotorAngle(joint, anum);  \
    lua_pushnumber(L, result);                          \
    return 1;                                           \
    }
F(GetAngle0, 0)
F(GetAngle1, 1)
F(GetAngle2, 2)
#undef F

#define F(Func, anum)                                   \
static int Func(lua_State *L)                           \
    {                                                   \
    joint_t joint = checkjoint_amotor(L, 1, NULL);      \
    double result = dJointGetAMotorAngleRate(joint, anum);  \
    lua_pushnumber(L, result);                          \
    return 1;                                           \
    }
F(GetAngleRate0, 0)
F(GetAngleRate1, 1)
F(GetAngleRate2, 2)
#undef F

static int GetMode(lua_State *L)
    {
    joint_t joint = checkjoint_amotor(L, 1, NULL);
    int mode = dJointGetAMotorMode(joint);
    pushamotormode(L, mode);
    return 1;
    }

SET_PARAM(SetParam, dJointSetAMotorParam, amotor)
GET_PARAM(GetParam, dJointGetAMotorParam, amotor)
DESTROY_FUNC(joint)

static const struct luaL_Reg Methods[] = 
    {
        { "set_param", SetParam  },
        { "get_param", GetParam  },
        { "set_num_axes", SetNumAxes },
        { "set_axis1", SetAxis0 },
        { "set_axis2", SetAxis1 },
        { "set_axis3", SetAxis2 },
        { "set_angle1", SetAngle0 },
        { "set_angle2", SetAngle1 },
        { "set_angle3", SetAngle2 },
        { "set_mode", SetMode },
        { "add_torques", AddTorques },
        { "get_num_axes", GetNumAxes },
        { "get_axis1", GetAxis0 },
        { "get_axis2", GetAxis1 },
        { "get_axis3", GetAxis2 },
        { "get_angle1", GetAngle0 },
        { "get_angle2", GetAngle1 },
        { "get_angle3", GetAngle2 },
        { "get_angle_rate1", GetAngleRate0 },
        { "get_angle_rate2", GetAngleRate1 },
        { "get_angle_rate3", GetAngleRate2 },
        { "get_mode", GetMode },
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
        { "create_amotor_joint", Create },
        { NULL, NULL } /* sentinel */
    };

void moonode_open_joint_amotor(lua_State *L)
    {
    udata_define(L, JOINT_AMOTOR_MT, Methods, MetaMethods);
    udata_inherit(L, JOINT_AMOTOR_MT, JOINT_MT);
    luaL_setfuncs(L, Functions, 0);
    }

