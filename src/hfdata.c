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

static int freehfdata(lua_State *L, ud_t *ud)
    {
    hfdata_t hfdata = (hfdata_t)ud->handle;
    if(!freeuserdata(L, ud, "hfdata")) return 0;
    dGeomHeightfieldDataDestroy(hfdata);
    return 0;
    }

static int newhfdata(lua_State *L, hfdata_t hfdata)
    {
    ud_t *ud;
    ud = newuserdata(L, hfdata, HFDATA_MT, "hfdata");
    ud->parent_ud = NULL;
    ud->destructor = freehfdata;
    return 1;
    }

static int Create(lua_State *L)
    {
    size_t len;
    int datatype = checkdatatype(L, 1);
    const char* data = luaL_checklstring(L, 2, &len);
    double width = luaL_checknumber(L, 3);
    double depth = luaL_checknumber(L, 4);
    int widthsamples = luaL_checkinteger(L, 5);
    int depthsamples = luaL_checkinteger(L, 6);
    double scale = luaL_checknumber(L, 7);
    double offset = luaL_checknumber(L, 8);
    double thickness = luaL_checknumber(L, 9);
    int wrap = checkboolean(L, 10);
    hfdata_t hfdata = dGeomHeightfieldDataCreate();
    switch(datatype)
        {
#define F(T, func)  do {                                                \
    if(len != widthsamples*depthsamples*sizeof(T))                      \
        {                                                               \
        dGeomHeightfieldDataDestroy(hfdata);                            \
        return argerror(L, 2, ERR_LENGTH);                              \
        }                                                               \
    func(hfdata, (const T*)data, 1/* bCopyHeightData */, width, depth,  \
        widthsamples, depthsamples, scale, offset, thickness, wrap);    \
} while(0)
        case DATATYPE_UCHAR: F(unsigned char, dGeomHeightfieldDataBuildByte); break;
        case DATATYPE_SHORT: F(short, dGeomHeightfieldDataBuildShort); break;
        case DATATYPE_FLOAT: F(float, dGeomHeightfieldDataBuildSingle); break;
        case DATATYPE_DOUBLE: F(double, dGeomHeightfieldDataBuildDouble); break;
        default: unexpected(L);
#undef F
        }
    return newhfdata(L, hfdata);
    }

static double GetHeightCallback(void* p_user_data, int x, int z)
    {
#define L moonode_L
    double result;
    hfdata_t hfdata = (hfdata_t)p_user_data;    
    ud_t *ud = userdata(hfdata);
    if(!ud) { unexpected(L); return 0; } 
    lua_rawgeti(L, LUA_REGISTRYINDEX, ud->ref1);
    pushhfdata(L, hfdata);
    lua_pushnumber(L, x);
    lua_pushnumber(L, z);
    if(lua_pcall(L, 3, 1, 0) != LUA_OK)
        { lua_error(L); return 0; }
    result = luaL_checknumber(L, -1);
    return result;
#undef L
    }

static int CreateWithCallback(lua_State *L)
    {
    ud_t *ud;
    hfdata_t hfdata;
    double width = luaL_checknumber(L, 2);
    double depth = luaL_checknumber(L, 3);
    int widthsamples = luaL_checkinteger(L, 4);
    int depthsamples = luaL_checkinteger(L, 5);
    double scale = luaL_checknumber(L, 6);
    double offset = luaL_checknumber(L, 7);
    double thickness = luaL_checknumber(L, 8);
    int wrap = checkboolean(L, 9);
    if(!lua_isfunction(L, 1)) return argerror(L, 1, ERR_FUNCTION);
    hfdata = dGeomHeightfieldDataCreate();
    newhfdata(L, hfdata);
    ud = userdata(hfdata);
    lua_pushvalue(L, 1);
    ud->ref1 = luaL_ref(L, LUA_REGISTRYINDEX);
    dGeomHeightfieldDataBuildCallback(hfdata, hfdata, GetHeightCallback, width, depth,
            widthsamples, depthsamples, scale, offset, thickness, wrap);
    return 1; /* pushed by newhfdata() */
    }

static int SetBounds(lua_State *L)
    {
    hfdata_t hfdata = checkhfdata(L, 1, NULL);
    double minheight = luaL_checknumber(L, 2);
    double maxheight = luaL_checknumber(L, 3);
    dGeomHeightfieldDataSetBounds(hfdata, minheight, maxheight);
    return 0;
    }

RAW_FUNC(hfdata)
DESTROY_FUNC(hfdata)

static const struct luaL_Reg Methods[] = 
    {
        { "raw", Raw },
        { "set_bounds", SetBounds },
        { "destroy", Destroy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg MetaMethods[] = 
    {
        { "__gc",  Destroy },
        { NULL, NULL } /* sentinel */
    };

static const struct luaL_Reg Functions[] = 
    {
        { "create_hfdata", Create },
        { "create_hfdata_with_callback", CreateWithCallback },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_hfdata(lua_State *L)
    {
    udata_define(L, HFDATA_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }


