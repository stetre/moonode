#!/usr/bin/env lua
-- MoonODE example: version.lua

local ode = require("moonode")

print("\nVersions:")
print(_VERSION)         -- Lua version
print(ode._VERSION)     -- MoonODE version
print(ode._ODE_VERSION) -- ODE version

print("\nConfiguration:")
print(ode.get_configuration())

local tokens = { -- configuration tokens
   "ODE",
   "ODE_single_precision",
   "ODE_double_precision",
   "ODE_EXT_no_debug",
   "ODE_EXT_trimesh",
   "ODE_EXT_opcode",
   "ODE_EXT_gimpact",
   "ODE_OPC_16bit_indices",
   "ODE_OPC_new_collider",
   "ODE_EXT_mt_collisions",
   "ODE_EXT_threading",
   "ODE_THR_builtin_impl",
-- "ODE_unknown_token",
-- "xx xy", -- @@ this returns true (bug?)
}

print("\nConfiguration check, by token:")
for _, token in ipairs(tokens) do
   print(token ..": "..(ode.check_configuration(token) and "yes" or "no"))
end
