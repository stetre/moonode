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

static int freespace(lua_State *L, ud_t *ud)
    {
    space_t space = (space_t)ud->handle;
    if(!freeuserdata(L, ud, "space_simple")) return 0;
    spacedestroy(L, space);
    return 0;
    }

static int newspace(lua_State *L, space_t space, space_t parentspace)
    {
    ud_t *ud;
    ud = newuserdata(L, space, SPACE_SIMPLE_MT, "space_simple");
    ud->parent_ud = NULL;
    ud->destructor = freespace;
    (void)parentspace;
    return 1;
    }

static int Create(lua_State *L)
    {
    space_t parentspace = optspace(L, 1, NULL);
    space_t space = dSimpleSpaceCreate(parentspace);
    return newspace(L, space, parentspace);
    }

DESTROY_FUNC(space)

static const struct luaL_Reg Methods[] = 
    {
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
        { "create_simple_space", Create },
        { NULL, NULL } /* sentinel */
    };

void moonode_open_space_simple(lua_State *L)
    {
    udata_define(L, SPACE_SIMPLE_MT, Methods, MetaMethods);
    udata_inherit(L, SPACE_SIMPLE_MT, SPACE_MT);
    luaL_setfuncs(L, Functions, 0);
    }

