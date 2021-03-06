local planes={
   { 0.0,0.0,-1.0,1.0 },
   { 0.0,0.0,1.0,1.0 },
   { 1.0,0.0,0.0,10.0 },
   { 0.0,-1.0,0.0,1.0 },
   { -1.0,0.0,-0.0,10.0 },
   { 0.0,1.0,0.0,1.0 },
}

local points={
   {  10.0, 1.0,-1.0 },
   {  10.0,-1.0,-1.0 },
   { -10.0,-1.0,-1.0 },
   { -10.0, 1.0,-1.0 },
   {  10.0, 1.0, 1.0 },
   {  10.0,-1.0, 1.0 },
   { -10.0,-1.0, 1.0 },
   { -10.0, 1.0, 1.0 },
}

local polygons = {
   { 0,1,2,3 },
   { 4,7,6,5 },
   { 0,4,5,1 },
   { 1,5,6,2 },
   { 2,6,7,3 },
   { 4,0,3,7 },
}

return {
   planes = planes,
   points = points,
   polygons = polygons,
}
