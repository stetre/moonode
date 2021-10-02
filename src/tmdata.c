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

typedef struct {
    void *positions;
    void *indices;
} info_t;

static int freetmdata(lua_State *L, ud_t *ud)
    {
    tmdata_t tmdata = (tmdata_t)ud->handle;
    info_t *info = (info_t*)ud->info;
    ud->info = NULL; // to prevent its automatic deletion
    if(!freeuserdata(L, ud, "tmdata")) return 0;
    if(info)
        {
        Free(L, info->positions);
        Free(L, info->indices);
        Free(L, info);
        }
    dGeomTriMeshDataDestroy(tmdata);
    return 0;
    }

static int Create(lua_State *L)
    {
    int single=0;
    int pstride, istride;
    int pcount, icount;
    size_t plen, ilen;
    ud_t *ud;
    info_t *info;
    int vertextype = checkdatatype(L, 1);
    const char* positions = luaL_checklstring(L, 2, &plen);
    const char* indices = luaL_checklstring(L, 3, &ilen);
    tmdata_t tmdata = dGeomTriMeshDataCreate();

    switch(vertextype)
        {
        case DATATYPE_FLOAT: single=1; pstride = 3*sizeof(float); break;
        case DATATYPE_DOUBLE: single=0; pstride = 3*sizeof(double); break;
        default:
            return argerror(L, 2, ERR_VALUE);
        }
    istride = 3*sizeof(uint32_t);

    if(plen==0 || plen % pstride != 0)
        return argerror(L, 3, ERR_LENGTH);
    pcount = plen/pstride;
    if(ilen==0 || ilen % istride != 0)
        return argerror(L, 3, ERR_LENGTH);
    icount = ilen/sizeof(uint32_t);

    info = Malloc(L, sizeof(info_t));
    info->positions = Strndup(L, positions, plen);
    info->indices = Strndup(L, indices, ilen);
    if(single)
        dGeomTriMeshDataBuildSingle(tmdata, info->positions, pstride, pcount,
                info->indices, icount, istride);
    else
        dGeomTriMeshDataBuildDouble(tmdata, info->positions, pstride, pcount,
                info->indices, icount, istride);
    ud = newuserdata(L, tmdata, TMDATA_MT, "tmdata");
    ud->parent_ud = NULL;
    ud->info = info;
    ud->destructor = freetmdata;
    return 1;
    }

static int Preprocess(lua_State *L)
    {
    unsigned int flags = 0;
    tmdata_t tmdata = checktmdata(L, 1, NULL);
    int build_concave_edges = optboolean(L, 2, 0);
    int build_face_angles = optboolean(L, 3, 0);
    if(build_concave_edges) flags |= (1U<<dTRIDATAPREPROCESS_BUILD_CONCAVE_EDGES);
    if(build_face_angles) flags |= (1U<<dTRIDATAPREPROCESS_BUILD_FACE_ANGLES);
    int rc = dGeomTriMeshDataPreprocess2(tmdata, flags, NULL);
    if(!rc) return failure(L, ERR_OPERATION);
    return 0;
    }

static int Update(lua_State *L)
    {
    tmdata_t tmdata = checktmdata(L, 1, NULL);
    dGeomTriMeshDataUpdate(tmdata);
    return 0;
    }

RAW_FUNC(tmdata)
DESTROY_FUNC(tmdata)

static const struct luaL_Reg Methods[] = 
    {
        { "raw", Raw },
        { "preprocess", Preprocess },
        { "update", Update },
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
        { "create_tmdata", Create },
        { NULL, NULL } /* sentinel */
    };

void moonode_open_tmdata(lua_State *L) /* Trimesh data */
    {
    if(sizeof(dTriIndex) != sizeof(uint32_t))
        luaL_error(L, "Invalid typedef for dTriIndex (MoonODE supports only 32 bit indices)");
    udata_define(L, TMDATA_MT, Methods, MetaMethods);
    luaL_setfuncs(L, Functions, 0);
    }

#if 0
// Do we need these?
enum { dTRIMESHDATA_FACE_NORMALS, dTRIMESHDATA_USE_FLAGS, }; /* values for data_id */
// void dGeomTriMeshDataSet(tmdata_t g, int data_id, void *in_data);
// void *dGeomTriMeshDataGet(tmdata_t g, int data_id);
// void *dGeomTriMeshDataGet2(tmdata_t g, int data_id, dsizeint *pout_size/*=NULL*/);
/* Set and get the TriMeshData additional data (Note: The data is NOT COPIED on assignment)
 */
#endif
