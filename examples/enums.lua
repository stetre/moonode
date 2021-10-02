#!/usr/bin/env lua
-- MoonODE example: enums.lua

local ode = require("moonode")
print(table.unpack(ode.enum("jointtype")))
print(table.unpack(ode.enum("param")))
print(table.unpack(ode.enum("amotormode")))
print(table.unpack(ode.enum("geomtype")))
print(table.unpack(ode.enum("axesorder")))
print(table.unpack(ode.enum("direction")))
print(table.unpack(ode.enum("datatype")))
print(table.unpack(ode.enum("relativeorientation")))
print(table.unpack(ode.enum("transmissionmode")))

print("'"..table.concat({ode.collideflags(-1)}, "', '").."'")
