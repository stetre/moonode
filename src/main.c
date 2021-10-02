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

lua_State *moonode_L;

static void AtExit(void)
    {
    if(moonode_L)
        {
        dCloseODE();
        enums_free_all(moonode_L);
        moonode_L = NULL;
        }
    }
 
static int AddVersions(lua_State *L)
    {
    lua_pushstring(L, "_VERSION");
    lua_pushstring(L, "MoonODE "MOONODE_VERSION);
    lua_settable(L, -3);

    lua_pushstring(L, "_ODE_VERSION");
    lua_pushstring(L, "ODE "dODE_VERSION);
    lua_settable(L, -3);

    return 0;
    }

static int Init(lua_State *L)
    {
    dCloseODE();
    if(dInitODE2(dAllocateMaskAll)==0)
        return luaL_error(L, "ODE init failed");
    return 0;
    }

static int GetConfiguration(lua_State *L)
    {
    const char* s = dGetConfiguration();
    lua_pushstring(L, s);
    return 1;
    }

static int CheckConfiguration(lua_State *L)
    {
    const char* token = luaL_checkstring(L, 1);
    lua_pushboolean(L, dCheckConfiguration(token));
    return 1;
    }

static int IsGlmathCompat(lua_State *L)
    {
    lua_pushboolean(L, isglmathcompat());
    return 1;
    }

static int GlmathCompat(lua_State *L)
    {
    int on = checkboolean(L, 1);
    glmathcompat(L, on);
    return 0;
    }

/* These functions convert plain tables to glmath types if glmath_compat
 * is enabled, otherwise they just return a copy of the passed table.
 * (Their purpose is to be used by submodules like moonode/random.lua,
 * that can't know if glmath is enabled and can't require() it because
 * this is a user choice.
 */
#define F(Func, what, T)            \
static int Func(lua_State *L)       \
    {                               \
    T x;                            \
    check##what(L, 1, x);           \
    push##what(L, x);               \
    return 1;                       \
    }
F(Vec3, vec3, vec3_t)
F(Vec4, vec4, vec4_t)
F(Mat3, mat3, mat3_t)
F(Mat4, mat4, mat4_t)
F(Quat, quat, quat_t)
F(Box3, box3, box3_t)
#undef F

static const struct luaL_Reg Functions[] = 
    {
        { "init", Init },
        { "get_configuration", GetConfiguration },
        { "check_configuration", CheckConfiguration },
        { "is_glmath_compat", IsGlmathCompat },
        { "glmath_compat", GlmathCompat },
        { "vec3", Vec3 },
        { "vec4", Vec4 },
        { "mat3", Mat3 },
        { "mat4", Mat4 },
        { "quat", Quat },
        { "box3", Box3 },
        { NULL, NULL } /* sentinel */
    };

int luaopen_moonode(lua_State *L)
/* Lua calls this function to load the module */
    {
    moonode_L = L;

    if(dInitODE2(dAllocateMaskAll)==0)
        return luaL_error(L, "ODE init failed");
    moonode_utils_init(L);
    atexit(AtExit);

    lua_newtable(L); /* the module table */
    moonode_open_enums(L);
    moonode_open_flags(L);
    AddVersions(L);
    luaL_setfuncs(L, Functions, 0);
    moonode_open_mass(L);
    moonode_open_collide(L);
    moonode_open_tracing(L);
    moonode_open_misc(L);
    moonode_open_world(L);
    moonode_open_body(L);
    moonode_open_joint(L);
    moonode_open_joint_ball(L);
    moonode_open_joint_hinge(L);
    moonode_open_joint_slider(L);
    moonode_open_joint_hinge2(L);
    moonode_open_joint_universal(L);
    moonode_open_joint_pr(L);
    moonode_open_joint_pu(L);
    moonode_open_joint_piston(L);
    moonode_open_joint_fixed(L);
    moonode_open_joint_null(L);
    moonode_open_joint_amotor(L);
    moonode_open_joint_lmotor(L);
    moonode_open_joint_plane2d(L);
    moonode_open_joint_dball(L);
    moonode_open_joint_dhinge(L);
    moonode_open_joint_transmission(L);
    moonode_open_joint_contact(L);
    moonode_open_space(L);
    moonode_open_space_simple(L);
    moonode_open_space_hash(L);
    moonode_open_space_quadtree(L);
    moonode_open_space_sap(L);
    moonode_open_geom(L);
    moonode_open_geom_sphere(L);
    moonode_open_geom_convex(L);
    moonode_open_geom_box(L);
    moonode_open_geom_plane(L);
    moonode_open_geom_capsule(L);
    moonode_open_geom_cylinder(L);
    moonode_open_geom_ray(L);
    moonode_open_geom_heightfield(L);
    moonode_open_geom_trimesh(L);
    moonode_open_hfdata(L);
    moonode_open_tmdata(L);
    moonode_open_datahandling(L);

    /* Add functions implemented in Lua */
    lua_pushvalue(L, -1); lua_setglobal(L, "moonode");
    if(luaL_dostring(L, "require('moonode.random')") != 0) lua_error(L);
    lua_pushnil(L);  lua_setglobal(L, "moonode");

    return 1;
    }

#if 0
//THREADING @@
//int dInitODE2(unsigned int uiInitFlags/*=0*/);
//int dAllocateODEDataForThread(unsigned int uiAllocateFlags);
//void dCleanupODEAllDataForThread();
#endif
