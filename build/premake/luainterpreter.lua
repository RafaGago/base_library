--[[
A premake action to allow use of premake itself as a LUA interpreter. 

E.g. you write a script like this one named "myscript.lua":

> require "luainterpreter"
>
> for k, v in ipairs(_ARGS) do
>   print ("argument "..k..": "..v)
> end

And run it with premake like this:

> ./premake5 luainterpreter --file=myscript.lua ARGUMENT_1 ARGUMENT_N

Which will print the next output:

> argument 1: arg
> argument 2: another_arg
> Running action 'luainterpreter'...

See: https://github.com/premake/premake-core/wiki/Starting-Your-New-Action

]]--

premake.modules.lua = {}
local m = premake.modules.lua
local p = premake

newaction {
  trigger = "luainterpreter",
  description = "use premake just as a LUA interpreter",
}
return m
