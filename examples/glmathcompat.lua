#!/usr/bin/env lua
-- MoonODE example: glmathcompat.lua

local glmath = require("moonglmath")
local ode = require("moonode")
ode.glmath_compat(true) 
-- Calling glmath_compat() makes moonode functions return vectors, matrices,
-- and quaternions in the corresponding moonglmath formats. These are convenient
-- because they have metamethods, operators, functions, synctatic sugar, and so
-- on. They are not used by default (plain tables are used instead) because one
-- may want to use other math libraries, or none at all. Note that this means that
-- moonglmath is not a dependency for moonode, even if I use it extensively in the
-- examples.

local world = ode.create_world()
local body = ode.create_body(world)
print("vec3", body:get_position())
print("mat3", body:get_rotation())
print("quat", body:get_quaternion())

