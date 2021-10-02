#!/usr/bin/env lua
-- MoonODE space_collide() tests
-- Derived from ode/demo/demo_space.cpp
local ode = require("moonode")
local glmath = require("moonglmath")
ode.glmath_compat(true)
-- testing procedure:
-- create a bunch of random boxes
-- test for intersections directly, put results in n^2 array
-- get space to report collisions:
-- - all correct collisions reported
-- - no pair reported more than once
-- - no incorrect collisions reported

local randomf = ode.randomf
local now, since = ode.now, ode.since
local stats, dumpdata = glmath.stats, glmath.dumpdata
local function printf(...) io.write(string.format(...)) end

local N = 20 -- number of boxes to test
local seed = 37

local bounds = {}
local boxes = {}
local data = {} -- data associated with boxes (dGeomSetData)
local hits = {} -- number of collisions a box has
local good_matrix = {}


local function init_test(space)
   local scale = 0.5
   -- set random boxes
   math.randomseed(seed)
   for i=1, N do 
      local b = {}
      b[1] = randomf()
      b[2] = b[1] + randomf()*scale
      b[3] = randomf()
      b[4] = b[3] + randomf()*scale
      b[5] = 2*randomf()
      b[6] = b[5] + randomf()*scale
      local box = ode.create_box(space, b[2]-b[1], b[4]-b[3], b[6]-b[5])
      box:set_position({(b[2]+b[1])/2, (b[4]+b[3])/2, (b[6]+b[5])/2})
      if boxes[i] then boxes[i]:destroy() end
      boxes[i] = box
      bounds[i] = b
      data[box] = i
   end
   -- compute all intersections and put the results in "good_matrix"
   good_matrix, test_matrix, hits = {}, {}, {}
   for i = 1, N do
      hits[i] = 0
      good_matrix[i],test_matrix[i] = {}, {}
      for j = 1, N do good_matrix[i][j] = 0 end
   end
   for i= 1, N do
      for j= i+1, N do
         local b1 = bounds[i]
         local b2 = bounds[j]
         if not (b1[1] > b2[2] or b1[2] < b2[1] or
                 b1[3] > b2[4] or b1[4] < b2[3] or
                 b1[5] > b2[6] or b1[6] < b2[5]) then
            good_matrix[i][j] = true
            good_matrix[j][i] = true
            hits[i] = hits[i] + 1
            hits[j] = hits[j] + 1
         end
      end
   end
end

local function check_results()
   for i= 1, N do
      for j= i+1, N do
         if good_matrix[i][j] and not test_matrix[i][j] then
            -- printf("failed to report collision(%d,%d)(seed=%d)\n",i,j,seed)
         end
      end
   end
end

local function near_callback(o1, o2)
   local i, j = data[o1], data[o2] -- dGeomGetData
   if i==j then
      printf("collision(%d,%d) is between the same object\n", i, j)
   end
   if not good_matrix[i][j] or not good_matrix[j][i] then
      printf("collision(%d,%d) is incorrect\n", i, j)
   end
   if test_matrix[i][j] or test_matrix[j][i] then
      printf("collision(%d,%d) reported more than once\n", i, j)
   end
   test_matrix[i][j] = true
   test_matrix[j][i] = true
end

local space
if not arg[1] or arg[1] == '-simple' then
   space = ode.create_simple_space()
elseif arg[1] == '-hash' then
   space = ode.create_hash_space()
   space:set_levels(-10, 10)
elseif arg[1] == '-quadtree' then
   space = ode.create_quadtree_space(nil, {0, 0, 0}, {10, 0, 10}, 7)
elseif arg[1] == '-sap' then
   space = ode.create_sap_space(nil, 'xzy')
else
   error("invalid option '"..arg[1].."'")
end
ode.set_near_callback(near_callback)

local NTESTS = 10000
local tmeas = {}
collectgarbage()
collectgarbage('stop')
for i = 1, NTESTS do
   init_test(space)
   local t = now()
   ode.space_collide(space)
   t = since(t)
   tmeas[i] = t*1e6
   -- printf("space_collide() executed in about %.3f ms\n", t*1e3)
   check_results()
   seed = seed + 1
   -- draw ...
   if i%100==0 then
      printf("space_collide() calls: %d\n", i)
      collectgarbage()
   end
end

-- compute stats and dump measurements to file
local tmean, tvar, tmin, tmax = stats(tmeas)
printf("space_collide() execution time: mean=%.0f, var=%.0f, min=%.0f, max=%.0f [us]\n",
         tmean, tvar, tmin, tmax)
dumpdata(tmeas, 'data.log') -- gnuplot> plot "data.log" using 1:2 with lines
