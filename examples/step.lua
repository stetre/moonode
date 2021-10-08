#!/usr/bin/env lua
-- MoonODE example: step.lua
-- Derived from ode/demo/demo_step.cpp
-- Compares the execution times of world:step() and world:quick:step().
local ode = require("moonode")
local glmath = require("moonglmath")
ode.glmath_compat(true)
--ode.trace_objects(true)
local now, since = ode.now, ode.since
local function printf(...) io.write(string.format(...)) end
local random, randomf = ode.random, ode.randomf
local randomrot, randomquat = ode.randomrot, ode.randomquat
local randomvec3 = ode.randomvec3

local STEP = 0.005      -- integration step (seconds)
local NUMSTEPS = 10000  -- number of integration steps in test
local NUMBODIES = 100   -- number of bodies
local NUMJOINTS = 99    -- number of joints
local RADIUS = 0.1732

local world, body, joint

local function create_world()
   if world then world:destroy() end
   world = ode.create_world()
   body, joint = {}, {}
   -- create random bodies
   for i=1, NUMBODIES do
      -- create bodies at random position and orientation
      body[i] = ode.create_body(world)
      body[i]:set_position({randomf(-1,1), randomf(-1,1), randomf()*2*RADIUS})
      body[i]:set_quaternion(randomquat())
      -- set random velocity
      body[i]:set_linear_vel(randomvec3())
      body[i]:set_angular_vel(randomvec3())
      -- set random mass(random diagonal mass rotated by a random amount)
      local mass = ode.mass_box(1, randomf()+.1, randomf()+.1, randomf()+.1, randomf()+1) 
      mass = mass:rotate(randomrot())
      body[i]:set_mass(mass)
   end
   -- create ball-n-socket joints at random positions, linking random bodies
   --(but make sure not to link the same pair of bodies twice)
   local linked = {}
   for i=1, NUMJOINTS do
      repeat
         local b1 = random(1, NUMBODIES)
         local b2 = random(1, NUMBODIES)
      until b1 ~= b2 and not linked[tostring(b1).."-"..tostring(b2)]
      linked[tostring(b1).."-"..tostring(b2)] = true
      linked[tostring(b2).."-"..tostring(b1)] = true
      joint[i] = ode.create_ball_joint(world)
      joint[i]:attach(body[b1], body[b2])
      joint[i]:set_anchor1({randomf(-1,1), randomf(-1,1), randomf()*2*RADIUS})
   end
    -- move bodies a bit to get some joint error
   for i=1, NUMBODIES do
      local pos = body[i]:get_position()
      body[i]:set_position(pos+randomvec3(.1))
   end
end


local function run_test(stepfunc)
   create_world()
   local step 
   if stepfunc == "step" then step = world.step
   elseif stepfunc == "quick_step" then step = world.quick_step
   else error("invalid step function")
   end
   local tmeas = {}
   -- Simulation loop:
   for i = 1,NUMSTEPS do
      -- add random forces and torques to all bodies
      for i=1, NUMBODIES do
         body[i]:add_force(randomvec3(-5, 5))
         body[i]:add_torque(randomvec3(-5, 5))
      end
      local t = now()
      step(world, STEP)
      t = since(t)
      table.insert(tmeas, t*1e6)
   end
   local tmean, tvar, tmin, tmax = glmath.stats(tmeas)
   printf("%s() execution time: mean=%.0f, var=%.0f, min=%.0f, max=%.0f [us]\n",
          stepfunc, tmean, tvar, tmin, tmax)
end

-- Main
math.randomseed(os.time())
run_test("step")
run_test("quick_step")

