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

#ifndef objectsDEFINED
#define objectsDEFINED

#include "tree.h"
#include "udata.h"

/* renaming (for internal use) */
#define world_t dWorldID
#define space_t dSpaceID
#define body_t dBodyID
#define geom_t dGeomID
#define joint_t dJointID
#define jointfeedback_t dJointFeedback
#define vec3_t dVector3
#define vec4_t dVector4
#define mat3_t dMatrix3
#define mat4_t dMatrix4
#define quat_t dQuaternion
#define mass_t dMass
typedef double box3_t[6];
#define contact_point_t dContactGeom
#define contact_t dContact
#define surface_parameters_t dSurfaceParameters
#define hfdata_t dHeightfieldDataID
#define tmdata_t dTriMeshDataID

/* Objects' metatable names */
#define MASS_MT "moonode_mass"
#define WORLD_MT "moonode_world"
#define SPACE_MT "moonode_space" /* base object */
#define SPACE_SIMPLE_MT "moonode_space_simple"
#define SPACE_HASH_MT "moonode_space_hash"
#define SPACE_QUADTREE_MT "moonode_space_quadtree"
#define SPACE_SAP_MT "moonode_space_sap" // sap = 'sweep and prune'
#define BODY_MT "moonode_body"
#define JOINT_MT "moonode_joint" /* base object */
#define JOINT_BALL_MT "moonode_joint_ball"
#define JOINT_HINGE_MT "moonode_joint_hinge"
#define JOINT_SLIDER_MT "moonode_joint_slider"
#define JOINT_CONTACT_MT "moonode_joint_contact"
#define JOINT_HINGE2_MT "moonode_joint_hinge2"
#define JOINT_UNIVERSAL_MT "moonode_joint_universal"
#define JOINT_PR_MT "moonode_joint_pr"
#define JOINT_PU_MT "moonode_joint_pu"
#define JOINT_PISTON_MT "moonode_joint_piston"
#define JOINT_FIXED_MT "moonode_joint_fixed"
#define JOINT_NULL_MT "moonode_joint_null"
#define JOINT_AMOTOR_MT "moonode_joint_amotor"
#define JOINT_LMOTOR_MT "moonode_joint_lmotor"
#define JOINT_PLANE2D_MT "moonode_joint_plane2d"
#define JOINT_DBALL_MT "moonode_joint_dball"
#define JOINT_DHINGE_MT "moonode_joint_dhinge"
#define JOINT_TRANSMISSION_MT "moonode_joint_transmission"
#define GEOM_MT "moonode_geom" /* base object */
#define GEOM_SPHERE_MT "moonode_geom_sphere"
#define GEOM_CONVEX_MT "moonode_geom_convex"
#define GEOM_BOX_MT "moonode_geom_box"
#define GEOM_PLANE_MT "moonode_geom_plane"
#define GEOM_CAPSULE_MT "moonode_geom_capsule"
#define GEOM_CYLINDER_MT "moonode_geom_cylinder"
#define GEOM_RAY_MT "moonode_geom_ray"
#define GEOM_HEIGHTFIELD_MT "moonode_geom_heightfield"
#define GEOM_TRIMESH_MT "moonode_geom_trimesh"
#define HFDATA_MT "moonode_hfdata" /* heightfield data */
#define TMDATA_MT "moonode_tmdata" /* trimesh data */

/* Userdata memory associated with objects */
#define ud_t moonode_ud_t
typedef struct moonode_ud_s ud_t;

struct moonode_ud_s {
    void *handle; /* the object handle bound to this userdata */
    int (*destructor)(lua_State *L, ud_t *ud);  /* self destructor */
    ud_t *parent_ud; /* the ud of the parent object */
    uint32_t marks;
    int ref1, ref2, ref3, ref4; /* references for callbacks, automatically unreference at deletion */
    int groupid;
    void *info; /* object specific info (ud_info_t, subject to Free() at destruction, if not NULL) */
};
    
/* Marks.  m_ = marks word (uint32_t) , i_ = bit number (0 .. 31)  */
#define MarkGet(m_,i_)  (((m_) & ((uint32_t)1<<(i_))) == ((uint32_t)1<<(i_)))
#define MarkSet(m_,i_)  do { (m_) = ((m_) | ((uint32_t)1<<(i_))); } while(0)
#define MarkReset(m_,i_) do { (m_) = ((m_) & (~((uint32_t)1<<(i_)))); } while(0)

#define IsValid(ud)             MarkGet((ud)->marks, 0)
#define MarkValid(ud)           MarkSet((ud)->marks, 0) 
#define CancelValid(ud)         MarkReset((ud)->marks, 0)

#if 0
/* .c */
#define  moonode_
#endif

#define setmetatable moonode_setmetatable
int setmetatable(lua_State *L, const char *mt);

#define newuserdata moonode_newuserdata
ud_t *newuserdata(lua_State *L, void *handle, const char *mt, const char *tracename);
#define freeuserdata moonode_freeuserdata
int freeuserdata(lua_State *L, ud_t *ud, const char *tracename);
#define pushuserdata moonode_pushuserdata 
int pushuserdata(lua_State *L, ud_t *ud);

#define freechildren moonode_freechildren
int freechildren(lua_State *L,  const char *mt, ud_t *parent_ud);

#define userdata_unref(L, handle) udata_unref((L),(handle))

#define UD(handle) userdata((handle)) /* dispatchable objects only */
#define userdata moonode_userdata
ud_t *userdata(void *handle);
#define testxxx moonode_testxxx
void *testxxx(lua_State *L, int arg, ud_t **udp, const char *mt);
#define checkxxx moonode_checkxxx
void *checkxxx(lua_State *L, int arg, ud_t **udp, const char *mt);
#define optxxx moonode_optxxx
void *optxxx(lua_State *L, int arg, ud_t **udp, const char *mt);
#define pushxxx moonode_pushxxx
int pushxxx(lua_State *L, void *handle);
#if 0
#define freexxxlist moonode_freexxxlist
void freexxxlist(lua_State *L, vector_ptr_t *v);
#define checkxxxlist moonode_checkxxxlist
vector_ptr_t* checkxxxlist(lua_State *L, int arg, int *err, const char *mt);
#endif

#define checkgeomorspace moonode_checkgeomorspace
geom_t checkgeomorspace(lua_State *L, int arg, ud_t **udp);

#if 0 // 7yy
/* zzz.c */
#define checkzzz(L, arg, udp) (zzz_t)checkxxx((L), (arg), (udp), ZZZ_MT)
#define testzzz(L, arg, udp) (zzz_t)testxxx((L), (arg), (udp), ZZZ_MT)
#define optzzz(L, arg, udp) (zzz_t)optxxx((L), (arg), (udp), ZZZ_MT)
#define freezzzlist freexxxlist
#define checkzzzlist(L, arg, err) checkxxxlist((L), (arg), (err), ZZZ_MT)
#define pushzzz(L, handle) pushxxx((L), (void*)(handle))

#endif

/* world.c */
#define checkworld(L, arg, udp) (world_t)checkxxx((L), (arg), (udp), WORLD_MT)
#define testworld(L, arg, udp) (world_t)testxxx((L), (arg), (udp), WORLD_MT)
#define optworld(L, arg, udp) (world_t)optxxx((L), (arg), (udp), WORLD_MT)
#define freeworldlist freexxxlist
#define checkworldlist(L, arg, err) checkxxxlist((L), (arg), (err), WORLD_MT)
#define pushworld(L, handle) pushxxx((L), (void*)(handle))

/* body.c */
#define checkbody(L, arg, udp) (body_t)checkxxx((L), (arg), (udp), BODY_MT)
#define testbody(L, arg, udp) (body_t)testxxx((L), (arg), (udp), BODY_MT)
#define optbody(L, arg, udp) (body_t)optxxx((L), (arg), (udp), BODY_MT)
#define freebodylist freexxxlist
#define checkbodylist(L, arg, err) checkxxxlist((L), (arg), (err), BODY_MT)
#define pushbody(L, handle) pushxxx((L), (void*)(handle))

/* joint.c */
#define checkjoint(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_MT)
#define testjoint(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_MT)
#define optjoint(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_MT)
#define freejointlist freexxxlist
#define checkjointlist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_MT)
#define pushjoint(L, handle) pushxxx((L), (void*)(handle))

/* joint_ball.c */
#define checkjoint_ball(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_BALL_MT)
#define testjoint_ball(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_BALL_MT)
#define optjoint_ball(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_BALL_MT)
#define freejoint_balllist freexxxlist
#define checkjoint_balllist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_BALL_MT)
#define pushjoint_ball(L, handle) pushxxx((L), (void*)(handle))

/* joint_hinge.c */
#define checkjoint_hinge(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_HINGE_MT)
#define testjoint_hinge(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_HINGE_MT)
#define optjoint_hinge(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_HINGE_MT)
#define freejoint_hingelist freexxxlist
#define checkjoint_hingelist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_HINGE_MT)
#define pushjoint_hinge(L, handle) pushxxx((L), (void*)(handle))

/* joint_slider.c */
#define checkjoint_slider(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_SLIDER_MT)
#define testjoint_slider(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_SLIDER_MT)
#define optjoint_slider(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_SLIDER_MT)
#define freejoint_sliderlist freexxxlist
#define checkjoint_sliderlist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_SLIDER_MT)
#define pushjoint_slider(L, handle) pushxxx((L), (void*)(handle))

/* joint_contact.c */
#define checkjoint_contact(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_CONTACT_MT)
#define testjoint_contact(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_CONTACT_MT)
#define optjoint_contact(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_CONTACT_MT)
#define freejoint_contactlist freexxxlist
#define checkjoint_contactlist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_CONTACT_MT)
#define pushjoint_contact(L, handle) pushxxx((L), (void*)(handle))

/* joint_hinge2.c */
#define checkjoint_hinge2(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_HINGE2_MT)
#define testjoint_hinge2(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_HINGE2_MT)
#define optjoint_hinge2(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_HINGE2_MT)
#define freejoint_hinge2list freexxxlist
#define checkjoint_hinge2list(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_HINGE2_MT)
#define pushjoint_hinge2(L, handle) pushxxx((L), (void*)(handle))

/* joint_universal.c */
#define checkjoint_universal(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_UNIVERSAL_MT)
#define testjoint_universal(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_UNIVERSAL_MT)
#define optjoint_universal(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_UNIVERSAL_MT)
#define freejoint_universallist freexxxlist
#define checkjoint_universallist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_UNIVERSAL_MT)
#define pushjoint_universal(L, handle) pushxxx((L), (void*)(handle))

/* joint_pr.c */
#define checkjoint_pr(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_PR_MT)
#define testjoint_pr(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_PR_MT)
#define optjoint_pr(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_PR_MT)
#define freejoint_prlist freexxxlist
#define checkjoint_prlist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_PR_MT)
#define pushjoint_pr(L, handle) pushxxx((L), (void*)(handle))

/* joint_pu.c */
#define checkjoint_pu(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_PU_MT)
#define testjoint_pu(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_PU_MT)
#define optjoint_pu(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_PU_MT)
#define freejoint_pulist freexxxlist
#define checkjoint_pulist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_PU_MT)
#define pushjoint_pu(L, handle) pushxxx((L), (void*)(handle))

/* joint_piston.c */
#define checkjoint_piston(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_PISTON_MT)
#define testjoint_piston(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_PISTON_MT)
#define optjoint_piston(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_PISTON_MT)
#define freejoint_pistonlist freexxxlist
#define checkjoint_pistonlist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_PISTON_MT)
#define pushjoint_piston(L, handle) pushxxx((L), (void*)(handle))

/* joint_fixed.c */
#define checkjoint_fixed(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_FIXED_MT)
#define testjoint_fixed(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_FIXED_MT)
#define optjoint_fixed(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_FIXED_MT)
#define freejoint_fixedlist freexxxlist
#define checkjoint_fixedlist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_FIXED_MT)
#define pushjoint_fixed(L, handle) pushxxx((L), (void*)(handle))

/* joint_null.c */
#define checkjoint_null(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_NULL_MT)
#define testjoint_null(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_NULL_MT)
#define optjoint_null(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_NULL_MT)
#define freejoint_nulllist freexxxlist
#define checkjoint_nulllist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_NULL_MT)
#define pushjoint_null(L, handle) pushxxx((L), (void*)(handle))

/* joint_amotor.c */
#define checkjoint_amotor(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_AMOTOR_MT)
#define testjoint_amotor(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_AMOTOR_MT)
#define optjoint_amotor(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_AMOTOR_MT)
#define freejoint_amotorlist freexxxlist
#define checkjoint_amotorlist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_AMOTOR_MT)
#define pushjoint_amotor(L, handle) pushxxx((L), (void*)(handle))

/* joint_lmotor.c */
#define checkjoint_lmotor(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_LMOTOR_MT)
#define testjoint_lmotor(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_LMOTOR_MT)
#define optjoint_lmotor(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_LMOTOR_MT)
#define freejoint_lmotorlist freexxxlist
#define checkjoint_lmotorlist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_LMOTOR_MT)
#define pushjoint_lmotor(L, handle) pushxxx((L), (void*)(handle))

/* joint_plane2d.c */
#define checkjoint_plane2d(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_PLANE2D_MT)
#define testjoint_plane2d(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_PLANE2D_MT)
#define optjoint_plane2d(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_PLANE2D_MT)
#define freejoint_plane2dlist freexxxlist
#define checkjoint_plane2dlist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_PLANE2D_MT)
#define pushjoint_plane2d(L, handle) pushxxx((L), (void*)(handle))

/* joint_dball.c */
#define checkjoint_dball(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_DBALL_MT)
#define testjoint_dball(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_DBALL_MT)
#define optjoint_dball(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_DBALL_MT)
#define freejoint_dballlist freexxxlist
#define checkjoint_dballlist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_DBALL_MT)
#define pushjoint_dball(L, handle) pushxxx((L), (void*)(handle))

/* joint_dhinge.c */
#define checkjoint_dhinge(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_DHINGE_MT)
#define testjoint_dhinge(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_DHINGE_MT)
#define optjoint_dhinge(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_DHINGE_MT)
#define freejoint_dhingelist freexxxlist
#define checkjoint_dhingelist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_DHINGE_MT)
#define pushjoint_dhinge(L, handle) pushxxx((L), (void*)(handle))

/* joint_transmission.c */
#define checkjoint_transmission(L, arg, udp) (joint_t)checkxxx((L), (arg), (udp), JOINT_TRANSMISSION_MT)
#define testjoint_transmission(L, arg, udp) (joint_t)testxxx((L), (arg), (udp), JOINT_TRANSMISSION_MT)
#define optjoint_transmission(L, arg, udp) (joint_t)optxxx((L), (arg), (udp), JOINT_TRANSMISSION_MT)
#define freejoint_transmissionlist freexxxlist
#define checkjoint_transmissionlist(L, arg, err) checkxxxlist((L), (arg), (err), JOINT_TRANSMISSION_MT)
#define pushjoint_transmission(L, handle) pushxxx((L), (void*)(handle))

/* space.c */
#define checkspace(L, arg, udp) (space_t)checkxxx((L), (arg), (udp), SPACE_MT)
#define testspace(L, arg, udp) (space_t)testxxx((L), (arg), (udp), SPACE_MT)
#define optspace(L, arg, udp) (space_t)optxxx((L), (arg), (udp), SPACE_MT)
#define freespacelist freexxxlist
#define checkspacelist(L, arg, err) checkxxxlist((L), (arg), (err), SPACE_MT)
#define pushspace(L, handle) pushxxx((L), (void*)(handle))

/* space_simple.c */
#define checkspace_simple(L, arg, udp) (space_t)checkxxx((L), (arg), (udp), SPACE_SIMPLE_MT)
#define testspace_simple(L, arg, udp) (space_t)testxxx((L), (arg), (udp), SPACE_SIMPLE_MT)
#define optspace_simple(L, arg, udp) (space_t)optxxx((L), (arg), (udp), SPACE_SIMPLE_MT)
#define freespace_simplelist freexxxlist
#define checkspace_simplelist(L, arg, err) checkxxxlist((L), (arg), (err), SPACE_SIMPLE_MT)
#define pushspace_simple(L, handle) pushxxx((L), (void*)(handle))

/* space_hash.c */
#define checkspace_hash(L, arg, udp) (space_t)checkxxx((L), (arg), (udp), SPACE_HASH_MT)
#define testspace_hash(L, arg, udp) (space_t)testxxx((L), (arg), (udp), SPACE_HASH_MT)
#define optspace_hash(L, arg, udp) (space_t)optxxx((L), (arg), (udp), SPACE_HASH_MT)
#define freespace_hashlist freexxxlist
#define checkspace_hashlist(L, arg, err) checkxxxlist((L), (arg), (err), SPACE_HASH_MT)
#define pushspace_hash(L, handle) pushxxx((L), (void*)(handle))

/* space_quadtree.c */
#define checkspace_quadtree(L, arg, udp) (space_t)checkxxx((L), (arg), (udp), SPACE_QUADTREE_MT)
#define testspace_quadtree(L, arg, udp) (space_t)testxxx((L), (arg), (udp), SPACE_QUADTREE_MT)
#define optspace_quadtree(L, arg, udp) (space_t)optxxx((L), (arg), (udp), SPACE_QUADTREE_MT)
#define freespace_quadtreelist freexxxlist
#define checkspace_quadtreelist(L, arg, err) checkxxxlist((L), (arg), (err), SPACE_QUADTREE_MT)
#define pushspace_quadtree(L, handle) pushxxx((L), (void*)(handle))

/* space_sap.c */
#define checkspace_sap(L, arg, udp) (space_t)checkxxx((L), (arg), (udp), SPACE_SAP_MT)
#define testspace_sap(L, arg, udp) (space_t)testxxx((L), (arg), (udp), SPACE_SAP_MT)
#define optspace_sap(L, arg, udp) (space_t)optxxx((L), (arg), (udp), SPACE_SAP_MT)
#define freespace_saplist freexxxlist
#define checkspace_saplist(L, arg, err) checkxxxlist((L), (arg), (err), SPACE_SAP_MT)
#define pushspace_sap(L, handle) pushxxx((L), (void*)(handle))

/* geom.c */
#define checkgeom(L, arg, udp) (geom_t)checkxxx((L), (arg), (udp), GEOM_MT)
#define testgeom(L, arg, udp) (geom_t)testxxx((L), (arg), (udp), GEOM_MT)
#define optgeom(L, arg, udp) (geom_t)optxxx((L), (arg), (udp), GEOM_MT)
#define freegeomlist freexxxlist
#define checkgeomlist(L, arg, err) checkxxxlist((L), (arg), (err), GEOM_MT)
#define pushgeom(L, handle) pushxxx((L), (void*)(handle))

/* geom_sphere.c */
#define checkgeom_sphere(L, arg, udp) (geom_t)checkxxx((L), (arg), (udp), GEOM_SPHERE_MT)
#define testgeom_sphere(L, arg, udp) (geom_t)testxxx((L), (arg), (udp), GEOM_SPHERE_MT)
#define optgeom_sphere(L, arg, udp) (geom_t)optxxx((L), (arg), (udp), GEOM_SPHERE_MT)
#define freegeomlist_sphere freexxxlist
#define checkgeomlist_sphere(L, arg, err) checkxxxlist((L), (arg), (err), GEOM_SPHERE_MT)
#define pushgeom_sphere(L, handle) pushxxx((L), (void*)(handle))

/* geom_convex.c */
#define checkgeom_convex(L, arg, udp) (geom_t)checkxxx((L), (arg), (udp), GEOM_CONVEX_MT)
#define testgeom_convex(L, arg, udp) (geom_t)testxxx((L), (arg), (udp), GEOM_CONVEX_MT)
#define optgeom_convex(L, arg, udp) (geom_t)optxxx((L), (arg), (udp), GEOM_CONVEX_MT)
#define freegeom_convexlist freexxxlist
#define checkgeom_convexlist(L, arg, err) checkxxxlist((L), (arg), (err), GEOM_CONVEX_MT)
#define pushgeom_convex(L, handle) pushxxx((L), (void*)(handle))

/* geom_box.c */
#define checkgeom_box(L, arg, udp) (geom_t)checkxxx((L), (arg), (udp), GEOM_BOX_MT)
#define testgeom_box(L, arg, udp) (geom_t)testxxx((L), (arg), (udp), GEOM_BOX_MT)
#define optgeom_box(L, arg, udp) (geom_t)optxxx((L), (arg), (udp), GEOM_BOX_MT)
#define freegeom_boxlist freexxxlist
#define checkgeom_boxlist(L, arg, err) checkxxxlist((L), (arg), (err), GEOM_BOX_MT)
#define pushgeom_box(L, handle) pushxxx((L), (void*)(handle))

/* geom_plane.c */
#define checkgeom_plane(L, arg, udp) (geom_t)checkxxx((L), (arg), (udp), GEOM_PLANE_MT)
#define testgeom_plane(L, arg, udp) (geom_t)testxxx((L), (arg), (udp), GEOM_PLANE_MT)
#define optgeom_plane(L, arg, udp) (geom_t)optxxx((L), (arg), (udp), GEOM_PLANE_MT)
#define freegeom_planelist freexxxlist
#define checkgeom_planelist(L, arg, err) checkxxxlist((L), (arg), (err), GEOM_PLANE_MT)
#define pushgeom_plane(L, handle) pushxxx((L), (void*)(handle))

/* geom_capsule.c */
#define checkgeom_capsule(L, arg, udp) (geom_t)checkxxx((L), (arg), (udp), GEOM_CAPSULE_MT)
#define testgeom_capsule(L, arg, udp) (geom_t)testxxx((L), (arg), (udp), GEOM_CAPSULE_MT)
#define optgeom_capsule(L, arg, udp) (geom_t)optxxx((L), (arg), (udp), GEOM_CAPSULE_MT)
#define freegeom_capsulelist freexxxlist
#define checkgeom_capsulelist(L, arg, err) checkxxxlist((L), (arg), (err), GEOM_CAPSULE_MT)
#define pushgeom_capsule(L, handle) pushxxx((L), (void*)(handle))

/* geom_cylinder.c */
#define checkgeom_cylinder(L, arg, udp) (geom_t)checkxxx((L), (arg), (udp), GEOM_CYLINDER_MT)
#define testgeom_cylinder(L, arg, udp) (geom_t)testxxx((L), (arg), (udp), GEOM_CYLINDER_MT)
#define optgeom_cylinder(L, arg, udp) (geom_t)optxxx((L), (arg), (udp), GEOM_CYLINDER_MT)
#define freegeom_cylinderlist freexxxlist
#define checkgeom_cylinderlist(L, arg, err) checkxxxlist((L), (arg), (err), GEOM_CYLINDER_MT)
#define pushgeom_cylinder(L, handle) pushxxx((L), (void*)(handle))

/* geom_ray.c */
#define checkgeom_ray(L, arg, udp) (geom_t)checkxxx((L), (arg), (udp), GEOM_RAY_MT)
#define testgeom_ray(L, arg, udp) (geom_t)testxxx((L), (arg), (udp), GEOM_RAY_MT)
#define optgeom_ray(L, arg, udp) (geom_t)optxxx((L), (arg), (udp), GEOM_RAY_MT)
#define freegeom_raylist freexxxlist
#define checkgeom_raylist(L, arg, err) checkxxxlist((L), (arg), (err), GEOM_RAY_MT)
#define pushgeom_ray(L, handle) pushxxx((L), (void*)(handle))

/* geom_heightfield.c */
#define checkgeom_heightfield(L, arg, udp) (geom_t)checkxxx((L), (arg), (udp), GEOM_HEIGHTFIELD_MT)
#define testgeom_heightfield(L, arg, udp) (geom_t)testxxx((L), (arg), (udp), GEOM_HEIGHTFIELD_MT)
#define optgeom_heightfield(L, arg, udp) (geom_t)optxxx((L), (arg), (udp), GEOM_HEIGHTFIELD_MT)
#define freegeom_heightfieldlist freexxxlist
#define checkgeom_heightfieldlist(L, arg, err) checkxxxlist((L), (arg), (err), GEOM_HEIGHTFIELD_MT)
#define pushgeom_heightfield(L, handle) pushxxx((L), (void*)(handle))

/* hfdata.c */
#define checkhfdata(L, arg, udp) (hfdata_t)checkxxx((L), (arg), (udp), HFDATA_MT)
#define testhfdata(L, arg, udp) (hfdata_t)testxxx((L), (arg), (udp), HFDATA_MT)
#define opthfdata(L, arg, udp) (hfdata_t)optxxx((L), (arg), (udp), HFDATA_MT)
#define freehfdatalist freexxxlist
#define checkhfdatalist(L, arg, err) checkxxxlist((L), (arg), (err), HFDATA_MT)
#define pushhfdata(L, handle) pushxxx((L), (void*)(handle))

/* tmdata.c */
#define checktmdata(L, arg, udp) (tmdata_t)checkxxx((L), (arg), (udp), TMDATA_MT)
#define testtmdata(L, arg, udp) (tmdata_t)testxxx((L), (arg), (udp), TMDATA_MT)
#define opttmdata(L, arg, udp) (tmdata_t)optxxx((L), (arg), (udp), TMDATA_MT)
#define freetmdatalist freexxxlist
#define checktmdatalist(L, arg, err) checkxxxlist((L), (arg), (err), TMDATA_MT)
#define pushtmdata(L, handle) pushxxx((L), (void*)(handle))

/* geom_trimesh.c */
#define checkgeom_trimesh(L, arg, udp) (geom_t)checkxxx((L), (arg), (udp), GEOM_TRIMESH_MT)
#define testgeom_trimesh(L, arg, udp) (geom_t)testxxx((L), (arg), (udp), GEOM_TRIMESH_MT)
#define optgeom_trimesh(L, arg, udp) (geom_t)optxxx((L), (arg), (udp), GEOM_TRIMESH_MT)
#define freegeom_trimeshlist freexxxlist
#define checkgeom_trimeshlist(L, arg, err) checkxxxlist((L), (arg), (err), GEOM_TRIMESH_MT)
#define pushgeom_trimesh(L, handle) pushxxx((L), (void*)(handle))

#define RAW_FUNC(xxx)                       \
static int Raw(lua_State *L)                \
    {                                       \
    lua_pushinteger(L, (uintptr_t)check##xxx(L, 1, NULL));  \
    return 1;                               \
    }

#define TYPE_FUNC(xxx) /* NONCL */          \
static int Type(lua_State *L)               \
    {                                       \
    (void)check##xxx(L, 1, NULL);           \
    lua_pushstring(L, ""#xxx);              \
    return 1;                               \
    }

#define DESTROY_FUNC(xxx)                   \
static int Destroy(lua_State *L)            \
    {                                       \
    ud_t *ud;                               \
    (void)test##xxx(L, 1, &ud);             \
    if(!ud) return 0; /* already deleted */ \
    return ud->destructor(L, ud);           \
    }

#define PARENT_FUNC(xxx)                    \
static int Parent(lua_State *L)             \
    {                                       \
    ud_t *ud;                               \
    (void)check##xxx(L, 1, &ud);            \
    if(!ud->parent_ud) return 0;            \
    return pushuserdata(L, ud->parent_ud);  \
    }

#endif /* objectsDEFINED */
