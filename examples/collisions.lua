#!/usr/bin/env lua
-- MoonODE collision tests.
-- Derived from ode/demo/demo_collisions.cpp
local ode = require("moonode")
local glmath = require("moonglmath")
ode.glmath_compat(true)

local TOL = 1e-8  -- tolerance used for numerical checks

local vec3, mat3, mat4 = glmath.vec3, glmath.mat3, glmath.mat4
local pi, cos, sin = math.pi, math.cos, math.sin
local abs, sqrt = math.abs, math.sqrt
local random, randomf, randomsign = ode.random, ode.randomf, ode.randomsign
local randomdir, randomvec3, randomrot = ode.randomdir, ode.randomvec3, ode.randomrot

local function printf(...) io.write(string.format(...)) end

local COUNTER = {} -- COUNTER[i] = no of failures for test #i
local function RESET() COUNTER = {} end
local function FAILED(i) COUNTER[i] = COUNTER[i] and COUNTER[i]+1 or 1 end

math.randomseed(os.time())

-------------------------------------------------------------------------------
-- Point depth tests
-------------------------------------------------------------------------------

local function test_sphere_point_depth()
   local space = ode.create_simple_space()
   local sphere = ode.create_sphere(nil, 1)
   space:add(sphere)
   -- make a random sphere
   local r = random()+.1
   local C = randomvec3(1.0)
   sphere:set_radius(r)
   sphere:set_position(C)
   sphere:set_rotation(randomrot())
   -- test that center point has depth r
   if abs(sphere:point_depth(C) - r) > TOL then FAILED(1) end
   -- test that point on surface has depth 0
   local P = C + r*randomdir()
   if abs(sphere:point_depth(P)) > TOL then FAILED(2) end
   -- test point at random depth
   local d = (randomf(-1, 1))*r
   local P = C + (r-d)*randomdir()
   if abs(sphere:point_depth(P) - d) > TOL then FAILED(3) end
end

local function test_box_point_depth()
   local space = ode.create_simple_space()
   local box = ode.create_box(nil, 1, 1, 1)
   space:add(box)
   -- make a random box
   local s = vec3(random()+.1, random()+.1, random()+.1)
   local C = randomvec3(1.0)
   box:set_lengths(s.x, s.y, s.z)
   box:set_position(C)
   box:set_rotation(randomrot())
   -- test that center point depth is half of smallest side
   local ss = math.min(s.x, s.y, s.z) -- smallest side
   if abs(box:point_depth(C) -ss/2) > TOL then FAILED(1) end
   -- test that points on surface have depth 0
   local P = vec3(randomf(-.5, .5)*s.x,
                  randomf(-.5, .5)*s.y, 
                  randomf(-.5, .5)*s.z)
   local i = random(1, 3)
   P[i] = randomsign()*s[i]*0.5
   P = C + box:get_rotation()*P
   if abs(box:point_depth(P)) > TOL then FAILED(2) end
   -- test that points outside the box have negative depth
   local P = vec3(randomsign()*(s.x/2+random()+.01), 
                  randomsign()*(s.y/2+random()+.01), 
                  randomsign()*(s.z/2+random()+.01)) 
   P = C + box:get_rotation()*P
   if box:point_depth(P)>= 0 then FAILED(3) end
   -- test that points inside the box have positive depth
   local P = vec3(s.x*.99*randomf(-.5, .5),
                  s.y*.99*randomf(-.5, .5),
                  s.z*.99*randomf(-.5, .5))
   P = C + box:get_rotation()*P
   if box:point_depth(P) <= 0 then FAILED(4) end
   -- test depth of point aligned along axis (up to smallest-side-deep)
   local d = random()*(ss/2 + 1) - 1
   local P = vec3()
   local i = random(1, 3)
   P[i] = randomsign()*(s[i]/2 - d)
   P = C + box:get_rotation()*P
   if abs(box:point_depth(P) -d) >= TOL then FAILED(5) end
end


local function test_capsule_point_depth()
   local space = ode.create_simple_space()
   local capsule = ode.create_capsule(nil, 1, 1)
   space:add(capsule)
   -- make a random capsule
   local r = .5*random()+.01
   local l = random()+ .01
   local C = randomvec3(1.0)
   capsule:set(r, l)
   capsule:set_position(C)
   capsule:set_rotation(randomrot())
   -- test that point on axis has depth equal to radius
   local P = randomf(-.5, .5)*l*vec3(0, 0, 1)
   P = C + capsule:get_rotation()*P
   if abs(capsule:point_depth(P) - r) >= TOL then FAILED(1) end
   -- test that a point on the surface (excluding caps) has depth 0
   local beta = randomf(0, 2*pi)
   local P = vec3(r*sin(beta), r*cos(beta), randomf(-.5,.5)*l) 
   P = C + capsule:get_rotation()*P
   if abs(capsule:point_depth(P)) > TOL then FAILED(2) end
   -- test that a point on a surface of a cap has depth 0
   local dir = randomdir()
   local zofs = vec3(0, 0, l/2)
   if dir * vec3(0, 0, 1) < 0 then zofs = -zofs end
   local P = C + capsule:get_rotation()*(zofs + r*dir)
   if abs(capsule:point_depth(P)) > TOL then FAILED(3) end
   -- test that a point inside the capsule has positive depth
   local P = C + capsule:get_rotation()*(zofs + .99*r*dir)
   if capsule:point_depth(P) < 0 then FAILED(4) end
   -- test point depth of a point inside the cylinder
   local beta = randomf(0, 2*pi)
   local d = randomf(-1, 1)*r
   local P = vec3((r-d)*sin(beta), (r-d)*cos(beta), l*randomf(-.5, .5))
   P = C + capsule:get_rotation()*P
   if abs(capsule:point_depth(P) -d) >= TOL then FAILED(5) end
   -- test point depth of a point inside a cap
   local d = randomf(-1, 1)*r
   local dir = randomdir()
   local zofs = vec3(0, 0, l/2)
   if dir * vec3(0, 0, 1) < 0 then zofs = -zofs end
   local P = C + capsule:get_rotation()*(zofs + (r-d)*dir)
   if abs(capsule:point_depth(P) -d) > TOL then FAILED(6) end
end


local function test_plane_point_depth()
   local space = ode.create_simple_space()
   local plane = ode.create_plane(nil, 0, 0, 1, 0)
   space:add(plane)
   -- make a random plane
   local n = randomdir() -- plane normal  
   local d = randomf(-.5, .5)
   plane:set(n.x, n.y, n.z, d) --  plane equation: n·(x, y, z) = d
   local n, p, q = plane:basis()
   -- test that a point on the plane has depth 0
   local P = d*n + randomf()*p + randomf()*q
   if abs(plane:point_depth(P)) >= TOL then FAILED(1) end
   -- test the depth of a random point
   local x = randomf(-.5, 5)  
   local y = randomf(-.5, 5)  
   local z = randomf(-.5, 5)  
   local P = x*p + y*q + (d+z)*n 
   -- if z > 0 then depth is negative, if z < 0 then depth is positive
   -- |z| is the distance from the plane, so the depth is -z
   if abs(plane:point_depth(P) + z) >= TOL then FAILED(2) end
   -- test that the depth of a point with depth=1 is actually 1
   local x = randomf(-.5, 5)  
   local y = randomf(-.5, 5)  
   local P = x*p + y*q + (d-1)*n 
   if abs(plane:point_depth(P) -1) >= TOL then FAILED(3) end
end

-------------------------------------------------------------------------------
-- Ray tests
-------------------------------------------------------------------------------

local function test_ray_and_sphere()
   local space = ode.create_simple_space()
   local ray = ode.create_ray(nil, 0)
   local sphere = ode.create_sphere(nil, 1)
   space:add(ray)
   space:add(sphere)
   -- make random sphere
   local r = randomf()+.1
   local C = randomvec3(1.0)
   sphere:set_radius(r)
   sphere:set_position(C)
   sphere:set_rotation(randomrot())
   -- test zero length ray just inside sphere
   local P = C + .99*r*randomdir()
   ray:set_length(0)
   ray:set_position(P)
   randomrot()
   ray:set_rotation(randomrot())
   if ode.collide(ray, sphere, 1) then FAILED(1) end
   -- test zero length ray just outside sphere
   local P = C + 1.01*r*randomdir()
   ray:set_length(0)
   ray:set_position(P)
   ray:set_rotation(randomrot())
   if ode.collide(ray, sphere, 1)  then FAILED(2) end
   -- test a finite length ray totally inside the sphere
   local P = C + 0.99*r*randomdir()
   local Q = C + 0.99*r*randomdir()
   local dir = (Q-P):normalize()
   ray:set(P, dir)
   ray:set_length((Q-P):norm())
   if ode.collide(ray, sphere, 1) then FAILED(3) end
   -- test a finite length ray totally outside the sphere
   local P = C + 1.01*r*randomdir()
   local n = (P-C)
   local dir
   repeat dir = randomdir() until dir*n > 0 -- make sure it points away from the sphere
   ray:set(P, dir)
   ray:set_length(100)
   if ode.collide(ray, sphere, 1) then FAILED(4) end
   -- test ray from outside to just above surface
   local dir = randomdir()
   local P = C - 2*r*dir
   ray:set(P, dir)
   ray:set_length(.99*r)
   if ode.collide(ray, sphere, 1)  then FAILED(5) end
   -- test ray from outside to just below surface
   ray:set_length(1.001*r)
   if not ode.collide(ray, sphere, 2) then FAILED(6) end
   -- test contact point distance for random rays
   local P = C + randomf(0.5, 1.5)*r*randomdir()
   local dir = randomdir()
   ray:set(P, dir)
   ray:set_length(100)
   local collide, contacts = ode.collide(ray, sphere, 1) 
   if collide then
      local contact = contacts[1]
      -- check that the contact point is on the sphere
      local d = (contact.position - C):norm() 
      if abs(d-r) > TOL then FAILED(7) end
      -- check the sign of the normal
      if contact.normal * dir > 0 then FAILED(8) end
      -- check the depth of the contact point
      if abs(sphere:point_depth(contact.position)) > TOL then FAILED(9) end
   end
   -- test tangential grazing - miss
   local n = randomdir()
   local dir, _ = ode.plane_basis(n)
   local P = C + 1.0001*r*n - dir
   ray:set(P, dir)
   ray:set_length(2)
   if ode.collide(ray, sphere, 1) then FAILED(9) end
   -- test tangential grazing - hit
   local n = randomdir()
   local dir, _ = ode.plane_basis(n)
   local P = C + 0.99*r*n - dir
   ray:set(P, dir)
   ray:set_length(2)
   if not ode.collide(ray, sphere, 1) then FAILED(10) end
end


local function test_ray_and_box()
   local space = ode.create_simple_space()   
   local ray = ode.create_ray(nil, 0)
   local box = ode.create_box(nil, 1, 1, 1)
   space:add(ray)
   space:add(box)
   -- make a random box
   local s = vec3(random()+.1, random()+.1, random()+.1)
   local C = randomvec3(1.0)
   box:set_lengths(s.x, s.y, s.z)
   box:set_position(C)
   box:set_rotation(randomrot())
   -- test a zero length ray just inside the box
   ray:set_length(0)
   local dir = vec3(randomf(-.5, .5)*s.x, randomf(-.5, .5)*s.y, randomf(-.5, .5)*s.z)
   local i = random(1, 3)
   dir[i] = randomsign()*.99*.5*s[i]
   local P = C + box:get_rotation()*dir
   ray:set_position(P)
   ray:set_rotation(randomrot())
   if ode.collide(ray, box, 1) then FAILED(1) end
   -- test a zero length ray just outside the box
   ray:set_length(0)
   local dir = vec3(randomf(-.5, .5)*s.x, randomf(-.5, .5)*s.y, randomf(-.5, .5)*s.z)
   local i = random(1, 3)
   dir[i] = randomsign()*.99*.5*s[i]
   local P = C + box:get_rotation()*dir
   ray:set_position(P)
   ray:set_rotation(randomrot())
   if ode.collide(ray, box, 1) then FAILED(2) end
   -- test a finite length ray totally contained inside the box
   local dir = vec3(randomf(-.5, .5)*s.x, randomf(-.5, .5)*s.y, randomf(-.5, .5)*s.z)*.99
   local P = C + box:get_rotation()*dir
   local dir = vec3(randomf(-.5, .5)*s.x, randomf(-.5, .5)*s.y, randomf(-.5, .5)*s.z)*.99
   local Q = C + box:get_rotation()*dir
   ray:set(P, (Q-P):normalize())
   ray:set_length((Q-P):norm())
   if ode.collide(ray, box, 1) then FAILED(3) end
   -- test finite length ray totally outside the box
   local dir = vec3(randomf(-.5, .5)*s.x, randomf(-.5, .5)*s.y, randomf(-.5, .5)*s.z)
   local i = random(1, 3)
   dir[i] = randomsign()*1.01*.5*s[i]
   dir = box:get_rotation()*dir
   local P = C + dir
   ray:set(P, dir:normalize())
   ray:set_length(10)
   if ode.collide(ray, box, 1) then FAILED(4) end
   -- test ray from outside to just above surface
   local dir = vec3(randomf(-.5, .5)*s.x, randomf(-.5, .5)*s.y, randomf(-.5, .5)*s.z)
   local i = random(1, 3)
   dir[i] = randomsign()*1.01*.5*s[i]
   dir = box:get_rotation()*dir
   local P = 
   ray:set(C + 2*dir, -(dir:normalize()))
   ray:set_length(dir:norm()*.99)
   if ode.collide(ray, box, 1) then FAILED(5) end
   -- test ray from outside to just below surface
   ray:set_length(dir:norm()*1.01)
   if not ode.collide(ray, box, 1) then FAILED(6) end
   -- test contact point position for random rays
   local P = C + box:get_rotation()*vec3(randomf()*s.x, randomf()*s.y, randomf()*s.z)
   local dir = randomdir()
   ray:set(P, dir)
   ray:set_length(10)
   local collide, contacts = ode.collide(ray, box, 1)
   if collide then
      local contact = contacts[1]
      -- check depth of contact point
      if abs(box:point_depth(contact.position)) > TOL then FAILED(7) end
      -- check position of contact point
      local Q = box:get_rotation()*(contact.position - C) -- to box space
      if abs(abs(Q.x) - s.x/2) > TOL 
         or abs(abs(Q.y) - s.y/2) > TOL 
         or abs(abs(Q.z) - s.z/2) > TOL 
      then FAILED(8)
      end
      -- check normal signs
      if dir*contact.normal > 9 then FAILED(9) end
   end
end


local function test_ray_and_capsule()
   local space = ode.create_simple_space()
   local ray = ode.create_ray(nil, 0)
   local capsule = ode.create_capsule(nil, 1, 1)
   space:add(ray)
   space:add(capsule)
   -- make a random capsule
   local r = randomf()/2 + .01
   local l = randomf() + .01
   capsule:set(r, l)
   local C = randomvec3()
   capsule:set_position(C)
   local rot = randomrot()
   capsule:set_rotation(rot)
   local zaxis = rot:column(3)
   -- test ray completely within capsule
   local k = l*randomf(-.5, .5)
   local P = C + .99*k*zaxis + .99*r*randomdir() -- point in the cap
   local Q = C + .99*k*zaxis + .99*r*randomdir()
   ray:set_length((Q-P):norm())
   ray:set(P, (Q-P):normalize())
   if ode.collide(ray, capsule, 1) then FAILED(1) end
   -- test ray outside capsule that just misses (between caps)
   local n
   repeat n = randomdir() until n*zaxis > 0 -- normal to a cap
   local P = C + l/2*rot*vec3(0,0,1) + 2*r*n -- outside point at distance r from the cap
   ray:set(P, -n:normalize())
   ray:set_length(.99*r)
   if ode.collide(ray, capsule, 1) then FAILED(2) end
   -- test ray outside capsule that just hits (between caps)
   ray:set_length(1.01*r)
   local collide, contacts = ode.collide(ray, capsule, 1)
   if not collide then FAILED(3) end
   -- also check the depth of the contact point
   if abs(capsule:point_depth(contacts[1].position)) > TOL then FAILED(4) end
   -- test ray outside capsule that just misses (caps)
   local n
   repeat n = randomdir() until n*zaxis > 0
   local dir, _ = ode.plane_basis(n)
   local P = C + l/2*rot*vec3(0,0,1) + 1.0001*r*n -- point just outside the cap
   ray:set(P-5*dir, dir)
   ray:set_length(10)
   if ode.collide(ray, capsule, 1) then FAILED(5) end
   -- test ray outside capsule that just hits (caps)
   local P = C + l/2*rot*vec3(0,0,1) + .99*r*n -- point just outside the cap
   ray:set(P-5*dir, dir)
   ray:set_length(10)
   local collide, contacts = ode.collide(ray, capsule, 1)
   if not collide then FAILED(6) end
   -- also check the depth of the contact point
   if abs(capsule:point_depth(contacts[1].position)) > TOL then FAILED(7) end
   -- test random rays
   local P = randomvec3()
   local dir = randomdir()
   ray:set(P, dir)
   ray:set_length(10)
   local collide, contacts = ode.collide(ray, capsule, 1)
   if collide then
      local contact = contacts[1]
      -- check depth of contact point
      if abs(capsule:point_depth(contact.position)) > TOL then FAILED(8) end
      -- check normal signs
      if dir*contact.normal > 0 then FAILED(9) end
   end
end


local function test_ray_and_cylinder()
   local space = ode.create_simple_space()
   local ray = ode.create_ray(nil, 0)
   local cylinder = ode.create_cylinder(nil, 0, 0)
   space:add(ray)
   space:add(cylinder)
   -- make random cylinder
   local r = randomf()+.1
   local l = randomf()+.1
   local C = randomvec3()
   local rot = randomrot()
   local xaxis, yaxis, zaxis = rot:column(1), rot:column(2), rot:column(3) 
   cylinder:set(r, l)
   cylinder:set_position(C)
   cylinder:set_rotation(rot)
   -- test inside ray that just misses the side
   local dir = (randomf()*xaxis + randomf()*yaxis):normalize()
   local P = C + l*randomf(-.5,.5)*zaxis + (r-.02)*dir
   ray:set(P, dir)
   ray:set_length(.01)
   if ode.collide(ray, cylinder, 1) then FAILED(1) end
   -- test inside ray that just hits the side
   ray:set_length(.03)
   if not ode.collide(ray, cylinder, 1) then FAILED(2) end
   local collide, contacts = ode.collide(ray, cylinder, 1)
   if collide then
      local contact = contacts[1]
      -- there is no cylinder:point_depth() method.
      -- check normal signs
      if dir*contact.normal > 0 then FAILED(3) end
   end
   -- test outside ray that just misses the side
   local dir = (randomf()*xaxis + randomf()*yaxis):normalize()
   local P = C + l*randomf(-.5,.5)*zaxis + (r+.02)*dir
   ray:set(P, -dir)
   ray:set_length(.01)
   if ode.collide(ray, cylinder, 1) then FAILED(4) end
   -- test outside ray that just hits the side
   ray:set_length(.03)
   if not ode.collide(ray, cylinder, 1) then FAILED(2) end
   local collide, contacts = ode.collide(ray, cylinder, 1)
   if collide then
      local contact = contacts[1]
      -- there is no cylinder:point_depth() method.
      -- check normal signs
      if dir*contact.normal < 0 then FAILED(3) end
   end
   -- test inside ray that just misses the top or bottom
   local dir = (randomf()*xaxis + randomf()*yaxis):normalize()
   local P = C + (l/2-.02)*zaxis + (r-.02)*dir
   ray:set(P, zaxis)
   ray:set_length(.01)
   if ode.collide(ray, cylinder, 1) then FAILED(4) end
   -- test inside ray that just hits the top or bottom
   ray:set_length(.03)
   if not ode.collide(ray, cylinder, 1) then FAILED(5) end
   local collide, contacts = ode.collide(ray, cylinder, 1)
   if collide then
      local contact = contacts[1]
      -- there is no cylinder:point_depth() method.
      -- check normal signs
      if zaxis*contact.normal > 0 then FAILED(6) end
   end
   -- test outside ray that just misses the top or bottom
   local dir = (randomf()*xaxis + randomf()*yaxis):normalize()
   local P = C + (l/2+.02)*zaxis + (r-.02)*dir
   ray:set(P, -zaxis)
   ray:set_length(.01)
   if ode.collide(ray, cylinder, 1) then FAILED(7) end
   -- test inside ray that just hits the top or bottom
   ray:set_length(.03)
   if not ode.collide(ray, cylinder, 1) then FAILED(8) end
   local collide, contacts = ode.collide(ray, cylinder, 1)
   if collide then
      local contact = contacts[1]
      -- there is no cylinder:point_depth() method.
      -- check normal signs
      if zaxis*contact.normal < 0 then FAILED(9) end
   end
end


local function test_ray_and_plane()
   local space = ode.create_simple_space()
   local ray = ode.create_ray(nil, 0)
   local plane = ode.create_plane(nil, 0, 0, 1, 0)
   space:add(ray)
   space:add(plane)
   -- make a random plane
   local n = randomdir()
   local d = randomf()
   plane:set(n.x, n.y, n.z, d)
   local _, p, q = plane:basis()
   -- test finite length ray below plane (miss)
   local P = p*randomf(-.5, .5) + q*randomf(-.5, .5) + n*(d-.1)
   ray:set(P, n)
   ray:set_length(0.09)
   if ode.collide(ray, plane, 1) then FAILED(1) end
   -- test finite length ray below plane (hit)
   ray:set_length(0.11)
   local collide, contacts = ode.collide(ray, plane, 1) 
   if not collide then
      FAILED(2)
   else -- test contact point depth and contact normal
      if abs(plane:point_depth(contacts[1].position)) > TOL then FAILED(3) end
      if n*contacts[1].normal > 0 then FAILED(4) end
   end
   -- test finite length ray above plane (miss)
   local P = p*randomf(-.5, .5) + q*randomf(-.5, .5) + n*(d+.1)
   ray:set(P, -n)
   ray:set_length(0.09)
   if ode.collide(ray, plane, 1) then FAILED(5) end
   -- test finite length ray above plane (hit)
   ray:set_length(0.11)
   local collide, contacts = ode.collide(ray, plane, 1) 
   if not collide then
      FAILED(6)
   else -- test contact point depth and normal sign
      if abs(plane:point_depth(contacts[1].position)) > TOL then FAILED(7) end
      if n*contacts[1].normal < 0 then FAILED(8) end
   end
end


-------------------------------------------------------------------------------
-- Run tests
-------------------------------------------------------------------------------

local function run_test(name, testfunc, N)
   RESET()
   for i = 1, N do testfunc() end
   local none = "none"
   printf("%s - %d tests, failed: ", name, N)
   for i, c in pairs(COUNTER) do
      none=""
      printf("test %d (%.1f%%) ", i, c/N*100)
   end
   printf("%s\n", none)
end

local N = 1000
run_test(" sphere_point_depth", test_sphere_point_depth, N)
run_test("    box_point_depth", test_box_point_depth, N)
run_test("capsule_point_depth", test_capsule_point_depth, N)
run_test("  plane_point_depth", test_plane_point_depth, N)
run_test("     ray_and_sphere", test_ray_and_sphere, N)
run_test("        ray_and_box", test_ray_and_box, N)
run_test("    ray_and_capsule", test_ray_and_capsule, N)
run_test("   ray_and_cylinder", test_ray_and_cylinder, N)
run_test("      ray_and_plane", test_ray_and_plane, N)

-- @@ There could be some problem in box:point_depth() (i.e. dGeomBoxPointDepth())
-- because also the original tests involving this function fail.
