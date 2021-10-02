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

#define ADD(c) do { lua_pushinteger(L, d##c); lua_setfield(L, -2, #c); } while(0)

/* checkkkflags: accepts a list of strings starting from index=arg
 * pushxxxflags -> pushes a list of strings 
 */

/*----------------------------------------------------------------------*
 | 
 *----------------------------------------------------------------------*/

static int checkcollideflags(lua_State *L, int arg) 
    {
    const char *s;
    int flags = 0;
    
    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(CONTACTS_UNIMPORTANT, "contacts unimportant");
#undef CASE
        return luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushcollideflags(lua_State *L, int flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
        CASE(CONTACTS_UNIMPORTANT, "contacts unimportant");
#undef CASE

    return n;
    }

static int CollideFlags(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushcollideflags(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkcollideflags(L, 1));
    return 1;
    }

#define dCONTACTS_UNIMPORTANT CONTACTS_UNIMPORTANT

#define Add_CollideFlags(L) \
    ADD(CONTACTS_UNIMPORTANT);\


static int AddConstants(lua_State *L) /* ode.XXX constants for dXxx values */
    {
    Add_CollideFlags(L);
    return 0;
    }

static const struct luaL_Reg Functions[] = 
    {
        { "collideflags", CollideFlags },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_flags(lua_State *L)
    {
    AddConstants(L);
    luaL_setfuncs(L, Functions, 0);
    }


#if 0 // scaffolding

/*----------------------------------------------------------------------*
 | 
 *----------------------------------------------------------------------*/

static int checkzzz(lua_State *L, int arg) 
    {
    const char *s;
    int flags = 0;
    
    while(lua_isstring(L, arg))
        {
        s = lua_tostring(L, arg++);
#define CASE(CODE,str) if((strcmp(s, str)==0)) do { flags |= CODE; goto done; } while(0)
        CASE(ZZZ_, "");
#undef CASE
        return luaL_argerror(L, --arg, badvalue(L,s));
        done: ;
        }

    return flags;
    }

static int pushzzz(lua_State *L, int flags)
    {
    int n = 0;

#define CASE(CODE,str) do { if( flags & CODE) { lua_pushstring(L, str); n++; } } while(0)
    CASE(ZZZ_, "");
#undef CASE

    return n;
    }

static int Zzz(lua_State *L)
    {
    if(lua_type(L, 1) == LUA_TNUMBER)
        return pushzzz(L, luaL_checkinteger(L, 1));
    lua_pushinteger(L, checkzzz(L, 1));
    return 1;
    }

    Add_Zzz(L);
        { "zzz", Zzz },
#define Add_Zzz(L) \
    ADD(ZZZ_);\

#endif

