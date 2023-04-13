## MoonODE: Lua bindings for ODE

MoonODE is a Lua binding library for the [Open Dynamics Engine](https://www.ode.org) (3D physics engine).

It runs on GNU/Linux and on Windows (MSYS2/MinGW) and requires 
[Lua](http://www.lua.org/) (>=5.3) and [ODE](https://bitbucket.org/odedevs/ode/downloads/) (>= 0.16.2).

_Author:_ _[Stefano Trettel](https://www.linkedin.com/in/stetre)_

[![Lua logo](./doc/powered-by-lua.gif)](http://www.lua.org/)

#### License

MIT/X11 license (same as Lua). See [LICENSE](./LICENSE).

#### Documentation

See the [Reference Manual](https://stetre.github.io/moonode/doc/index.html).

#### Getting and installing

Setup the build environment as described [here](https://github.com/stetre/moonlibs), then:

```sh
$ git clone https://github.com/stetre/moonode/
$ cd moonode
moonode$ make
moonode$ sudo make install
```

#### Example

The example below shows the simulation of a box and a cylinder sliding down an inclined terrain.

Other examples can be found in the **examples/** directory contained in the release package.

```lua
-- MoonODE example: hello.lua
-- Derived from ode/demo/demo_cyl.cpp
local ode = require("moonode")

local pi = math.pi
local now, since = ode.now, ode.since
local function printf(...) io.write(string.format(...)) end

local STEP = 0.005 -- simulation step (seconds)
local DURATION = 5 -- seconds
local RENDER_INTERVAL = .2 -- seconds
local GROUPID = 0 -- group id for contact joints
local BOX_SIDE=0.4
local CYL_RADIUS, CYL_WIDTH, CYL_MASS = 0.22, 0.2, 0.2

-- Create world:
local world = ode.create_world()
local space = ode.create_hash_space()
world:set_gravity({0, 0, -9.8})
world:set_quick_step_num_iterations(12)

-- Create a static terrain using a triangle mesh that we can collide with:
local meshdata = require("meshdata.world_geom3")
local positions = ode.pack('float', meshdata.positions)
local indices = ode.pack('uint', meshdata.indices)
printf("positions: %d, indices: %d\n", #meshdata.positions/3, #meshdata.indices)
meshdata = nil -- don't need this any more
local tmdata = ode.create_tmdata('float', positions, indices)
local terrain = ode.create_trimesh(space, tmdata)
terrain:set_position({0, 0, 0.5})
terrain:set_rotation(ode.r_from_axis_and_angle({0, 1, 0}, 0.0))

-- Create a box:
local box = ode.create_box(nil, BOX_SIDE, BOX_SIDE, BOX_SIDE)
local body = ode.create_body(world)
body:set_mass(ode.mass_box(1, BOX_SIDE, BOX_SIDE, BOX_SIDE, 1))
box:set_body(body)
space:add(box)

-- Create a cylinder:
local cyl = ode.create_cylinder(nil, CYL_RADIUS, CYL_WIDTH)
local body = ode.create_body(world)
body:set_mass(ode.mass_sphere(1, CYL_RADIUS, CYL_MASS))
cyl:set_body(body)
space:add(cyl)

-- Set initial conditions:
local sx, sy, sz = -4, -4, 2
local q = ode.q_from_axis_and_angle({1, 0, 0}, pi/2)
local body = box:get_body()
body:set_position({sz, sy+1, sz})
body:set_linear_vel({0, 0, 0})
body:set_angular_vel({0, 0, 0})
body:set_quaternion(q)
local body = cyl:get_body()
body:set_position({sz, sy, sz})
body:set_linear_vel({0, 0, 0})
body:set_angular_vel({0, 0, 0})
body:set_quaternion(q)

-- Prepare surface parameters for joint contacts. Since they are always the same,
-- for efficiency we do it only once before starting the simulation:
local surfpar = ode.pack_surfaceparameters({
   mu = 50.0,
   slip1 = 0.7,
   slip2 = 0.7,
   soft_erp = 0.96,
   soft_cfm = 0.04,
   approx1 = true,
})

-- Set the 'near callback', invoked when two geoms are potentially colliding:
ode.set_near_callback(function(o1, o2)
   local collide, contactpoints = ode.collide(o1, o2, 32)
   if not collide then return end
   for _, contactpoint in ipairs(contactpoints) do
      local joint = ode.create_contact_joint(world, GROUPID, contactpoint, surfpar)
      joint:attach(o1:get_body(), o2:get_body())
   end
end)

-- Run simulation
local t_start = now()
local t_stop = t_start + DURATION
local t, t_last, t_next_render = t_start, t_start, t_start
local n, remainder = 0, 0

while t < t_stop do
   t = now()
   local dt = t - t_last -- time elapsed since last iteration
   -- Compute the number n of time steps to be do at this iteration,
   -- and the remainder to be accounted for at the next one:
   local tot_dt = dt + remainder
   n, remainder = tot_dt//STEP, tot_dt%STEP
   for i=1, n do
      ode.space_collide(space)
      world:quick_step(STEP)
      ode.destroy_joint_group(GROUPID) 
   end
   -- Now it would be the time to render the scene, but here for simplicity
   -- we just print the positions of the objects, at regular intervals:
   if t > t_next_render then
      local p = box:get_position()
      printf("[%f] box position: { %f, %f, %f }\n", t-t_start, p[1], p[2], p[3])
      local p = cyl:get_position()
      printf("[%f] cyl position: { %f, %f, %f }\n", t-t_start, p[1], p[2], p[3])
      t_next_render = t_next_render + RENDER_INTERVAL
   end
   t_last = t
end

```

The script can be executed at the shell prompt with the standard Lua interpreter:

```shell
$ lua hello.lua
```

#### See also

* [MoonLibs - Graphics and Audio Lua Libraries](https://github.com/stetre/moonlibs).
