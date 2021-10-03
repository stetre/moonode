#!/usr/bin/env lua
-- MoonODE example: geoms.lua
-- Shows the creation of a few geometric objects
local ode = require("moonode")
ode.trace_objects(true)

local space = ode.create_simple_space()

-- Sphere ----------------------------------------------------------------------
local radius = 10.2
local geom = ode.create_sphere(space, radius)

-- Box -------------------------------------------------------------------------
local lx, ly, lz = 1, 3, 2
local geom = ode.create_box(space, lx, ly, lz)

-- Plane -----------------------------------------------------------------------
local a, b, c, d = 0, 1, 0, 3
local geom = ode.create_plane(space, a, b, c, d)

-- Capsule ---------------------------------------------------------------------
local radius, length = 1.0, 2.0
local geom = ode.create_capsule(space, radius, length)

-- Cylinder --------------------------------------------------------------------
local radius, length = 0.5, 3.0
local geom = ode.create_cylinder(space, radius, length)

-- Ray -------------------------------------------------------------------------
local length = 10.123
local origin, direction = {1.0, 0.0, 3.0}, {0.0, 0.0, 1.0}
local geom = ode.create_ray(space, length)
geom:set(origin, direction)

-- Convex ----------------------------------------------------------------------
local data = require("meshdata.convex_prism")
local geom = ode.create_convex(space, data.planes, data.points, data.polygons)

-- Trimesh ---------------------------------------------------------------------
local data = require("meshdata.world_geom3")
local positions = ode.pack('float', data.positions)
local indices = ode.pack('uint', data.indices)
local tmdata = ode.create_tmdata('float', positions, indices)
local geom = ode.create_trimesh(space, tmdata)

-- Heightfield -----------------------------------------------------------------
local width, depth = 4, 8
local nw, nd = 15, 31
local scale, offset, thickness, wrap = 1.0, 0, 0, false
local data = {} -- height data
for i=1, nw do
   data[i]={}
   for j=1,nd do data[i][j] = ode.randomf(1.0, 2.0) end
end
local data = ode.pack('float', data)
local hfdata = ode.create_hfdata('float', data, width, depth, nw, nd, scale, offset, thickness, wrap)
local geom = ode.create_heightfield(space, hfdata, false)

