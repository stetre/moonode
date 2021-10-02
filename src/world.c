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

static int freeworld(lua_State *L, ud_t *ud)
    {
    world_t world = (world_t)ud->handle;
    freechildren(L, JOINT_BALL_MT, ud);
    freechildren(L, JOINT_HINGE_MT, ud);
    freechildren(L, JOINT_SLIDER_MT, ud);
    freechildren(L, JOINT_CONTACT_MT, ud);
    freechildren(L, JOINT_HINGE2_MT, ud);
    freechildren(L, JOINT_UNIVERSAL_MT, ud);
    freechildren(L, JOINT_PR_MT, ud);
    freechildren(L, JOINT_PU_MT, ud);
    freechildren(L, JOINT_PISTON_MT, ud);
    freechildren(L, JOINT_FIXED_MT, ud);
    freechildren(L, JOINT_NULL_MT, ud);
    freechildren(L, JOINT_AMOTOR_MT, ud);
    freechildren(L, JOINT_LMOTOR_MT, ud);
    freechildren(L, JOINT_PLANE2D_MT, ud);
    freechildren(L, JOINT_DBALL_MT, ud);
    freechildren(L, JOINT_DHINGE_MT, ud);
    freechildren(L, JOINT_TRANSMISSION_MT, ud);
    freechildren(L, BODY_MT, ud);
    if(!freeuserdata(L, ud, "world")) return 0;
    dWorldDestroy(world);
    return 0;
    }

static int newworld(lua_State *L, world_t world)
    {
    ud_t *ud;
    ud = newuserdata(L, world, WORLD_MT, "world");
    ud->parent_ud = NULL;
    ud->destructor = freeworld;
    return 1;
    }

static int Create(lua_State *L)
    {
    world_t world = dWorldCreate();
    return newworld(L, world);
    }

static int SetGravity(lua_State *L)
    {
    vec3_t gravity;
    world_t world = checkworld(L, 1, NULL);
    checkvec3(L, 2, gravity);
    dWorldSetGravity(world, gravity[0], gravity[1], gravity[2]);
    return 0;
    }

static int GetGravity(lua_State *L)
    {
    vec3_t gravity;
    world_t world = checkworld(L, 1, NULL);
    dWorldGetGravity(world, gravity);
    pushvec3(L, gravity);
    return 1;
    }

#define F(Func, func)   /* void func(world_t, double) */    \
static int Func(lua_State *L)                               \
    {                                                       \
    world_t world = checkworld(L, 1, NULL);                 \
    double val = luaL_checknumber(L, 2);                    \
    func(world, val);                                       \
    return 0;                                               \
    }
F(SetERP, dWorldSetERP)
F(SetCFM, dWorldSetCFM)
F(SetQuickStepW, dWorldSetQuickStepW)
F(SetContactMaxCorrectingVel, dWorldSetContactMaxCorrectingVel)
F(SetContactSurfaceLayer, dWorldSetContactSurfaceLayer)
F(SetAutoDisableLinearThreshold, dWorldSetAutoDisableLinearThreshold)
F(SetAutoDisableAngularThreshold, dWorldSetAutoDisableAngularThreshold)
F(SetAutoDisableTime, dWorldSetAutoDisableTime)
F(SetLinearDampingThreshold, dWorldSetLinearDampingThreshold)
F(SetAngularDampingThreshold, dWorldSetAngularDampingThreshold)
F(SetLinearDamping, dWorldSetLinearDamping)
F(SetAngularDamping, dWorldSetAngularDamping)
F(SetMaxAngularSpeed, dWorldSetMaxAngularSpeed)
#undef F

#define F(Func, func)   /* double func(world_t) */          \
static int Func(lua_State *L)                               \
    {                                                       \
    world_t world = checkworld(L, 1, NULL);                 \
    lua_pushnumber(L, func(world));                         \
    return 1;                                               \
    }
F(GetERP, dWorldGetERP)
F(GetCFM, dWorldGetCFM)
F(GetQuickStepW, dWorldGetQuickStepW)
F(GetContactMaxCorrectingVel, dWorldGetContactMaxCorrectingVel)
F(GetContactSurfaceLayer, dWorldGetContactSurfaceLayer)
F(GetAutoDisableLinearThreshold, dWorldGetAutoDisableLinearThreshold)
F(GetAutoDisableAngularThreshold, dWorldGetAutoDisableAngularThreshold)
F(GetAutoDisableTime, dWorldGetAutoDisableTime)
F(GetLinearDampingThreshold, dWorldGetLinearDampingThreshold)
F(GetAngularDampingThreshold, dWorldGetAngularDampingThreshold)
F(GetLinearDamping, dWorldGetLinearDamping)
F(GetAngularDamping, dWorldGetAngularDamping)
F(GetMaxAngularSpeed, dWorldGetMaxAngularSpeed)
#undef F

static int SetDamping(lua_State *L)
    {
    world_t world = checkworld(L, 1, NULL);
    double linear = luaL_checknumber(L, 2);
    double angular = luaL_checknumber(L, 3);
    dWorldSetDamping(world, linear, angular);
    return 0;
    }

static int GetDamping(lua_State *L)
    {
    world_t world = checkworld(L, 1, NULL);
    lua_pushnumber(L, dWorldGetLinearDamping(world));
    lua_pushnumber(L, dWorldGetAngularDamping(world));
    return 2;
    }

#define F(Func, func)   /* void func(world_t, int) */       \
static int Func(lua_State *L)                               \
    {                                                       \
    world_t world = checkworld(L, 1, NULL);                 \
    int val = luaL_checkinteger(L, 2);                      \
    func(world, val);                                       \
    return 0;                                               \
    }
F(SetQuickStepNumIterations, dWorldSetQuickStepNumIterations)
F(SetAutoDisableSteps, dWorldSetAutoDisableSteps)
F(SetAutoDisableAverageSamplesCount, dWorldSetAutoDisableAverageSamplesCount)
#undef F

#define F(Func, func)   /* int func(world_t) */             \
static int Func(lua_State *L)                               \
    {                                                       \
    world_t world = checkworld(L, 1, NULL);                 \
    lua_pushinteger(L, func(world));                        \
    return 1;                                               \
    }
F(GetQuickStepNumIterations, dWorldGetQuickStepNumIterations)
F(GetAutoDisableSteps, dWorldGetAutoDisableSteps)
F(GetAutoDisableAverageSamplesCount, dWorldGetAutoDisableAverageSamplesCount)
#undef F

#define F(Func, func)   /* void func(world_t, boolean) */   \
static int Func(lua_State *L)                               \
    {                                                       \
    world_t world = checkworld(L, 1, NULL);                 \
    int val = checkboolean(L, 2);                           \
    func(world, val);                                       \
    return 0;                                               \
    }
F(SetAutoDisableFlag, dWorldSetAutoDisableFlag)
#undef F

#define F(Func, func)   /* boolean func(world_t) */         \
static int Func(lua_State *L)                               \
    {                                                       \
    world_t world = checkworld(L, 1, NULL);                 \
    lua_pushboolean(L, func(world));                        \
    return 1;                                               \
    }
F(GetAutoDisableFlag, dWorldGetAutoDisableFlag)
#undef F

static int Step(lua_State *L)
    {
    world_t world = checkworld(L, 1, NULL);
    double stepsize = luaL_checknumber(L, 2);
    int rc = dWorldStep(world, stepsize);
    if(!rc) return failure(L, ERR_OPERATION);
    return 0;
    }

static int QuickStep(lua_State *L) /* wolf pack ;-) */
    {
    world_t world = checkworld(L, 1, NULL);
    double stepsize = luaL_checknumber(L, 2);
    int rc = dWorldQuickStep(world, stepsize);
    if(!rc) return failure(L, ERR_OPERATION);
    return 0;
    }

static int ImpulseToForce(lua_State *L)
    {
    vec3_t impulse, force;
    world_t world = checkworld(L, 1, NULL);
    double stepsize = luaL_checknumber(L, 2);
    checkvec3(L, 3, impulse);
    dWorldImpulseToForce(world, stepsize, impulse[0], impulse[1], impulse[2], force);
    pushvec3(L, force);
    return 1;
    }

RAW_FUNC(world)
DESTROY_FUNC(world)

static const struct luaL_Reg Methods[] = 
    {
        { "raw", Raw },
        { "destroy", Destroy },
        { "get_gravity", GetGravity },
        { "get_erp", GetERP },
        { "get_cfm", GetCFM },
        { "get_quick_step_w", GetQuickStepW },
        { "get_contact_max_correcting_vel", GetContactMaxCorrectingVel },
        { "get_contact_surface_layer", GetContactSurfaceLayer },
        { "get_auto_disable_linear_threshold", GetAutoDisableLinearThreshold },
        { "get_auto_disable_angular_threshold", GetAutoDisableAngularThreshold },
        { "get_auto_disable_time", GetAutoDisableTime },
        { "get_linear_damping_threshold", GetLinearDampingThreshold },
        { "get_angular_damping_threshold", GetAngularDampingThreshold },
        { "get_linear_damping", GetLinearDamping },
        { "get_angular_damping", GetAngularDamping },
        { "get_max_angular_speed", GetMaxAngularSpeed },
        { "set_gravity", SetGravity },
        { "set_erp", SetERP },
        { "set_cfm", SetCFM },
        { "set_quick_step_w", SetQuickStepW },
        { "set_contact_max_correcting_vel", SetContactMaxCorrectingVel },
        { "set_contact_surface_layer", SetContactSurfaceLayer },
        { "set_auto_disable_linear_threshold", SetAutoDisableLinearThreshold },
        { "set_auto_disable_angular_threshold", SetAutoDisableAngularThreshold },
        { "set_auto_disable_time", SetAutoDisableTime },
        { "set_linear_damping_threshold", SetLinearDampingThreshold },
        { "set_angular_damping_threshold", SetAngularDampingThreshold },
        { "set_linear_damping", SetLinearDamping },
        { "set_angular_damping", SetAngularDamping },
        { "set_max_angular_speed", SetMaxAngularSpeed },
        { "set_damping", SetDamping },
        { "get_damping", GetDamping },
        { "set_quick_step_num_iterations", SetQuickStepNumIterations },
        { "set_auto_disable_steps", SetAutoDisableSteps },
        { "get_quick_step_num_iterations", GetQuickStepNumIterations },
        { "get_auto_disable_steps", GetAutoDisableSteps },
        { "set_auto_disable_average_samples_count", SetAutoDisableAverageSamplesCount },
        { "get_auto_disable_average_samples_count", GetAutoDisableAverageSamplesCount },
        { "set_auto_disable_flag", SetAutoDisableFlag },
        { "get_auto_disable_flag", GetAutoDisableFlag },
        { "step", Step },
        { "quick_step", QuickStep },
        { "impulse_to_force", ImpulseToForce },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  Destroy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "create_world", Create },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_world(lua_State *L)
    {
    udata_define(L, WORLD_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

#if 0
//@@TODO? void dWorldExportDIF(dWorldID w, FILE *file, const char *world_name);
// Not used:
//void dWorldSetData(world_t world, void* data);
//void* dWorldGetData(world_t world);
//void dWorldSetStepIslandsProcessingMaxThreadCount(world_t w, unsigned count);
//unsigned dWorldGetStepIslandsProcessingMaxThreadCount(world_t w);
//int dWorldUseSharedWorkingMemory(world_t w, world_t from_world/*=NULL*/);
//void dWorldCleanupWorkingMemory(world_t w);
//int dWorldSetStepMemoryReservationPolicy(world_t w, const dWorldStepReserveInfo *policyinfo/*=NULL*/);
//int dWorldSetStepMemoryManager(world_t w, const dWorldStepMemoryFunctionsInfo *memfuncs);
//void dWorldSetStepThreadingImplementation(world_t w, const dThreadingFunctionsInfo *functions_info, dThreadingImplementationID threading_impl);
#endif

