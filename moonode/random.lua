-- The MIT License (MIT)
--
-- Copyright (c) 2021 Stefano Trettel
--
-- Software repository: MoonODE, https://github.com/stetre/moonode
--
-- Permission is hereby granted, free of charge, to any person obtaining a copy
-- of this software and associated documentation files (the "Software"), to deal
-- in the Software without restriction, including without limitation the rights
-- to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
-- copies of the Software, and to permit persons to whom the Software is
-- furnished to do so, subject to the following conditions:
--
-- The above copyright notice and this permission notice shall be included in all
-- copies or substantial portions of the Software.
--
-- THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
-- IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
-- FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
-- AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
-- LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
-- OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
-- SOFTWARE.
--

-- *********************************************************************
-- DO NOT require() THIS MODULE (it is loaded automatically by MoonODE)
-- *********************************************************************

------------------------------------------------------------------------
-- Random generation utilities, based on math.random()
------------------------------------------------------------------------
do
local ode = moonode -- require("moonode")

-- These convert plain tables to glmath types if glmath_compat is enabled:
local vec3, vec4, mat4 = ode.vec3, ode.vec4, ode.mat4 

local rand = math.random
local pi, sin, cos = math.pi, math.sin, math.cos
local sqrt = math.sqrt

local function randomf(a, b) -- in range [a, b)
	if not a then return rand() end
	return a + rand()*(b-a)
end

local function randomi(n, m) -- in range [n, m]
	if not n then return random(0, 1) end
	return random(n, m)
end

local function randrange(r) -- in range [-r, r)
	local r = r or 1.0
	return (rand()*2-1)*r
end

local function randomdir()
	local theta = rand()*2*pi -- random in [0, 2pi)
	local z = randrange(1)    -- random in [-1,1)
	local k = sqrt(1-z^2)
	return vec3({k*cos(theta), k*sin(theta), z}) -- unit vector
end

local function randomvec3(r)
	return vec3({randrange(r), randrange(r), randrange(r)})
end

local function randomvec4(r)
	return vec4({randrange(r), randrange(r), randrange(r), randrange(r)})
end

local function randommat3(r)
	return mat3({{randrange(r), randrange(r), randrange(r)},
					 {randrange(r), randrange(r), randrange(r)},
					 {randrange(r), randrange(r), randrange(r)}})
end

local function randomsign()
	return rand() > .5 and 1 or -1
end

local r_from_axis_and_angle = ode.r_from_axis_and_angle 
local pi2 = 2*math.pi
local function randomrot()
	return r_from_axis_and_angle(randomdir(), randomf(0, pi2))
end

local q_from_r = ode.q_from_r
local function randomquat()
	return q_from_r(randomrot())
end

ode.random = math.random
ode.randomf = randomf
ode.randomi = randomi
ode.randomdir = randomdir
ode.randomvec3 = randomvec3
ode.randomvec4 = randomvec4
ode.randommat3 = randommat3
ode.randomsign = randomsign
ode.randomrot = randomrot
ode.randomquat = randomquat

end
