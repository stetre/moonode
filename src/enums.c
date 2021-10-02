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

/*------------------------------------------------------------------------------*
 | Code<->string map for enumerations                                           |
 *------------------------------------------------------------------------------*/

/* code <-> string record */
#define rec_t struct rec_s
struct rec_s {
    RB_ENTRY(rec_s) CodeEntry;
    RB_ENTRY(rec_s) StringEntry;
    int domain;
    int code;  /* (domain, code) = search key in code tree */
    char     *str;  /* (domain, str) = search key in string tree */
};

/* compare functions */
static int cmp_code(rec_t *rec1, rec_t *rec2) 
    { 
    if(rec1->domain != rec2->domain)
        return (rec1->domain < rec2->domain ? -1 : rec1->domain > rec2->domain);
    return (rec1->code < rec2->code ? -1 : rec1->code > rec2->code);
    } 

static int cmp_str(rec_t *rec1, rec_t *rec2) 
    { 
    if(rec1->domain != rec2->domain)
        return (rec1->domain < rec2->domain ? -1 : rec1->domain > rec2->domain);
    return strcmp(rec1->str, rec2->str);
    } 

static RB_HEAD(CodeTree, rec_s) CodeHead = RB_INITIALIZER(&CodeHead);
RB_PROTOTYPE_STATIC(CodeTree, rec_s, CodeEntry, cmp_code) 
RB_GENERATE_STATIC(CodeTree, rec_s, CodeEntry, cmp_code) 

static RB_HEAD(StringTree, rec_s) StringHead = RB_INITIALIZER(&StringHead);
RB_PROTOTYPE_STATIC(StringTree, rec_s, StringEntry, cmp_str) 
RB_GENERATE_STATIC(StringTree, rec_s, StringEntry, cmp_str) 
 
static rec_t *code_remove(rec_t *rec) 
    { return RB_REMOVE(CodeTree, &CodeHead, rec); }
static rec_t *code_insert(rec_t *rec) 
    { return RB_INSERT(CodeTree, &CodeHead, rec); }
static rec_t *code_search(int domain, int code) 
    { rec_t tmp; tmp.domain = domain; tmp.code = code; return RB_FIND(CodeTree, &CodeHead, &tmp); }
static rec_t *code_first(int domain, int code) 
    { rec_t tmp; tmp.domain = domain; tmp.code = code; return RB_NFIND(CodeTree, &CodeHead, &tmp); }
static rec_t *code_next(rec_t *rec)
    { return RB_NEXT(CodeTree, &CodeHead, rec); }
#if 0
static rec_t *code_prev(rec_t *rec)
    { return RB_PREV(CodeTree, &CodeHead, rec); }
static rec_t *code_min(void)
    { return RB_MIN(CodeTree, &CodeHead); }
static rec_t *code_max(void)
    { return RB_MAX(CodeTree, &CodeHead); }
static rec_t *code_root(void)
    { return RB_ROOT(&CodeHead); }
#endif
 
static rec_t *str_remove(rec_t *rec) 
    { return RB_REMOVE(StringTree, &StringHead, rec); }
static rec_t *str_insert(rec_t *rec) 
    { return RB_INSERT(StringTree, &StringHead, rec); }
static rec_t *str_search(int domain, const char* str) 
    { rec_t tmp; tmp.domain = domain; tmp.str = (char*)str; return RB_FIND(StringTree, &StringHead, &tmp); }
#if 0
static rec_t *str_first(int domain, const char* str ) 
    { rec_t tmp; tmp.domain = domain; tmp.str = str; return RB_NFIND(StringTree, &StringHead, &tmp); }
static rec_t *str_next(rec_t *rec)
    { return RB_NEXT(StringTree, &StringHead, rec); }
static rec_t *str_prev(rec_t *rec)
    { return RB_PREV(StringTree, &StringHead, rec); }
static rec_t *str_min(void)
    { return RB_MIN(StringTree, &StringHead); }
static rec_t *str_max(void)
    { return RB_MAX(StringTree, &StringHead); }
static rec_t *str_root(void)
    { return RB_ROOT(&StringHead); }
#endif


static int enums_new(lua_State *L, int domain, int code, const char *str)
    {
    rec_t *rec;
    if((rec = (rec_t*)Malloc(L, sizeof(rec_t))) == NULL) 
        return luaL_error(L, errstring(ERR_MEMORY));

    memset(rec, 0, sizeof(rec_t));
    rec->domain = domain;
    rec->code = code;
    rec->str = Strdup(L, str);
    if(code_search(domain, code) || str_search(domain, str))
        { 
        Free(L, rec->str);
        Free(L, rec); 
        return unexpected(L); /* duplicate value */
        }
    code_insert(rec);
    str_insert(rec);
    return 0;
    }

static void enums_free(lua_State *L, rec_t* rec)
    {
    if(code_search(rec->domain, rec->code) == rec)
        code_remove(rec);
    if(str_search(rec->domain, rec->str) == rec)
        str_remove(rec);
    Free(L, rec->str);
    Free(L, rec);   
    }

void enums_free_all(lua_State *L)
    {
    rec_t *rec;
    while((rec = code_first(0, 0)))
        enums_free(L, rec);
    }

#if 0
int enums_code(int domain, const char *str, int* found)
    {
    rec_t *rec = str_search(domain, str);
    if(!rec)
        { *found = 0; return 0; }
    *found = 1; 
    return rec->code;
    }

const char* enums_string(int domain, int code)
    {
    rec_t *rec = code_search(domain, code);
    if(!rec)
        return NULL;
    return rec->str;
    }

#endif

int enums_test(lua_State *L, int domain, int arg, int *err)
    {
    rec_t *rec;
    const char *s = luaL_optstring(L, arg, NULL);
    if(!s) { *err = ERR_NOTPRESENT; return 0; }
    rec = str_search(domain, s);
    if(!rec) { *err = ERR_VALUE; return 0; }
    *err = ERR_SUCCESS;
    return rec->code;
    }

int enums_opt(lua_State *L, int domain, int arg, int defval)
    {
    rec_t *rec;
    const char *s = luaL_optstring(L, arg, NULL);
    if(!s) { return defval; }
    rec = str_search(domain, s);
    if(!rec) return luaL_argerror(L, arg, badvalue(L, s));
    return rec->code;
    }

int enums_check(lua_State *L, int domain, int arg)
    {
    rec_t *rec;
    const char *s = luaL_checkstring(L, arg);
    rec = str_search(domain, s);
    if(!rec) return luaL_argerror(L, arg, badvalue(L, s));
    return rec->code;
    }

int enums_push(lua_State *L, int domain, int code)
    {
    rec_t *rec = code_search(domain, code);
    if(!rec) return unexpected(L);
    lua_pushstring(L, rec->str);
    return 1;
    }

int enums_values(lua_State *L, int domain)
    {
    int i;
    rec_t *rec;

    lua_newtable(L);
    i = 1;
    rec = code_first(domain, 0);
    while(rec)
        {
        if(rec->domain == domain)
            {
            lua_pushstring(L, rec->str);
            lua_rawseti(L, -2, i++);
            }
        rec = code_next(rec);
        }

    return 1;
    }


int* enums_checklist(lua_State *L, int domain, int arg, int *count, int *err)
    {
    int* list;
    int i;

    *count = 0;
    *err = 0;
    if(lua_isnoneornil(L, arg))
        { *err = ERR_NOTPRESENT; return NULL; }
    if(lua_type(L, arg) != LUA_TTABLE)
        { *err = ERR_TABLE; return NULL; }

    *count = luaL_len(L, arg);
    if(*count == 0)
        { *err = ERR_NOTPRESENT; return NULL; }

    list = (int*)MallocNoErr(L, sizeof(int) * (*count));
    if(!list)
        { *count = 0; *err = ERR_MEMORY; return NULL; }

    for(i=0; i<*count; i++)
        {
        lua_rawgeti(L, arg, i+1);
        list[i] = enums_test(L, domain, -1, err);
        lua_pop(L, 1);
        if(*err)
            { Free(L, list); *count = 0; return NULL; }
        }
    return list;
    }

void enums_freelist(lua_State *L, int *list)
    {
    if(!list) return;
    Free(L, list);
    }

/*------------------------------------------------------------------------------*
 |                                                                              |
 *------------------------------------------------------------------------------*/

static int Enum(lua_State *L)
/* { strings } = ode.enum('type') lists all the values for a given enum type */
    { 
    const char *s = luaL_checkstring(L, 1); 
#define CASE(xxx) if(strcmp(s, ""#xxx) == 0) return values##xxx(L)
    CASE(jointtype);
    CASE(param);
    CASE(amotormode);
    CASE(geomtype);
    CASE(axesorder);
    CASE(direction);
    CASE(datatype);
    CASE(relativeorientation);
    CASE(transmissionmode);
#undef CASE
    return 0;
    }

static const struct luaL_Reg Functions[] = 
    {
        { "enum", Enum },
        { NULL, NULL } /* sentinel */
    };


void moonode_open_enums(lua_State *L)
    {
    int domain;

    luaL_setfuncs(L, Functions, 0);

    /* Add all the code<->string mappings */
#define ADD(what, s) do { enums_new(L, domain, what, s); } while(0)
    domain = DOMAIN_JOINT_TYPE;
    ADD(dJointTypeNone, "none");
    ADD(dJointTypeBall, "ball");
    ADD(dJointTypeHinge, "hinge");
    ADD(dJointTypeSlider, "slider");
    ADD(dJointTypeContact, "contact");
    ADD(dJointTypeUniversal, "universal");
    ADD(dJointTypeHinge2, "hinge2");
    ADD(dJointTypeFixed, "fixed");
    ADD(dJointTypeNull, "null");
    ADD(dJointTypeAMotor, "amotor");
    ADD(dJointTypeLMotor, "lmotor");
    ADD(dJointTypePlane2D, "plane2d");
    ADD(dJointTypePR, "pr");
    ADD(dJointTypePU, "pu");
    ADD(dJointTypePiston, "piston");
    ADD(dJointTypeDBall, "dball");
    ADD(dJointTypeDHinge, "dhinge");
    ADD(dJointTypeTransmission, "transmission");

    domain = DOMAIN_PARAM;
    ADD(dParamLoStop, "lo stop");
    ADD(dParamHiStop, "hi stop");
    ADD(dParamVel, "vel");
    ADD(dParamLoVel, "lo vel");
    ADD(dParamHiVel, "hi vel");
    ADD(dParamFMax, "fmax");
    ADD(dParamFudgeFactor, "fudge factor");
    ADD(dParamBounce, "bounce");
    ADD(dParamCFM, "cfm");
    ADD(dParamStopERP, "stop erp");
    ADD(dParamStopCFM, "stop cfm");
    ADD(dParamSuspensionERP, "suspension erp");
    ADD(dParamSuspensionCFM, "suspension cfm");
    ADD(dParamERP, "erp");
//  ADD(dParamLoStop2, "lo stop 2");
//  ADD(dParamLoStop3, "lo stop 3");
//  ADD(dParamHiStop2, "hi stop 2");
//  ADD(dParamHiStop3, "hi stop 3");
//  ADD(dParamVel2, "vel 2");
//  ADD(dParamVel3, "vel 3");
//  ADD(dParamLoVel2, "lo vel 2");
//  ADD(dParamLoVel3, "lo vel 3");
//  ADD(dParamHiVel2, "hi vel 2");
//  ADD(dParamHiVel3, "hi vel 3");
//  ADD(dParamFMax2, "fmax 2");
//  ADD(dParamFMax3, "fmax 3");
//  ADD(dParamFudgeFactor2, "fudge factor 2");
//  ADD(dParamFudgeFactor3, "fudge factor 3");
//  ADD(dParamBounce2, "bounce 2");
//  ADD(dParamBounce3, "bounce 3");
//  ADD(dParamCFM2, "cfm 2");
//  ADD(dParamCFM3, "cfm 3");
//  ADD(dParamStopERP2, "stop erp 2");
//  ADD(dParamStopERP3, "stop erp 3");
//  ADD(dParamStopCFM2, "stop cfm 2");
//  ADD(dParamStopCFM3, "stop cfm 3");
//  ADD(dParamSuspensionERP2, "suspension erp 2");
//  ADD(dParamSuspensionERP3, "suspension erp 3");
//  ADD(dParamSuspensionCFM2, "suspension cfm 2");
//  ADD(dParamSuspensionCFM3, "suspension cfm 3");
//  ADD(dParamERP2, "erp 2");
//  ADD(dParamERP3, "erp 3");

    domain = DOMAIN_AMOTOR_MODE;
    ADD(dAMotorUser, "user");
    ADD(dAMotorEuler, "euler");

    domain = DOMAIN_GEOM_TYPE;
    ADD(dSphereClass, "sphere");
    ADD(dBoxClass, "box");
    ADD(dCapsuleClass, "capsule");
    ADD(dCylinderClass, "cylinder");
    ADD(dPlaneClass, "plane");
    ADD(dRayClass, "ray");
    ADD(dConvexClass, "convex");
    ADD(dGeomTransformClass, "geom transform");
    ADD(dTriMeshClass, "trimesh");
    ADD(dHeightfieldClass, "heightfield");
    ADD(dSimpleSpaceClass, "simple space");
    ADD(dHashSpaceClass, "hash space");
    ADD(dSweepAndPruneSpaceClass, "sap space");
    ADD(dQuadTreeSpaceClass, "quad tree space");
    ADD(dFirstUserClass, "user1");
    ADD(dFirstUserClass+1, "user2");
    ADD(dFirstUserClass+2, "user3");
    ADD(dFirstUserClass+3, "user4"); // dMaxUserClasses = 4

    domain = DOMAIN_AXES_ORDER;
    ADD(dSAP_AXES_XYZ, "xyz");
    ADD(dSAP_AXES_XZY, "xzy");
    ADD(dSAP_AXES_YXZ, "yxz");
    ADD(dSAP_AXES_YZX, "yzx");
    ADD(dSAP_AXES_ZXY, "zxy");
    ADD(dSAP_AXES_ZYX, "zyx");

    domain = DOMAIN_DIRECTION;
    ADD(1, "x");
    ADD(2, "y");
    ADD(3, "z");

    domain = DOMAIN_RELATIVE_ORIENTATION;
    ADD(RELATIVE_ORIENTATION_GLOBAL_FRAME, "global frame");
    ADD(RELATIVE_ORIENTATION_FIRST_BODY, "first body");
    ADD(RELATIVE_ORIENTATION_SECOND_BODY, "second body");

    domain = DOMAIN_DATATYPE;
    ADD(DATATYPE_CHAR, "char");
    ADD(DATATYPE_UCHAR, "uchar");
    ADD(DATATYPE_SHORT, "short");
    ADD(DATATYPE_USHORT, "ushort");
    ADD(DATATYPE_INT, "int");
    ADD(DATATYPE_UINT, "uint");
    ADD(DATATYPE_LONG, "long");
    ADD(DATATYPE_ULONG, "ulong");
    ADD(DATATYPE_FLOAT, "float");
    ADD(DATATYPE_DOUBLE, "double");

    domain = DOMAIN_TRANSMISSION_MODE;
    ADD(dTransmissionParallelAxes, "parallel axes");
    ADD(dTransmissionIntersectingAxes, "intersecting axes");
    ADD(dTransmissionChainDrive, "chain drive");


#undef ADD
    }

