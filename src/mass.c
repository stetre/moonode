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
    
static int Mass(lua_State *L)
    {
    mass_t m;
    dMassSetZero(&m);
    m.I[0] = m.I[5] = m.I[10] = 1.0f;
    m.mass = luaL_optnumber(L, 1, 0);
    if(!lua_isnoneornil(L, 2)) checkvec3(L, 2, m.c);
    if(!lua_isnoneornil(L, 3)) checkmat3(L, 3, m.I);
    pushmass(L, &m);
    return 1;
    }

static int MassCheck(lua_State *L)
    {
    mass_t m;
    checkmass(L, 1, &m);
    lua_pushboolean(L, dMassCheck(&m));
    return 1;
    }

static int MassSphere(lua_State *L)
    {
    mass_t m;
    double density = luaL_checknumber(L, 1);
    double radius = luaL_checknumber(L, 2);
    int total = lua_toboolean(L, 3);
    if(total)
        dMassSetSphereTotal(&m, density, radius);
    else
        dMassSetSphere(&m, density, radius);
    pushmass(L, &m);
    return 1;
    }

static int MassCapsule(lua_State *L)
    {
    mass_t m;
    double density = luaL_checknumber(L, 1);
    int direction = checkdirection(L, 2);
    double radius = luaL_checknumber(L, 3);
    double length = luaL_checknumber(L, 4);
    int total = lua_toboolean(L, 5);
    if(total)
        dMassSetCapsule(&m, density, direction, radius, length);
    else
        dMassSetCapsule(&m, density, direction, radius, length);
    pushmass(L, &m);
    return 1;
    }

static int MassCylinder(lua_State *L)
    {
    mass_t m;
    double density = luaL_checknumber(L, 1);
    int direction = checkdirection(L, 2);
    double radius = luaL_checknumber(L, 3);
    double length = luaL_checknumber(L, 4);
    int total = lua_toboolean(L, 5);
    if(total)
        dMassSetCylinder(&m, density, direction, radius, length);
    else
        dMassSetCylinder(&m, density, direction, radius, length);
    pushmass(L, &m);
    return 1;
    }

static int MassBox(lua_State *L)
    {
    mass_t m;
    double density = luaL_checknumber(L, 1);
    double lx = luaL_checknumber(L, 2);
    double ly = luaL_checknumber(L, 3);
    double lz = luaL_checknumber(L, 4);
    int total = lua_toboolean(L, 5);
    if(total)
        dMassSetBoxTotal(&m, density, lx, ly, lz);
    else
        dMassSetBox(&m, density, lx, ly, lz);
    pushmass(L, &m);
    return 1;
    }

static int MassTrimesh(lua_State *L)
    {
    mass_t m;
    double density = luaL_checknumber(L, 1);
    int total = lua_toboolean(L, 3);
    geom_t geom = checkgeom(L, 2, NULL);
    if(total)
        dMassSetTrimeshTotal(&m, density, geom);
    else
        dMassSetTrimesh(&m, density, geom);
    pushmass(L, &m);
    return 1;
    }

static int MassAdjust(lua_State *L)
    {
    mass_t m;
    double newmass;
    checkmass(L, 1, &m);
    newmass = luaL_checknumber(L, 2);
    dMassAdjust(&m, newmass);
    pushmass(L, &m);
    return 1;
    }

static int MassTranslate(lua_State *L)
    {
    mass_t m;
    vec3_t v;
    checkmass(L, 1, &m);
    checkvec3(L, 2, v);
    dMassTranslate(&m, v[0], v[1], v[2]);
    pushmass(L, &m);
    return 1;
    }

static int MassRotate(lua_State *L)
    {
    mass_t m;
    mat3_t rot;
    checkmass(L, 1, &m);
    checkmat3(L, 2, rot);
    dMassRotate(&m, rot);
    pushmass(L, &m);
    return 1;
    }

static int MassAdd(lua_State *L)
    {
    mass_t a, b;
    checkmass(L, 1, &a);
    checkmass(L, 2, &b);
    dMassAdd(&a, &b);
    pushmass(L, &a);
    return 1;
    }

static const struct luaL_Reg Methods[] = 
    {
        { "check", MassCheck },
        { "adjust", MassAdjust },
        { "translate", MassTranslate },
        { "rotate", MassRotate },
        { "add", MassAdd },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg MetaMethods[] = 
    {
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "mass", Mass },
        { "mass_check", MassCheck },
        { "mass_sphere", MassSphere },
        { "mass_capsule", MassCapsule },
        { "mass_cylinder", MassCylinder },
        { "mass_box", MassBox },
        { "mass_trimesh", MassTrimesh },
        { "mass_adjust", MassAdjust },
        { "mass_translate", MassTranslate },
        { "mass_rotate", MassRotate },
        { "mass_add", MassAdd },
        { NULL, NULL } /* sentinel */
    };

void moonode_open_mass(lua_State *L)
    {
    udata_define(L, MASS_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

