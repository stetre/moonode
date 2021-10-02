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

#ifndef internalDEFINED
#define internalDEFINED

#ifdef LINUX
#define _ISOC11_SOURCE /* see man aligned_alloc(3) */
#endif
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "moonode.h"

#define TOSTR_(x) #x
#define TOSTR(x) TOSTR_(x)

#include "tree.h"
#include "objects.h"
#include "enums.h"

/* Note: all the dynamic symbols of this library (should) start with 'moonode_' .
 * The only exception is the luaopen_moonode() function, which is searched for
 * with that name by Lua.
 * MoonODE's string references on the Lua registry also start with 'moonode_'.
 */

#if 0
/* .c */
#define  moonode_
#endif

/* flags.c */
#define checkflags(L, arg) luaL_checkinteger((L), (arg))
#define optflags(L, arg, defval) luaL_optinteger((L), (arg), (defval))
#define pushflags(L, val) lua_pushinteger((L), (val))

/* utils.c */
void moonode_utils_init(lua_State *L);
#define copytable moonode_copytable
int copytable(lua_State *L);
#define noprintf moonode_noprintf
int noprintf(const char *fmt, ...); 
#define now moonode_now
double now(void);
#define sleeep moonode_sleeep
void sleeep(double seconds);
#define since(t) (now() - (t))
#define notavailable moonode_notavailable
int notavailable(lua_State *L, ...);
#define Malloc moonode_Malloc
void *Malloc(lua_State *L, size_t size);
#define MallocNoErr moonode_MallocNoErr
void *MallocNoErr(lua_State *L, size_t size);
#define Strdup moonode_Strdup
char *Strdup(lua_State *L, const char *s);
#define Strndup moonode_Strndup
char *Strndup(lua_State *L, const char *s, size_t len);
#define Free moonode_Free
void Free(lua_State *L, void *ptr);
#define checkboolean moonode_checkboolean
int checkboolean(lua_State *L, int arg);
#define testboolean moonode_testboolean
int testboolean(lua_State *L, int arg, int *err);
#define optboolean moonode_optboolean
int optboolean(lua_State *L, int arg, int d);
#define checklightuserdata moonode_checklightuserdata
void *checklightuserdata(lua_State *L, int arg);
#define checklightuserdataorzero moonode_checklightuserdataorzero
void *checklightuserdataorzero(lua_State *L, int arg);
#define optlightuserdata moonode_optlightuserdata
void *optlightuserdata(lua_State *L, int arg);
#define testindex moonode_testindex
int testindex(lua_State *L, int arg, int *err);
#define checkindex moonode_checkindex
int checkindex(lua_State *L, int arg);
#define optindex moonode_optindex
int optindex(lua_State *L, int arg, int optval);
#define pushindex moonode_pushindex
void pushindex(lua_State *L, int val);

/* datastructs.c */
#define isglmathcompat moonode_isglmathcompat
int isglmathcompat(void);
#define glmathcompat moonode_glmathcompat
int glmathcompat(lua_State *L, int on);

#define testvec3 moonode_testvec3
int testvec3(lua_State *L, int arg, vec3_t dst);
#define optvec3 moonode_optvec3
int optvec3(lua_State *L, int arg, vec3_t dst);
#define checkvec3 moonode_checkvec3
int checkvec3(lua_State *L, int arg, vec3_t dst);
#define pushvec3 moonode_pushvec3
void pushvec3(lua_State *L, const vec3_t val);
#define checkvec3list moonode_checkvec3list
vec3_t *checkvec3list(lua_State *L, int arg, int *countp, int *err);
#define pushvec3list moonode_pushvec3list
void pushvec3list(lua_State *L, const vec3_t *vecs, int count);

#define testvec4 moonode_testvec4
int testvec4(lua_State *L, int arg, vec4_t dst);
#define optvec4 moonode_optvec4
int optvec4(lua_State *L, int arg, vec4_t dst);
#define checkvec4 moonode_checkvec4
int checkvec4(lua_State *L, int arg, vec4_t dst);
#define pushvec4 moonode_pushvec4
void pushvec4(lua_State *L, const vec4_t val);
#define checkvec4list moonode_checkvec4list
vec4_t *checkvec4list(lua_State *L, int arg, int *countp, int *err);
#define pushvec4list moonode_pushvec4list
void pushvec4list(lua_State *L, const vec4_t *vecs, int count);

#define testquat moonode_testquat
int testquat(lua_State *L, int arg, quat_t dst);
#define optquat moonode_optquat
int optquat(lua_State *L, int arg, quat_t dst);
#define checkquat moonode_checkquat
int checkquat(lua_State *L, int arg, quat_t dst);
#define pushquat moonode_pushquat
void pushquat(lua_State *L, const quat_t val);
#define checkquatlist moonode_checkquatlist
quat_t *checkquatlist(lua_State *L, int arg, int *countp, int *err);
#define pushquatlist moonode_pushquatlist
void pushquatlist(lua_State *L, const quat_t *quats, int count);

#define testmat3 moonode_testmat3
int testmat3(lua_State *L, int arg, mat3_t dst);
#define optmat3 moonode_optmat3
int optmat3(lua_State *L, int arg, mat3_t dst);
#define checkmat3 moonode_checkmat3
int checkmat3(lua_State *L, int arg, mat3_t dst);
#define pushmat3 moonode_pushmat3
void pushmat3(lua_State *L, const mat3_t val);

#define testmat4 moonode_testmat4
int testmat4(lua_State *L, int arg, mat4_t dst);
#define optmat4 moonode_optmat4
int optmat4(lua_State *L, int arg, mat4_t dst);
#define checkmat4 moonode_checkmat4
int checkmat4(lua_State *L, int arg, mat4_t dst);
#define pushmat4 moonode_pushmat4
void pushmat4(lua_State *L, const mat4_t val);

#define testbox3 moonode_testbox3
int testbox3(lua_State *L, int arg, box3_t dst);
#define optbox3 moonode_optbox3
int optbox3(lua_State *L, int arg, box3_t dst);
#define checkbox3 moonode_checkbox3
int checkbox3(lua_State *L, int arg, box3_t dst);
#define pushbox3 moonode_pushbox3
void pushbox3(lua_State *L, const box3_t val);

#define checkpolygons moonode_checkpolygons
unsigned int *checkpolygons(lua_State *L, int arg, unsigned int *countp, int *err);

#define checkmass moonode_checkmass
int checkmass(lua_State *L, int arg, mass_t *dst);
#define pushmass moonode_pushmass
void pushmass(lua_State *L, mass_t *val);
#define checkcontactpoint moonode_checkcontactpoint
int checkcontactpoint(lua_State *L, int arg, contact_point_t *dst);
#define pushcontactpoint moonode_pushcontactpoint
void pushcontactpoint(lua_State *L, contact_point_t *val);
#define checksurfaceparameters moonode_checksurfaceparameters
int checksurfaceparameters(lua_State *L, int arg, surface_parameters_t *dst);

/* Internal error codes */
#define ERR_NOTPRESENT       1
#define ERR_SUCCESS          0
#define ERR_GENERIC         -1
#define ERR_TYPE            -2
#define ERR_ELEMTYPE        -3
#define ERR_VALUE           -4
#define ERR_ELEMVALUE       -5
#define ERR_TABLE           -6
#define ERR_FUNCTION        -7
#define ERR_EMPTY           -8
#define ERR_MEMORY          -9
#define ERR_MALLOC_ZERO     -10
#define ERR_LENGTH          -11
#define ERR_POOL            -12
#define ERR_BOUNDARIES      -13
#define ERR_RANGE           -14
#define ERR_FOPEN           -15
#define ERR_OPERATION       -16
#define ERR_UNKNOWN         -17
#define errstring moonode_errstring
const char* errstring(int err);

/* tracing.c */
#define trace_objects moonode_trace_objects
extern int trace_objects;

#define raiseerror moonode_raiseerror
int raiseerror(lua_State *L, const int ec);
#define pusherror moonode_pusherror
int pusherror(lua_State *L, const int ec);

/* joint.c */
#define jointdestroy moonode_jointdestroy
int jointdestroy(lua_State *L, joint_t joint);

/* geom.c */
#define geomdestroy moonode_geomdestroy
int geomdestroy(lua_State *L, geom_t geom);

/* space.c */
#define spacedestroy moonode_spacedestroy
int spacedestroy(lua_State *L, space_t space);

/* main.c */
extern lua_State *moonode_L;
int luaopen_moonode(lua_State *L);
void moonode_open_mass(lua_State *L);
void moonode_open_collide(lua_State *L);
void moonode_open_enums(lua_State *L);
void moonode_open_flags(lua_State *L);
void moonode_open_tracing(lua_State *L);
void moonode_open_misc(lua_State *L);
void moonode_open_world(lua_State *L);
void moonode_open_body(lua_State *L);
void moonode_open_joint(lua_State *L);
void moonode_open_joint_ball(lua_State *L);
void moonode_open_joint_hinge(lua_State *L);
void moonode_open_joint_slider(lua_State *L);
void moonode_open_joint_hinge2(lua_State *L);
void moonode_open_joint_universal(lua_State *L);
void moonode_open_joint_pr(lua_State *L);
void moonode_open_joint_pu(lua_State *L);
void moonode_open_joint_piston(lua_State *L);
void moonode_open_joint_fixed(lua_State *L);
void moonode_open_joint_null(lua_State *L);
void moonode_open_joint_amotor(lua_State *L);
void moonode_open_joint_lmotor(lua_State *L);
void moonode_open_joint_plane2d(lua_State *L);
void moonode_open_joint_dball(lua_State *L);
void moonode_open_joint_dhinge(lua_State *L);
void moonode_open_joint_transmission(lua_State *L);
void moonode_open_joint_contact(lua_State *L);
void moonode_open_space(lua_State *L);
void moonode_open_space_simple(lua_State *L);
void moonode_open_space_hash(lua_State *L);
void moonode_open_space_quadtree(lua_State *L);
void moonode_open_space_sap(lua_State *L);
void moonode_open_geom(lua_State *L);
void moonode_open_geom_sphere(lua_State *L);
void moonode_open_geom_convex(lua_State *L);
void moonode_open_geom_box(lua_State *L);
void moonode_open_geom_plane(lua_State *L);
void moonode_open_geom_capsule(lua_State *L);
void moonode_open_geom_cylinder(lua_State *L);
void moonode_open_geom_ray(lua_State *L);
void moonode_open_geom_heightfield(lua_State *L);
void moonode_open_geom_trimesh(lua_State *L);
void moonode_open_hfdata(lua_State *L);
void moonode_open_tmdata(lua_State *L);
void moonode_open_datahandling(lua_State *L);

/*------------------------------------------------------------------------------*
 | Debug and other utilities                                                    |
 *------------------------------------------------------------------------------*/

/* If this is printed, it denotes a suspect bug: */
#define UNEXPECTED_ERROR "unexpected error (%s, %d)", __FILE__, __LINE__
#define unexpected(L) luaL_error((L), UNEXPECTED_ERROR)

/* Errors with internal error code (ERR_XXX) */
#define failure(L, errcode) luaL_error((L), errstring((errcode)))
#define argerror(L, arg, errcode) luaL_argerror((L), (arg), errstring((errcode)))
#define errmemory(L) luaL_error((L), errstring((ERR_MEMORY)))

#define notsupported(L) luaL_error((L), "operation not supported")
#define badvalue(L, s)   lua_pushfstring((L), "invalid value '%s'", (s))


/* Reference/unreference variables on the Lua registry */
#define Unreference(L, ref) do {                        \
    if((ref)!= LUA_NOREF)                               \
        {                                               \
        luaL_unref((L), LUA_REGISTRYINDEX, (ref));      \
        (ref) = LUA_NOREF;                              \
        }                                               \
} while(0)

#define Reference(L, arg, ref)  do {                    \
    Unreference((L), (ref));                            \
    lua_pushvalue(L, (arg));                            \
    (ref) = luaL_ref(L, LUA_REGISTRYINDEX);             \
} while(0)

/* DEBUG -------------------------------------------------------- */
#if defined(DEBUG)

#define DBG printf
#define TR() do { printf("trace %s %d\n",__FILE__,__LINE__); } while(0)
#define BK() do { printf("break %s %d\n",__FILE__,__LINE__); getchar(); } while(0)
#define TSTART double ts = now();
#define TSTOP do {                                          \
    ts = since(ts); ts = ts*1e6;                            \
    printf("%s %d %.3f us\n", __FILE__, __LINE__, ts);      \
    ts = now();                                             \
} while(0);

#else 

#define DBG noprintf
#define TR()
#define BK()
#define TSTART do {} while(0) 
#define TSTOP do {} while(0)    

#endif /* DEBUG ------------------------------------------------- */

#endif /* internalDEFINED */
