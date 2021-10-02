#!/usr/bin/env lua
-- MoonODE example: mass.lua
local ode = require("moonode")
local glmath = require("moonglmath")
ode.glmath_compat(true)
local vec3, mat3 = glmath.vec3, glmath.mat3

-- add metamethods for printing masses:
local mt = getmetatable(ode.mass())
mt.__tostring = function(m)
   return "total="..m.total.." center="..m.center.." inertia="..m.inertia..
         (m:check() and " (valid)" or " (not valid)")
end
mt.__concat = function(a, b) return tostring(a)..tostring(b) end

local m
m = ode.mass() print(m)
m = ode.mass(78.5) print(m)

m = ode.mass(80.5, {1, 1, 1}, {{1, 0, 0},{0, 1, 0}, {0, 0, 1}}) print(m)   --> not valid
m = ode.mass(80.5, {0, 0, 0}, {{1, 0, 0},{0, 1, 0}, {0, 0, 1}})  
m:translate({1,1,1}) print(m)                                              --> valid

m = m:translate({1, 1, 1}) print(m)                            --> valid
m = ode.mass(80.5, {0, 0, 0}, {{1, 0, 0},{0, 1, 0}, {0, 0, 1}}) print(m)
m = ode.mass_sphere(1, 10) print(m)
m = ode.mass(123.4, vec3(), mat3()) print(m) 
m = ode.mass(10) print(m)
m = m:add(ode.mass(20)) print(m)

m = ode.mass_box(1, .4, .4, .4) print(m)
m = m:adjust(1) print(m)
m = ode.mass_box(1, .4, .4, .4, 1) print(m)
