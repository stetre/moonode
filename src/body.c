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

static int freebody(lua_State *L, ud_t *ud)
    {
    body_t body = (body_t)ud->handle;
//  freechildren(L, _MT, ud);
    if(!freeuserdata(L, ud, "body")) return 0;
    dBodyDestroy(body);
    return 0;
    }

static int newbody(lua_State *L, body_t body, ud_t *world_ud)
    {
    ud_t *ud;
    ud = newuserdata(L, body, BODY_MT, "body");
    ud->parent_ud = world_ud;
    ud->destructor = freebody;
    return 1;
    }

static int Create(lua_State *L)
    {
    ud_t *world_ud;
    world_t world = checkworld(L, 1, &world_ud);
    body_t body = dBodyCreate(world);
    return newbody(L, body, world_ud);
    }


#define F(Func, func)   /* void func(body_t) */             \
static int Func(lua_State *L)                               \
    {                                                       \
    body_t body = checkbody(L, 1, NULL);                    \
    func(body);                                             \
    return 0;                                               \
    }
F(SetDynamic, dBodySetDynamic)
F(SetKinematic, dBodySetKinematic)
F(Enable, dBodyEnable)
F(Disable, dBodyDisable)
F(SetDampingDefaults, dBodySetDampingDefaults)
F(SetAutoDisableDefaults, dBodySetAutoDisableDefaults)
#undef F

#define F(Func, func)   /* boolean func(body) */            \
static int Func(lua_State *L)                               \
    {                                                       \
    body_t body = checkbody(L, 1, NULL);                    \
    lua_pushboolean(L, func(body));                         \
    return 1;                                               \
    }
F(IsKinematic, dBodyIsKinematic)
F(IsEnabled, dBodyIsEnabled)
F(GetAutoDisableFlag, dBodyGetAutoDisableFlag)
F(GetFiniteRotationMode, dBodyGetFiniteRotationMode)
F(GetGravityMode, dBodyGetGravityMode)
F(GetGyroscopicMode, dBodyGetGyroscopicMode)
#undef F


#define F(Func, func)   /* void func(body, boolean) */      \
static int Func(lua_State *L)                               \
    {                                                       \
    body_t body = checkbody(L, 1, NULL);                    \
    int val = checkboolean(L, 2);                           \
    func(body, val);                                        \
    return 0;                                               \
    }
F(SetAutoDisableFlag, dBodySetAutoDisableFlag)
F(SetFiniteRotationMode, dBodySetFiniteRotationMode)
F(SetGravityMode, dBodySetGravityMode)
F(SetGyroscopicMode, dBodySetGyroscopicMode)
#undef F


#define F(Func, func)   /* void func(body, int) */          \
static int Func(lua_State *L)                               \
    {                                                       \
    body_t body = checkbody(L, 1, NULL);                    \
    int val = luaL_checkinteger(L, 2);                      \
    func(body, val);                                        \
    return 0;                                               \
    }
F(SetAutoDisableAverageSamplesCount, dBodySetAutoDisableAverageSamplesCount)
F(SetAutoDisableSteps, dBodySetAutoDisableSteps)
#undef F


#define F(Func, func)   /* int func(body) */                \
static int Func(lua_State *L)                               \
    {                                                       \
    body_t body = checkbody(L, 1, NULL);                    \
    lua_pushinteger(L, func(body));                         \
    return 1;                                               \
    }
F(GetAutoDisableAverageSamplesCount, dBodyGetAutoDisableAverageSamplesCount)
F(GetAutoDisableSteps, dBodyGetAutoDisableSteps)
F(GetNumJoints, dBodyGetNumJoints)
#undef F


#define F(Func, func)   /* void func(body, double) */   \
static int Func(lua_State *L)                               \
    {                                                       \
    body_t body = checkbody(L, 1, NULL);                    \
    double val = luaL_checknumber(L, 2);                    \
    func(body, val);                                        \
    return 0;                                               \
    }
F(SetAutoDisableLinearThreshold, dBodySetAutoDisableLinearThreshold)
F(SetAutoDisableAngularThreshold, dBodySetAutoDisableAngularThreshold)
F(SetAutoDisableTime, dBodySetAutoDisableTime)
F(SetLinearDamping, dBodySetLinearDamping)
F(SetAngularDamping, dBodySetAngularDamping)
F(SetLinearDampingThreshold, dBodySetLinearDampingThreshold)
F(SetAngularDampingThreshold, dBodySetAngularDampingThreshold)
F(SetMaxAngularSpeed, dBodySetMaxAngularSpeed)
#undef F

static int SetDamping(lua_State *L)
    {
    body_t body = checkbody(L, 1, NULL);
    double val1 = luaL_checknumber(L, 2);
    double val2 = luaL_checknumber(L, 3);
    dBodySetDamping(body, val1, val2);
    return 0;
    }

static int GetDamping(lua_State *L)
    {
    body_t body = checkbody(L, 1, NULL);
    double val1 = dBodyGetLinearDamping(body);
    double val2 = dBodyGetAngularDamping(body);
    lua_pushnumber(L, val1);
    lua_pushnumber(L, val2);
    return 2;
    }

#define F(Func, func)   /* double func(body) */             \
static int Func(lua_State *L)                               \
    {                                                       \
    body_t body = checkbody(L, 1, NULL);                    \
    lua_pushnumber(L, func(body));                          \
    return 1;                                               \
    }
F(GetAutoDisableLinearThreshold, dBodyGetAutoDisableLinearThreshold)
F(GetAutoDisableAngularThreshold, dBodyGetAutoDisableAngularThreshold)
F(GetAutoDisableTime, dBodyGetAutoDisableTime)
F(GetLinearDamping, dBodyGetLinearDamping)
F(GetAngularDamping, dBodyGetAngularDamping)
F(GetLinearDampingThreshold, dBodyGetLinearDampingThreshold)
F(GetAngularDampingThreshold, dBodyGetAngularDampingThreshold)
F(GetMaxAngularSpeed, dBodyGetMaxAngularSpeed)
#undef F


#define F(Func, func)   /* void func(body, vec3) */         \
static int Func(lua_State *L)                               \
    {                                                       \
    vec3_t val;                                             \
    body_t body = checkbody(L, 1, NULL);                    \
    checkvec3(L, 2, val);                                   \
    func(body, val[0], val[1], val[2]);                     \
    return 0;                                               \
    }
F(SetPosition, dBodySetPosition)
F(SetLinearVel , dBodySetLinearVel)
F(SetAngularVel, dBodySetAngularVel)
F(SetForce, dBodySetForce)
F(SetTorque, dBodySetTorque)
F(AddForce, dBodyAddForce)
F(AddTorque, dBodyAddTorque)
F(AddRelForce, dBodyAddRelForce)
F(AddRelTorque, dBodyAddRelTorque)
F(SetFiniteRotationAxis, dBodySetFiniteRotationAxis)
#undef F


#define F(Func, func)   /* vec3 func(body) */               \
static int Func(lua_State *L)                               \
    {                                                       \
    vec3_t res;                                             \
    body_t body = checkbody(L, 1, NULL);                    \
    func(body, res);                                        \
    pushvec3(L, res);                                       \
    return 1;                                               \
    }
F(GetPosition, dBodyCopyPosition) //dBodyGetPosition not used
F(GetFiniteRotationAxis, dBodyGetFiniteRotationAxis)
#undef F

static int SetRotation(lua_State *L)
    {
    mat3_t val;
    body_t body = checkbody(L, 1, NULL);
    checkmat3(L, 2, val);
    dBodySetRotation(body, val);
    return 0;
    }

static int GetRotation(lua_State *L)
    {
    mat3_t res;
    body_t body = checkbody(L, 1, NULL);
    dBodyCopyRotation(body, res); // dBodyGetRotation not used
    pushmat3(L, res);
    return 1;
    }

static int SetQuaternion(lua_State *L)
    {
    quat_t val;
    body_t body = checkbody(L, 1, NULL);
    checkquat(L, 2, val);
    dBodySetQuaternion(body, val);
    return 0;
    }

static int GetQuaternion(lua_State *L)
    {
    quat_t res;
    body_t body = checkbody(L, 1, NULL);
    dBodyCopyQuaternion(body, res); // dBodyGetQuaternion not used
    pushquat(L, res);
    return 1;
    }


#define F(Func, func)   /* vec3* func(body) */              \
static int Func(lua_State *L)                               \
    {                                                       \
    vec3_t res;                                             \
    body_t body = checkbody(L, 1, NULL);                    \
    const double *val = func(body);                         \
    res[0]=val[0]; res[1]=val[1]; res[2]=val[2];            \
    pushvec3(L, res);                                       \
    return 1;                                               \
    }
F(GetLinearVel, dBodyGetLinearVel)
F(GetAngularVel, dBodyGetAngularVel)
F(GetForce, dBodyGetForce)
F(GetTorque, dBodyGetTorque)
#undef F


#define F(Func, func)   /* void func(body, vec3, vec3) */   \
static int Func(lua_State *L)                               \
    {                                                       \
    vec3_t val1, val2;                                      \
    body_t body = checkbody(L, 1, NULL);                    \
    checkvec3(L, 2, val1);                                  \
    checkvec3(L, 3, val2);                                  \
    func(body, val1[0], val1[1], val1[2], val2[0], val2[1], val2[2]);\
    return 0;                                               \
    }
F(AddForceAtPos, dBodyAddForceAtPos)
F(AddForceAtRelPos, dBodyAddForceAtRelPos)
F(AddRelForceAtPos, dBodyAddRelForceAtPos)
F(AddRelForceAtRelPos, dBodyAddRelForceAtRelPos)
#undef F


#define F(Func, func)   /* vec3 func(body, vec3) */         \
static int Func(lua_State *L)                               \
    {                                                       \
    vec3_t val, res;                                        \
    body_t body = checkbody(L, 1, NULL);                    \
    checkvec3(L, 2, val);                                   \
    func(body, val[0], val[1], val[2], res);                \
    pushvec3(L, res);                                       \
    return 1;                                               \
    }
F(GetRelPointPos, dBodyGetRelPointPos)
F(GetRelPointVel, dBodyGetRelPointVel)
F(GetPointVel, dBodyGetPointVel)
F(GetPosRelPoint, dBodyGetPosRelPoint)
F(VectorToWorld, dBodyVectorToWorld)
F(VectorFromWorld, dBodyVectorFromWorld)
#undef F

#if 0 // use get_joints instead
static int GetJoint(lua_State *L)
    {
    body_t body = checkbody(L, 1, NULL);
    int index = checkindex(L, 2);
    joint_t joint = dBodyGetJoint(body, index);
    if(!joint) lua_pushnil(L);
    else pushjoint(L, joint);
    return 1;
    }
#endif

static int GetJoints(lua_State *L)
// returns all joints at once, in a table
    {
    int i, j;
    joint_t joint;
    ud_t *ud;
    body_t body = checkbody(L, 1, NULL);
    int n = dBodyGetNumJoints(body);
    lua_newtable(L);
    j=1;
    for(i=0; i<n; i++)
        {
        joint = dBodyGetJoint(body , i);
        ud = userdata(joint);
        if(ud) 
            {
            pushuserdata(L, ud);
            lua_rawseti(L, -2, j++);
            }
        }
    return 1;
    }

static int GetWorld(lua_State *L)
    {
    body_t body = checkbody(L, 1, NULL);
    world_t world = dBodyGetWorld(body);
    pushworld(L, world);
    return 1;
    }

static int SetMass(lua_State *L)
    {
    mass_t mass;
    body_t body = checkbody(L, 1, NULL);
    checkmass(L, 2, &mass);
    dBodySetMass(body, &mass);
    return 0;
    }

static int GetMass(lua_State *L)
    {
    mass_t mass;
    body_t body = checkbody(L, 1, NULL);
    dBodyGetMass(body, &mass);
    pushmass(L, &mass);
    return 1;
    }

static int GetGeoms(lua_State *L)
    {
    int i=0;
    geom_t geom;
    body_t body = checkbody(L, 1, NULL);
    lua_newtable(L);
    geom = dBodyGetFirstGeom(body);
    while(geom)
        {
        pushgeom(L, geom);
        lua_rawseti(L, -2, ++i);
        geom = dBodyGetNextGeom(geom);
        }
    return 1;
    }

static void MovedCallback(body_t body)
    {
#define L moonode_L
    ud_t *ud = userdata(body);
    if(!ud)
        { unexpected(L); return; } 
    lua_rawgeti(L, LUA_REGISTRYINDEX, ud->ref1);
    pushbody(L, body);
    if(lua_pcall(L, 1, 0, 0) != LUA_OK)
        { lua_error(L); return; }
#undef L
    }

static int SetMovedCallback(lua_State *L)
    {
    ud_t *ud;
    body_t body = checkbody(L, 1, &ud);
    if(lua_isnoneornil(L, 2)) /* remove callback */
        {
        if(ud->ref1!=LUA_NOREF)
            {
            dBodySetMovedCallback(body, NULL);
            Unreference(L, ud->ref1);
            }
        return 0;
        }
    if(!lua_isfunction(L, 2))
        return argerror(L, 2, ERR_FUNCTION);
    Reference(L, 2, ud->ref1);
    dBodySetMovedCallback(body, MovedCallback);
    return 0;
    }

RAW_FUNC(body)
DESTROY_FUNC(body)

static const struct luaL_Reg Methods[] = 
    {
        { "raw", Raw },
        { "destroy", Destroy },
        { "set_auto_disable_linear_threshold", SetAutoDisableLinearThreshold },
        { "set_auto_disable_angular_threshold", SetAutoDisableAngularThreshold },
        { "set_auto_disable_time", SetAutoDisableTime },
        { "set_linear_damping", SetLinearDamping },
        { "set_angular_damping", SetAngularDamping },
        { "set_linear_damping_threshold", SetLinearDampingThreshold },
        { "set_angular_damping_threshold", SetAngularDampingThreshold },
        { "set_max_angular_speed", SetMaxAngularSpeed },
        { "set_damping", SetDamping },
        { "get_damping", GetDamping },
        { "get_auto_disable_linear_threshold", GetAutoDisableLinearThreshold },
        { "get_auto_disable_angular_threshold", GetAutoDisableAngularThreshold },
        { "get_auto_disable_time", GetAutoDisableTime },
        { "get_linear_damping", GetLinearDamping },
        { "get_angular_damping", GetAngularDamping },
        { "get_linear_damping_threshold", GetLinearDampingThreshold },
        { "get_angular_damping_threshold", GetAngularDampingThreshold },
        { "get_max_angular_speed", GetMaxAngularSpeed },
        { "set_dynamic", SetDynamic },
        { "set_kinematic", SetKinematic },
        { "enable", Enable },
        { "disable", Disable },
        { "set_damping_defaults", SetDampingDefaults },
        { "set_auto_disable_defaults", SetAutoDisableDefaults },
        { "is_kinematic", IsKinematic },
        { "is_enabled", IsEnabled },
        { "get_auto_disable_flag", GetAutoDisableFlag },
        { "get_finite_rotation_mode", GetFiniteRotationMode },
        { "get_gravity_mode", GetGravityMode },
        { "get_gyroscopic_mode", GetGyroscopicMode },
        { "set_auto_disable_flag", SetAutoDisableFlag },
        { "set_finite_rotation_mode", SetFiniteRotationMode },
        { "set_gravity_mode", SetGravityMode },
        { "set_gyroscopic_mode", SetGyroscopicMode },
        { "set_auto_disable_average_samples_count", SetAutoDisableAverageSamplesCount },
        { "set_auto_disable_steps", SetAutoDisableSteps },
        { "get_auto_disable_average_samples_count", GetAutoDisableAverageSamplesCount },
        { "get_auto_disable_steps", GetAutoDisableSteps },
        { "set_position", SetPosition },
        { "set_linear_vel", SetLinearVel  },
        { "set_angular_vel", SetAngularVel },
        { "set_force", SetForce },
        { "set_torque", SetTorque },
        { "set_finite_rotation_axis", SetFiniteRotationAxis },
        { "get_position", GetPosition },
        { "get_finite_rotation_axis", GetFiniteRotationAxis },
        { "get_linear_vel", GetLinearVel },
        { "get_angular_vel", GetAngularVel },
        { "get_force", GetForce },
        { "get_torque", GetTorque },
        { "add_force", AddForce },
        { "add_torque", AddTorque },
        { "add_rel_force", AddRelForce },
        { "add_rel_torque", AddRelTorque },
        { "add_force_at_pos", AddForceAtPos },
        { "add_force_at_rel_pos", AddForceAtRelPos },
        { "add_rel_force_at_pos", AddRelForceAtPos },
        { "add_rel_force_at_rel_pos", AddRelForceAtRelPos },
        { "get_rel_point_pos", GetRelPointPos },
        { "get_rel_point_vel", GetRelPointVel },
        { "get_point_vel", GetPointVel },
        { "get_pos_rel_point", GetPosRelPoint },
        { "vector_to_world", VectorToWorld },
        { "vector_from_world", VectorFromWorld },
        { "set_rotation", SetRotation },
        { "get_rotation", GetRotation },
        { "set_quaternion", SetQuaternion},
        { "get_quaternion", GetQuaternion},
        { "get_num_joints", GetNumJoints },
//      { "get_joint", GetJoint },
        { "get_joints", GetJoints },
        { "get_world", GetWorld },
        { "set_mass", SetMass },
        { "get_mass", GetMass },
        { "get_geoms", GetGeoms },
        { "set_moved_callback", SetMovedCallback },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  Destroy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "create_body", Create },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_body(lua_State *L)
    {
    udata_define(L, BODY_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

#if 0
//Not used:
//void *dBodyGetData(body_t);
//void  dBodySetData(body_t, void *data);
#endif

