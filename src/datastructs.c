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

#define GLMATH_COMPAT (tovec3 != LUA_NOREF)
/* References to the MoonGLMATH functions used to convert pushed values to glmath objects. */
static int tovec3 = LUA_NOREF;
static int tovec4 = LUA_NOREF;
static int tomat3 = LUA_NOREF;
static int tomat4 = LUA_NOREF;
static int toquat = LUA_NOREF;
static int tobox3 = LUA_NOREF;

int isglmathcompat(void)
    { return GLMATH_COMPAT; }

int glmathcompat(lua_State *L, int on)
    {
    if(on)
        {
        if(GLMATH_COMPAT) return 0; /* already enabled */
        if(luaL_dostring(L, "return require('moonglmath').tovec3") != 0) lua_error(L);
        tovec3 = luaL_ref(L, LUA_REGISTRYINDEX);
        if(luaL_dostring(L, "return require('moonglmath').tovec4") != 0) lua_error(L);
        tovec4 = luaL_ref(L, LUA_REGISTRYINDEX);
        if(luaL_dostring(L, "return require('moonglmath').tomat3") != 0) lua_error(L);
        tomat3 = luaL_ref(L, LUA_REGISTRYINDEX);
        if(luaL_dostring(L, "return require('moonglmath').tomat4") != 0) lua_error(L);
        tomat4 = luaL_ref(L, LUA_REGISTRYINDEX);
        if(luaL_dostring(L, "return require('moonglmath').toquat") != 0) lua_error(L);
        toquat = luaL_ref(L, LUA_REGISTRYINDEX);
        if(luaL_dostring(L, "return require('moonglmath').tobox3") != 0) lua_error(L);
        tobox3 = luaL_ref(L, LUA_REGISTRYINDEX);
        }
    else
        {
        if(!GLMATH_COMPAT) return 0; /* already disabled */
        luaL_unref(L, LUA_REGISTRYINDEX, tovec3); tovec3 = LUA_NOREF;
        luaL_unref(L, LUA_REGISTRYINDEX, tovec4); tovec4 = LUA_NOREF;
        luaL_unref(L, LUA_REGISTRYINDEX, tomat3); tomat3 = LUA_NOREF;
        luaL_unref(L, LUA_REGISTRYINDEX, tomat4); tomat4 = LUA_NOREF;
        luaL_unref(L, LUA_REGISTRYINDEX, toquat); toquat = LUA_NOREF;
        luaL_unref(L, LUA_REGISTRYINDEX, tovec3); tobox3 = LUA_NOREF;
        }
    return 0;
    }


/* vec3_t ----------------------------------------------------------*/

int testvec3(lua_State *L, int arg, vec3_t dst)
    {
    int isnum;
    int t = lua_type(L, arg);
    switch(t)
        {
        case LUA_TNONE:
        case LUA_TNIL:  return ERR_NOTPRESENT;
        case LUA_TTABLE: break;
        default: return ERR_TABLE;
        }
#define POP if(!isnum) { lua_pop(L, 1); return ERR_VALUE; } lua_pop(L, 1);
    lua_rawgeti(L, arg, 1); dst[0] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, arg, 2); dst[1] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, arg, 3); dst[2] = lua_tonumberx(L, -1, &isnum); POP
#undef POP
    return 0;
    }

int optvec3(lua_State *L, int arg, vec3_t dst)
    {
    int ec = testvec3(L, arg, dst);
    if(ec<0) return argerror(L, arg, ec);
    return ec;
    }

int checkvec3(lua_State *L, int arg, vec3_t dst)
    {
    int ec = testvec3(L, arg, dst);
    if(ec) return argerror(L, arg, ec);
    return ec;
    }

void pushvec3(lua_State *L, const vec3_t val)
    {
    if(GLMATH_COMPAT) lua_rawgeti(L, LUA_REGISTRYINDEX, tovec3);
    lua_newtable(L);
    lua_pushnumber(L, val[0]); lua_rawseti(L, -2, 1);
    lua_pushnumber(L, val[1]); lua_rawseti(L, -2, 2);
    lua_pushnumber(L, val[2]); lua_rawseti(L, -2, 3);
    if(GLMATH_COMPAT && lua_pcall(L,1,1,0)!=LUA_OK) { unexpected(L); return; }
    }

vec3_t *checkvec3list(lua_State *L, int arg, int *countp, int *err)
/* Check if the value at arg is a table of vecs and returns the corresponding
 * array of vec3_t, storing the size in *countp. The array is Malloc()'d and the
 * caller is in charge of Free()ing it.
 * If err=NULL, raises an error on failure, otherwise returns NULL and stores
 * the ERR_XXX code in *err.
 */
    {
    int count, i;
    vec3_t *dst = NULL;
    *countp = 0;
#define ERR(ec) do { if(err) *err=(ec); else argerror(L, arg, (ec)); return NULL; } while(0)
    if(lua_isnoneornil(L, arg)) ERR(ERR_NOTPRESENT);
    if(lua_type(L, arg)!=LUA_TTABLE) ERR(ERR_TABLE);

    count = luaL_len(L, arg);
    if(count==0) ERR(ERR_EMPTY);
    dst = MallocNoErr(L, count*sizeof(vec3_t));
    if(!dst) ERR(ERR_MEMORY);
    for(i=0; i<count; i++)
        {
        lua_rawgeti(L, arg, i+1);
        if(testvec3(L, -1, dst[i])!=0)
            { Free(L, dst); ERR(ERR_TYPE); }
        lua_pop(L, 1);
        }
#undef ERR
    *countp = count;
    if(err) *err=0;
    return dst;
    }

void pushvec3list(lua_State *L, const vec3_t *vecs, int count)
    {
    int i;
    lua_newtable(L);
    for(i=0; i<count; i++)
        {
        pushvec3(L, vecs[i]);
        lua_rawseti(L, -2, i+1);
        }
    }

/* vec4_t ----------------------------------------------------------*/

int testvec4(lua_State *L, int arg, vec4_t dst)
    {
    int isnum;
    int t = lua_type(L, arg);
    switch(t)
        {
        case LUA_TNONE:
        case LUA_TNIL:  return ERR_NOTPRESENT;
        case LUA_TTABLE: break;
        default: return ERR_TABLE;
        }
#define POP if(!isnum) { lua_pop(L, 1); return ERR_VALUE; } lua_pop(L, 1);
    lua_rawgeti(L, arg, 1); dst[0] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, arg, 2); dst[1] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, arg, 3); dst[2] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, arg, 4); dst[3] = lua_tonumberx(L, -1, &isnum); POP
#undef POP
    return 0;
    }

int optvec4(lua_State *L, int arg, vec4_t dst)
    {
    int ec = testvec4(L, arg, dst);
    if(ec<0) return argerror(L, arg, ec);
    return ec;
    }

int checkvec4(lua_State *L, int arg, vec4_t dst)
    {
    int ec = testvec4(L, arg, dst);
    if(ec) return argerror(L, arg, ec);
    return ec;
    }

void pushvec4(lua_State *L, const vec4_t val)
    {
    if(GLMATH_COMPAT) lua_rawgeti(L, LUA_REGISTRYINDEX, tovec4);
    lua_newtable(L);
    lua_pushnumber(L, val[0]); lua_rawseti(L, -2, 1);
    lua_pushnumber(L, val[1]); lua_rawseti(L, -2, 2);
    lua_pushnumber(L, val[2]); lua_rawseti(L, -2, 3);
    lua_pushnumber(L, val[3]); lua_rawseti(L, -2, 4);
    if(GLMATH_COMPAT && lua_pcall(L,1,1,0)!=LUA_OK) { unexpected(L); return; }
    }

vec4_t *checkvec4list(lua_State *L, int arg, int *countp, int *err)
/* Check if the value at arg is a table of vecs and returns the corresponding
 * array of vec4_t, storing the size in *countp. The array is Malloc()'d and the
 * caller is in charge of Free()ing it.
 * If err=NULL, raises an error on failure, otherwise returns NULL and stores
 * the ERR_XXX code in *err.
 */
    {
    int count, i;
    vec4_t *dst = NULL;
    *countp = 0;
#define ERR(ec) do { if(err) *err=(ec); else argerror(L, arg, (ec)); return NULL; } while(0)
    if(lua_isnoneornil(L, arg)) ERR(ERR_NOTPRESENT);
    if(lua_type(L, arg)!=LUA_TTABLE) ERR(ERR_TABLE);

    count = luaL_len(L, arg);
    if(count==0) ERR(ERR_EMPTY);
    dst = MallocNoErr(L, count*sizeof(vec4_t));
    if(!dst) ERR(ERR_MEMORY);
    for(i=0; i<count; i++)
        {
        lua_rawgeti(L, arg, i+1);
        if(testvec4(L, -1, dst[i])!=0)
            { Free(L, dst); ERR(ERR_TYPE); }
        lua_pop(L, 1);
        }
#undef ERR
    *countp = count;
    if(err) *err=0;
    return dst;
    }

void pushvec4list(lua_State *L, const vec4_t *vecs, int count)
    {
    int i;
    lua_newtable(L);
    for(i=0; i<count; i++)
        {
        pushvec4(L, vecs[i]);
        lua_rawseti(L, -2, i+1);
        }
    }

/* quat_t ----------------------------------------------------------*/

int testquat(lua_State *L, int arg, quat_t dst)
    {
    int isnum;
    int t = lua_type(L, arg);
    switch(t)
        {
        case LUA_TNONE:
        case LUA_TNIL:  return ERR_NOTPRESENT;
        case LUA_TTABLE: break;
        default: return ERR_TABLE;
        }
#define POP if(!isnum) { lua_pop(L, 1); return ERR_VALUE; } lua_pop(L, 1);
    lua_rawgeti(L, arg, 1); dst[0] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, arg, 2); dst[1] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, arg, 3); dst[2] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, arg, 4); dst[3] = lua_tonumberx(L, -1, &isnum); POP
#undef POP
    return 0;
    }

int optquat(lua_State *L, int arg, quat_t dst)
    {
    int ec = testquat(L, arg, dst);
    if(ec<0) return argerror(L, arg, ec);
    return ec;
    }

int checkquat(lua_State *L, int arg, quat_t dst)
    {
    int ec = testquat(L, arg, dst);
    if(ec) return argerror(L, arg, ec);
    return ec;
    }

void pushquat(lua_State *L, const quat_t val)
    {
    if(GLMATH_COMPAT) lua_rawgeti(L, LUA_REGISTRYINDEX, toquat);
    lua_newtable(L);
    lua_pushnumber(L, val[0]); lua_rawseti(L, -2, 1);
    lua_pushnumber(L, val[1]); lua_rawseti(L, -2, 2);
    lua_pushnumber(L, val[2]); lua_rawseti(L, -2, 3);
    lua_pushnumber(L, val[3]); lua_rawseti(L, -2, 4);
    if(GLMATH_COMPAT && lua_pcall(L,1,1,0)!=LUA_OK) { unexpected(L); return; }
    }

quat_t *checkquatlist(lua_State *L, int arg, int *countp, int *err)
/* Check if the value at arg is a table of quats and returns the corresponding
 * array of quat_t, storing the size in *countp. The array is Malloc()'d and the
 * caller is in charge of Free()ing it.
 * If err=NULL, raises an error on failure, otherwise returns NULL and stores
 * the ERR_XXX code in *err.
 */
    {
    int count, i;
    quat_t *dst = NULL;
    *countp = 0;
#define ERR(ec) do { if(err) *err=(ec); else argerror(L, arg, (ec)); return NULL; } while(0)
    if(lua_isnoneornil(L, arg)) ERR(ERR_NOTPRESENT);
    if(lua_type(L, arg)!=LUA_TTABLE) ERR(ERR_TABLE);

    count = luaL_len(L, arg);
    if(count==0) ERR(ERR_EMPTY);
    dst = MallocNoErr(L, count*sizeof(quat_t));
    if(!dst) ERR(ERR_MEMORY);
    for(i=0; i<count; i++)
        {
        lua_rawgeti(L, arg, i+1);
        if(testquat(L, -1, dst[i])!=0)
            { Free(L, dst); ERR(ERR_TYPE); }
        lua_pop(L, 1);
        }
#undef ERR
    *countp = count;
    if(err) *err=0;
    return dst;
    }

void pushquatlist(lua_State *L, const quat_t *quats, int count)
    {
    int i;
    lua_newtable(L);
    for(i=0; i<count; i++)
        {
        pushquat(L, quats[i]);
        lua_rawseti(L, -2, i+1);
        }
    }

/* mat3_t ----------------------------------------------------------*/

int testmat3(lua_State *L, int arg, mat3_t dst)
    {
    int isnum;
    int t = lua_type(L, arg);
    switch(t)
        {
        case LUA_TNONE:
        case LUA_TNIL:  return ERR_NOTPRESENT;
        case LUA_TTABLE: break;
        default: return ERR_TABLE;
        }

    dst[3]=dst[7]=dst[11]=0; /* translation part */
#define POP if(!isnum) { lua_pop(L, 1); return ERR_VALUE; } lua_pop(L, 1);
    /* row 1 */
    lua_rawgeti(L, arg, 1); if(lua_type(L, -1)!=LUA_TTABLE) return argerror(L, arg, ERR_VALUE);
    lua_rawgeti(L, -1, 1); dst[0] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 2); dst[1] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 3); dst[2] = lua_tonumberx(L, -1, &isnum); POP
    lua_pop(L, 1);
    /* row 2 */
    lua_rawgeti(L, arg, 2); if(lua_type(L, -1)!=LUA_TTABLE) return argerror(L, arg, ERR_VALUE);
    lua_rawgeti(L, -1, 1); dst[4] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 2); dst[5] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 3); dst[6] = lua_tonumberx(L, -1, &isnum); POP
    lua_pop(L, 1);
    /* row 3 */
    lua_rawgeti(L, arg, 3); if(lua_type(L, -1)!=LUA_TTABLE) return argerror(L, arg, ERR_VALUE);
    lua_rawgeti(L, -1, 1); dst[8] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 2); dst[9] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 3); dst[10] = lua_tonumberx(L, -1, &isnum); POP
    lua_pop(L, 1);
#undef POP
    return 0;
    }

int optmat3(lua_State *L, int arg, mat3_t dst)
    {
    int ec = testmat3(L, arg, dst);
    if(ec<0) return argerror(L, arg, ec);
    return ec;
    }

int checkmat3(lua_State *L, int arg, mat3_t dst)
    {
    int ec = testmat3(L, arg, dst);
    if(ec) return argerror(L, arg, ec);
    return ec;
    }

void pushmat3(lua_State *L, const mat3_t val)
    {
    if(GLMATH_COMPAT) lua_rawgeti(L, LUA_REGISTRYINDEX, tomat3);
    lua_newtable(L);
    /* row 1 */
    lua_newtable(L);
    lua_pushnumber(L, val[0]); lua_rawseti(L, -2, 1);
    lua_pushnumber(L, val[1]); lua_rawseti(L, -2, 2);
    lua_pushnumber(L, val[2]); lua_rawseti(L, -2, 3);
    lua_rawseti(L, -2, 1);
    /* row 2 */
    lua_newtable(L);
    lua_pushnumber(L, val[4]); lua_rawseti(L, -2, 1);
    lua_pushnumber(L, val[5]); lua_rawseti(L, -2, 2);
    lua_pushnumber(L, val[6]); lua_rawseti(L, -2, 3);
    lua_rawseti(L, -2, 2);
    /* row 3 */
    lua_newtable(L);
    lua_pushnumber(L, val[8]); lua_rawseti(L, -2, 1);
    lua_pushnumber(L, val[9]); lua_rawseti(L, -2, 2);
    lua_pushnumber(L, val[10]); lua_rawseti(L, -2, 3);
    lua_rawseti(L, -2, 3);
    if(GLMATH_COMPAT && lua_pcall(L,1,1,0)!=LUA_OK) { unexpected(L); return; }
    }

/* mat4_t ----------------------------------------------------------*/

int testmat4(lua_State *L, int arg, mat4_t dst)
    {
    int isnum;
    int t = lua_type(L, arg);
    switch(t)
        {
        case LUA_TNONE:
        case LUA_TNIL:  return ERR_NOTPRESENT;
        case LUA_TTABLE: break;
        default: return ERR_TABLE;
        }

#define POP if(!isnum) { lua_pop(L, 1); return ERR_VALUE; } lua_pop(L, 1);
    /* row 1 */
    lua_rawgeti(L, arg, 1); if(lua_type(L, -1)!=LUA_TTABLE) return argerror(L, arg, ERR_VALUE);
    lua_rawgeti(L, -1, 1); dst[0] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 2); dst[1] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 3); dst[2] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 4); dst[3] = lua_tonumberx(L, -1, &isnum); POP
    lua_pop(L, 1);
    /* row 2 */
    lua_rawgeti(L, arg, 2); if(lua_type(L, -1)!=LUA_TTABLE) return argerror(L, arg, ERR_VALUE);
    lua_rawgeti(L, -1, 1); dst[4] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 2); dst[5] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 3); dst[6] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 4); dst[7] = lua_tonumberx(L, -1, &isnum); POP
    lua_pop(L, 1);
    /* row 3 */
    lua_rawgeti(L, arg, 3); if(lua_type(L, -1)!=LUA_TTABLE) return argerror(L, arg, ERR_VALUE);
    lua_rawgeti(L, -1, 1); dst[8] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 2); dst[9] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 3); dst[10] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 4); dst[11] = lua_tonumberx(L, -1, &isnum); POP
    lua_pop(L, 1);
    /* row 4 */
    lua_rawgeti(L, arg, 4); if(lua_type(L, -1)!=LUA_TTABLE) return argerror(L, arg, ERR_VALUE);
    lua_rawgeti(L, -1, 1); dst[12] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 2); dst[13] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 3); dst[14] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, -1, 4); dst[15] = lua_tonumberx(L, -1, &isnum); POP
    lua_pop(L, 1);
#undef POP
    return 0;
    }

int optmat4(lua_State *L, int arg, mat4_t dst)
    {
    int ec = testmat4(L, arg, dst);
    if(ec<0) return argerror(L, arg, ec);
    return ec;
    }

int checkmat4(lua_State *L, int arg, mat4_t dst)
    {
    int ec = testmat4(L, arg, dst);
    if(ec) return argerror(L, arg, ec);
    return ec;
    }

void pushmat4(lua_State *L, const mat4_t val)
    {
    if(GLMATH_COMPAT) lua_rawgeti(L, LUA_REGISTRYINDEX, tomat4);
    lua_newtable(L);
    /* row 1 */
    lua_newtable(L);
    lua_pushnumber(L, val[0]); lua_rawseti(L, -2, 1);
    lua_pushnumber(L, val[1]); lua_rawseti(L, -2, 2);
    lua_pushnumber(L, val[2]); lua_rawseti(L, -2, 3);
    lua_pushnumber(L, val[3]); lua_rawseti(L, -2, 4);
    lua_rawseti(L, -2, 1);
    /* row 2 */
    lua_newtable(L);
    lua_pushnumber(L, val[4]); lua_rawseti(L, -2, 1);
    lua_pushnumber(L, val[5]); lua_rawseti(L, -2, 2);
    lua_pushnumber(L, val[6]); lua_rawseti(L, -2, 3);
    lua_pushnumber(L, val[7]); lua_rawseti(L, -2, 4);
    lua_rawseti(L, -2, 2);
    /* row 3 */
    lua_newtable(L);
    lua_pushnumber(L, val[8]); lua_rawseti(L, -2, 1);
    lua_pushnumber(L, val[9]); lua_rawseti(L, -2, 2);
    lua_pushnumber(L, val[10]); lua_rawseti(L, -2, 3);
    lua_pushnumber(L, val[11]); lua_rawseti(L, -2, 4);
    lua_rawseti(L, -2, 3);
    /* row 4 */
    lua_newtable(L);
    lua_pushnumber(L, val[12]); lua_rawseti(L, -2, 1);
    lua_pushnumber(L, val[13]); lua_rawseti(L, -2, 2);
    lua_pushnumber(L, val[14]); lua_rawseti(L, -2, 3);
    lua_pushnumber(L, val[15]); lua_rawseti(L, -2, 4);
    lua_rawseti(L, -2, 4);
    if(GLMATH_COMPAT && lua_pcall(L,1,1,0)!=LUA_OK) { unexpected(L); return; }
    }


/* box3_t ----------------------------------------------------------*/

int testbox3(lua_State *L, int arg, box3_t dst)
    {
    int isnum;
    int t = lua_type(L, arg);
    switch(t)
        {
        case LUA_TNONE:
        case LUA_TNIL:  return ERR_NOTPRESENT;
        case LUA_TTABLE: break;
        default: return ERR_TABLE;
        }
#define POP if(!isnum) { lua_pop(L, 1); return ERR_VALUE; } lua_pop(L, 1);
    lua_rawgeti(L, arg, 1); dst[0] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, arg, 2); dst[1] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, arg, 3); dst[2] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, arg, 4); dst[3] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, arg, 5); dst[4] = lua_tonumberx(L, -1, &isnum); POP
    lua_rawgeti(L, arg, 6); dst[5] = lua_tonumberx(L, -1, &isnum); POP
#undef POP
    return 0;
    }

int optbox3(lua_State *L, int arg, box3_t dst)
    {
    int ec = testbox3(L, arg, dst);
    if(ec<0) return argerror(L, arg, ec);
    return ec;
    }

int checkbox3(lua_State *L, int arg, box3_t dst)
    {
    int ec = testbox3(L, arg, dst);
    if(ec) return argerror(L, arg, ec);
    return ec;
    }


void pushbox3(lua_State *L, const box3_t val)
    {
    if(GLMATH_COMPAT) lua_rawgeti(L, LUA_REGISTRYINDEX, tobox3);
    lua_newtable(L);
    lua_pushnumber(L, val[0]); lua_rawseti(L, -2, 1);
    lua_pushnumber(L, val[1]); lua_rawseti(L, -2, 2);
    lua_pushnumber(L, val[2]); lua_rawseti(L, -2, 3);
    lua_pushnumber(L, val[3]); lua_rawseti(L, -2, 4);
    lua_pushnumber(L, val[4]); lua_rawseti(L, -2, 5);
    lua_pushnumber(L, val[5]); lua_rawseti(L, -2, 6);
    if(GLMATH_COMPAT && lua_pcall(L,1,1,0)!=LUA_OK) { unexpected(L); return; }
    }

/* polygons --------------------------------------------------------*/

unsigned int *checkpolygons(lua_State *L, int arg, unsigned int *countp, int *err)
/* Check if the value at arg is a table of polygons, and returns the corresponding
 * array of integers, storing the size in *countp. The array is Malloc()'d and the
 * caller is in charge of Free()ing it.
 * If err=NULL, raises an error on failure, otherwise returns NULL and stores
 * the ERR_XXX code in *err.
 * A polygon is represented in Lua as a table of 0-based integer indexes (denoting
 * the vertices that compose the polygon). This is encoded in C as a similar array of
 * unsigned integers, with the addition of the number of vertices as first element,
 * e.g.:   Lua:        p = {7, 13, 3, 6, 1} 
 *     ->  C:   uint p[] = {5, 7, 13, 3, 6, 1};
 */
    {
    int top, isnum;
    unsigned int npoly, count, n, i, j;
    unsigned int *dst = NULL;
    *countp = 0;
    top = lua_gettop(L);
#define ERR(ec) do { \
    if(err) *err=(ec); else argerror(L, arg, (ec)); \
    lua_settop(L, top); return NULL;                \
} while(0)
    if(lua_isnoneornil(L, arg)) ERR(ERR_NOTPRESENT);
    if(lua_type(L, arg)!=LUA_TTABLE) ERR(ERR_TABLE);

    npoly = luaL_len(L, arg);
    /* first pass to compute the length of the destination array */
    count = 0;
    for(i=0; i<npoly; i++)
        {
        lua_rawgeti(L, arg, i+1);
        if(lua_type(L, -1)!=LUA_TTABLE) ERR(ERR_TYPE);
        n = luaL_len(L, -1); /* no. of vertices in the polygon */
        if(n>0) // skip empty tables
            count = count + n + 1; // add 1 for the no. of vertices
        lua_pop(L, 1);
        }

    if(count==0) ERR(ERR_EMPTY);
    dst = MallocNoErr(L, count*sizeof(unsigned int));
    if(!dst) ERR(ERR_MEMORY);

    /* second pass to actually encode the destination array */
    count = 0;
    for(i=0; i<npoly; i++)
        {
        lua_rawgeti(L, arg, i+1);
        n = luaL_len(L, -1);
        if(n>0) // skip empty tables
            {
            dst[count++] = n;
            for(j=0; j<n; j++)
                {
                lua_rawgeti(L, -1, j+1);
                dst[count++] = lua_tointegerx(L, -1, &isnum);
                if(!isnum) { Free(L, dst); ERR(ERR_TYPE); }
                lua_pop(L, 1);
                }
            }
        lua_pop(L, 1);
        }
#undef ERR
    lua_settop(L, top);
    *countp = count;
    if(err) *err=0;
    return dst;
    }

/* mass_t ----------------------------------------------------------*/

int checkmass(lua_State *L, int arg, mass_t *dst)
    {
    int t = lua_type(L, arg);
    switch(t)
        {
        case LUA_TNONE:
        case LUA_TNIL:  return argerror(L, arg, ERR_NOTPRESENT);
        case LUA_TTABLE: break;
        default: return argerror(L, arg, ERR_TABLE);
        }
    lua_getfield(L, arg, "total"); dst->mass = luaL_checknumber(L, -1); lua_pop(L, 1);
    lua_getfield(L, arg, "center");  checkvec3(L, -1, dst->c); lua_pop(L, 1);
    lua_getfield(L, arg, "inertia");  checkmat3(L, -1, dst->I); lua_pop(L, 1);
    return 0;
    }

void pushmass(lua_State *L, mass_t *val)
    {
    lua_newtable(L);
    lua_pushnumber(L, val->mass); lua_setfield(L, -2, "total");
    pushvec3(L, val->c); lua_setfield(L, -2, "center");
    pushmat3(L, val->I); lua_setfield(L, -2, "inertia");
    setmetatable(L, MASS_MT);
    }

/* contact_point_t --------------------------------------------------*/

int checkcontactpoint(lua_State *L, int arg, contact_point_t *dst)
    {
    int t = lua_type(L, arg);
    switch(t)
        {
        case LUA_TNONE:
        case LUA_TNIL:  return argerror(L, arg, ERR_NOTPRESENT);
        case LUA_TTABLE: break;
        default: return argerror(L, arg, ERR_TABLE);
        }
    (void)dst;
    lua_getfield(L, arg, "position"); checkvec3(L, -1, dst->pos); lua_pop(L, 1);
    lua_getfield(L, arg, "normal"); checkvec3(L, -1, dst->normal); lua_pop(L, 1);
    lua_getfield(L, arg, "depth"); dst->depth = luaL_checknumber(L, -1); lua_pop(L, 1);
    lua_getfield(L, arg, "geom1");  dst->g1 = checkgeom(L, lua_gettop(L), NULL); lua_pop(L, 1);
    lua_getfield(L, arg, "geom2");  dst->g2 = checkgeom(L, lua_gettop(L), NULL); lua_pop(L, 1);
    lua_getfield(L, arg, "side1"); dst->side1 = luaL_checkinteger(L, -1); lua_pop(L, 1);
    lua_getfield(L, arg, "side2"); dst->side2 = luaL_checkinteger(L, -1); lua_pop(L, 1);
    return 0;
    }

void pushcontactpoint(lua_State *L, contact_point_t *val)
    {
    lua_newtable(L);
    pushvec3(L, val->pos); lua_setfield(L, -2, "position");
    pushvec3(L, val->normal); lua_setfield(L, -2, "normal");
    lua_pushnumber(L, val->depth); lua_setfield(L, -2, "depth");
    pushxxx(L, val->g1); lua_setfield(L, -2, "geom1");
    pushxxx(L, val->g2); lua_setfield(L, -2, "geom2");
    lua_pushinteger(L, val->side1); lua_setfield(L, -2, "side1");
    lua_pushinteger(L, val->side2); lua_setfield(L, -2, "side2");
    }

/* surface_parameters_t --------------------------------------------*/

int checksurfaceparameters(lua_State *L, int arg, surface_parameters_t *dst)
    {
    int t = lua_type(L, arg);
    int isnum;
    int mode = 0;
    switch(t)
        {
        case LUA_TNONE:
        case LUA_TNIL:  return argerror(L, arg, ERR_NOTPRESENT);
        case LUA_TTABLE: break;
        default: return argerror(L, arg, ERR_TABLE);
        }
    lua_getfield(L, arg, "mu"); dst->mu = luaL_checknumber(L, -1); lua_pop(L, 1); // mandatory
#define GET(field, name, flag) do {                                                         \
    lua_getfield(L, arg, name); dst->field = lua_tonumberx(L, -1, &isnum); lua_pop(L, 1);   \
    if(isnum) mode |= flag;                                                                 \
} while(0)
    GET(mu2, "mu2", dContactMu2); // same as dContactAxisDep
    GET(rho, "rho", dContactRolling);
    GET(rho2, "rho2", dContactRolling);
    GET(rhoN, "rhoN", dContactRolling);
    GET(bounce, "bounce", dContactBounce);
    GET(bounce_vel, "bounce_vel", dContactBounce);
    GET(soft_erp, "soft_erp", dContactSoftERP);
    GET(soft_cfm, "soft_cfm", dContactSoftCFM);
    GET(motion1, "motion1", dContactMotion1);
    GET(motion2, "motion2", dContactMotion2);
    GET(motionN, "motionN", dContactMotionN);
    GET(slip1, "slip1", dContactSlip1);
    GET(slip2, "slip2", dContactSlip2);
#undef GET
    /* Additional dContact flags, represented here as optional boolean fields */
#define GET(name, flag) do {                \
    lua_getfield(L, arg, name);             \
    if(lua_toboolean(L, -1)) mode |= flag;  \
    lua_pop(L, 1);                          \
} while(0)
//  GET("fdir1", dContactFDir1); set automatically if fdir1 is passed to create_joint_contact
    GET("approx1", dContactApprox1); // equivalent to approx1_1|approx1_2|approx1_N
    GET("approx1_1", dContactApprox1_1);
    GET("approx1_2", dContactApprox1_2);
    GET("approx1_N", dContactApprox1_N);
#undef GET
    dst->mode = mode;
    return 0;
    }

