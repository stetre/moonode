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

#ifndef enumsDEFINED
#define enumsDEFINED

/* enums.c */
#define enums_free_all moonode_enums_free_all
void enums_free_all(lua_State *L);
#define enums_test moonode_enums_test
int enums_test(lua_State *L, int domain, int arg, int *err);
#define enums_opt moonode_enums_opt
int enums_opt(lua_State *L, int domain, int arg, int defval);
#define enums_check moonode_enums_check
int enums_check(lua_State *L, int domain, int arg);
#define enums_push moonode_enums_push
int enums_push(lua_State *L, int domain, int code);
#define enums_values moonode_enums_values
int enums_values(lua_State *L, int domain);
#define enums_checklist moonode_enums_checklist
int* enums_checklist(lua_State *L, int domain, int arg, int *count, int *err);
#define enums_freelist moonode_enums_freelist
void enums_freelist(lua_State *L, int *list);


/* Enum domains */
#define DOMAIN_                         0
#define DOMAIN_JOINT_TYPE               1
#define DOMAIN_PARAM                    2
#define DOMAIN_AMOTOR_MODE              3
#define DOMAIN_GEOM_TYPE            4
#define DOMAIN_AXES_ORDER               5
#define DOMAIN_DIRECTION                6
#define DOMAIN_RELATIVE_ORIENTATION     7
#define DOMAIN_TRANSMISSION_MODE        8
#define DOMAIN_DATATYPE                 9

/* codes for datatype */
#define RELATIVE_ORIENTATION_GLOBAL_FRAME   0
#define RELATIVE_ORIENTATION_FIRST_BODY     1
#define RELATIVE_ORIENTATION_SECOND_BODY    2

/* codes for datatype */
#define DATATYPE_CHAR         1
#define DATATYPE_UCHAR        2
#define DATATYPE_SHORT        3
#define DATATYPE_USHORT       4
#define DATATYPE_INT          5
#define DATATYPE_UINT         6
#define DATATYPE_LONG         7
#define DATATYPE_ULONG        8
#define DATATYPE_FLOAT        9
#define DATATYPE_DOUBLE       10

#define testjointtype(L, arg, err) enums_test((L), DOMAIN_JOINT_TYPE, (arg), (err))
#define optjointtype(L, arg, defval) enums_opt((L), DOMAIN_JOINT_TYPE, (arg), (defval))
#define checkjointtype(L, arg) enums_check((L), DOMAIN_JOINT_TYPE, (arg))
#define pushjointtype(L, val) enums_push((L), DOMAIN_JOINT_TYPE, (int)(val))
#define valuesjointtype(L) enums_values((L), DOMAIN_JOINT_TYPE)

#define testparam(L, arg, err) enums_test((L), DOMAIN_PARAM, (arg), (err))
#define optparam(L, arg, defval) enums_opt((L), DOMAIN_PARAM, (arg), (defval))
#define checkparam(L, arg) enums_check((L), DOMAIN_PARAM, (arg))
#define pushparam(L, val) enums_push((L), DOMAIN_PARAM, (int)(val))
#define valuesparam(L) enums_values((L), DOMAIN_PARAM)

#define testamotormode(L, arg, err) enums_test((L), DOMAIN_AMOTOR_MODE, (arg), (err))
#define optamotormode(L, arg, defval) enums_opt((L), DOMAIN_AMOTOR_MODE, (arg), (defval))
#define checkamotormode(L, arg) enums_check((L), DOMAIN_AMOTOR_MODE, (arg))
#define pushamotormode(L, val) enums_push((L), DOMAIN_AMOTOR_MODE, (int)(val))
#define valuesamotormode(L) enums_values((L), DOMAIN_AMOTOR_MODE)

#define testgeomtype(L, arg, err) enums_test((L), DOMAIN_GEOM_TYPE, (arg), (err))
#define optgeomtype(L, arg, defval) enums_opt((L), DOMAIN_GEOM_TYPE, (arg), (defval))
#define checkgeomtype(L, arg) enums_check((L), DOMAIN_GEOM_TYPE, (arg))
#define pushgeomtype(L, val) enums_push((L), DOMAIN_GEOM_TYPE, (int)(val))
#define valuesgeomtype(L) enums_values((L), DOMAIN_GEOM_TYPE)

#define testaxesorder(L, arg, err) enums_test((L), DOMAIN_AXES_ORDER, (arg), (err))
#define optaxesorder(L, arg, defval) enums_opt((L), DOMAIN_AXES_ORDER, (arg), (defval))
#define checkaxesorder(L, arg) enums_check((L), DOMAIN_AXES_ORDER, (arg))
#define pushaxesorder(L, val) enums_push((L), DOMAIN_AXES_ORDER, (int)(val))
#define valuesaxesorder(L) enums_values((L), DOMAIN_AXES_ORDER)

#define testdirection(L, arg, err) enums_test((L), DOMAIN_DIRECTION, (arg), (err))
#define optdirection(L, arg, defval) enums_opt((L), DOMAIN_DIRECTION, (arg), (defval))
#define checkdirection(L, arg) enums_check((L), DOMAIN_DIRECTION, (arg))
#define pushdirection(L, val) enums_push((L), DOMAIN_DIRECTION, (int)(val))
#define valuesdirection(L) enums_values((L), DOMAIN_DIRECTION)

#define testrelativeorientation(L, arg, err) enums_test((L), DOMAIN_RELATIVE_ORIENTATION, (arg), (err))
#define optrelativeorientation(L, arg, defval) enums_opt((L), DOMAIN_RELATIVE_ORIENTATION, (arg), (defval))
#define checkrelativeorientation(L, arg) enums_check((L), DOMAIN_RELATIVE_ORIENTATION, (arg))
#define pushrelativeorientation(L, val) enums_push((L), DOMAIN_RELATIVE_ORIENTATION, (int)(val))
#define valuesrelativeorientation(L) enums_values((L), DOMAIN_RELATIVE_ORIENTATION)

#define testdatatype(L, arg, err) enums_test((L), DOMAIN_DATATYPE, (arg), (err))
#define optdatatype(L, arg, defval) enums_opt((L), DOMAIN_DATATYPE, (arg), (defval))
#define checkdatatype(L, arg) enums_check((L), DOMAIN_DATATYPE, (arg))
#define pushdatatype(L, val) enums_push((L), DOMAIN_DATATYPE, (int)(val))
#define valuesdatatype(L) enums_values((L), DOMAIN_DATATYPE)

#define testtransmissionmode(L, arg, err) enums_test((L), DOMAIN_TRANSMISSION_MODE, (arg), (err))
#define opttransmissionmode(L, arg, defval) enums_opt((L), DOMAIN_TRANSMISSION_MODE, (arg), (defval))
#define checktransmissionmode(L, arg) enums_check((L), DOMAIN_TRANSMISSION_MODE, (arg))
#define pushtransmissionmode(L, val) enums_push((L), DOMAIN_TRANSMISSION_MODE, (int)(val))
#define valuestransmissionmode(L) enums_values((L), DOMAIN_TRANSMISSION_MODE)

#if 0 /* scaffolding 7yy */
#define testxxx(L, arg, err) enums_test((L), DOMAIN_XXX, (arg), (err))
#define optxxx(L, arg, defval) enums_opt((L), DOMAIN_XXX, (arg), (defval))
#define checkxxx(L, arg) enums_check((L), DOMAIN_XXX, (arg))
#define pushxxx(L, val) enums_push((L), DOMAIN_XXX, (int)(val))
#define valuesxxx(L) enums_values((L), DOMAIN_XXX)
    CASE(xxx);

#endif

#endif /* enumsDEFINED */


