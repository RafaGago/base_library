-------------------------------------------------------------------------------
local order = {}
local msgs  = {}
-------------------------------------------------------------------------------
local indent_str = "  "
-------------------------------------------------------------------------------
local type_sizes = {
  u8     = 1,
  u16    = 2,
  u32    = 4,
  u64    = 8,
  i8     = 1,
  i16    = 2,
  i32    = 4,
  i64    = 8,
  float  = 4,
  double = 8,
}
-------------------------------------------------------------------------------
local keywords = {
  message = "message",
  endfile = "endfile",
  static  = "static",
  dynamic = "dynamic",
  fixed   = "fixed",
}
-------------------------------------------------------------------------------
local function valid_name (name, msg_name)
  if type_sizes[name] or keywords[name] then
    print ("on "..msg_name..". name is a type or keyword: "..name)
    return false
  end
  local match = name:gmatch ("[_%a][_%w]*")
  if not match then
    print ("on "..msg_name..". invalid name:"..name)
    return false
  end
  return true
end
-------------------------------------------------------------------------------
local function valid_msg_name (name, msgs)
  if msgs[name] then
    print ("duplicated message name: "..name)
    return false
  end
  return valid_name (name, name)
end
-------------------------------------------------------------------------------
local function type_is_msg (t, parent, msgs, msg_name)
  if type_sizes[t] then
    return false
  end
  if msgs[t] or t == parent then
    return true
  end
  print ("on "..msg_name..". expected a type, got:"..t)
  return nil
end
-------------------------------------------------------------------------------
local function validate_parse_array_count (t, msg_name)
  local count = t:match ("^%[(%d*)%]$")
  if count == "" then
    print ("on "..msg_name..". array needs to specify it's maximum size: "..t)
    return -1
  elseif count ~= nil then
    count = tonumber (count)
    if count == "0" then
      print ("on "..msg_name..". array can't have a 0 maximum size: "..t)
      return -1
    end
    return count
  else 
    print ("on "..msg_name..". invalid array syntax: "..t)
    return -1    
  end
end
-------------------------------------------------------------------------------
local function get_array_index_type (max_count)
  if max_count < 256 then
    return "u8"
  elseif max_count < 65536 then
    return "u16"
  elseif max_count < 4294967296 then
    return "u32"
  else
    return "u64"
  end
end
-------------------------------------------------------------------------------
local function do_align (size, alig)
  return math.ceil (size / alig) * alig
end
-------------------------------------------------------------------------------
local function msg_postprocess(msg)
  msg.csize          = 0
  msg.fix_wire_size  = 0
  msg.repeated_count = 0
  msg.dynamic_count  = 0
  msg.align          = 1 --recursive

  for _, v in ipairs (msg.fields) do
    msg.csize = msg.csize + v.csize
    if v.array_max == nil and v.is_msg == false then
      --message wsizes just contain the higher level, not the whole nesting
      msg.fix_wire_size = msg.fix_wire_size + v.csize;
    end
    if v.is_dynamic then
      assert (v.array_max ~= nil, "bug")
      msg.dynamic_count = msg.dynamic_count + 1
    end
    if v.array_max ~= nil then
      msg.repeated_count = msg.repeated_count + 1
    end
    msg.align = math.max (msg.align, v.align)
  end
  table.sort (msg.fields, function (a, b)
    --this algorithm can be improved
    if (a.align ~= b.align) then
      return a.align > b.align
    else
      return a.csize > b.csize
    end
  end)
  msg.csize = do_align (msg.csize, msg.align)
end
-------------------------------------------------------------------------------
local msg_parser = coroutine.wrap (function (tk, msgs, order)
  local msg_name = nil
  local msg      = nil

  while true do
    local totalsz = 0;
    while tk[1] ~= "message" and tk[1] ~= "endfile" and msg_name ~= nil do
      local field = {}
      --type
      field.itype    = keywords.fixed
      field.is_count = false
      field.is_msg   = type_is_msg (tk[1], msg_name, msgs, msg_name)
      if field.is_msg == nil then
        coroutine.yield (-1)
      end
      field.type = tk[1]
      if field.is_msg then
        field.csize = msgs[tk[1]].csize
        field.align = msgs[tk[1]].align
      else
        field.csize = type_sizes[tk[1]]
        field.align = field.csize
      end
      coroutine.yield (0)
      if not valid_name (tk[1], msg_name) then
        coroutine.yield (-1)
      end
      --name 
      field.name = tk[1]
      coroutine.yield (0)
      --array (optional)
      if tk[1]:byte (1) == string.byte ("[") then
        field.array_max = validate_parse_array_count (tk[1], msg_name)
        if field.array_max < 1 then
          coroutine.yield (-1)
        end
        coroutine.yield (0)
        field.itype = tk[1]
        if tk[1] == keywords.static or tk[1] == keywords.fixed then          
          field.csize = field.csize * field.array_max
        elseif tk[1] == keywords.dynamic then
          field.csize = ptr_bytes
          field.align = ptr_bytes
        else
          print(
            "on: "..msg_name..". array with no "..keywords.dynamic.."/"..
            keywords.static.."specifiers: "..field.name
            )
        end
        if tj[1] ~= keywords.fixed then
          --adding the count field to the struct
          local cfield = {} 
          cfield.type       = get_array_index_type (field.array_max)
          cfield.is_msg     = false
          cfield.name       = field.name.."_count"
          cfield.csize      = type_sizes[cfield.type]
          cfield.align      = cfield.csize
          cfield.is_count   = true
          table.insert (msg.fields, cfield)
        end
        coroutine.yield (0)     
      end
      --validation and insertion
      if field.type == msg_name and field.itype ~= keywords.dynamic then
        print ("invalid reference to itself:"..msg_name)
        coroutine.yield (-1)
      end

      for _ , v in ipairs (msg) do
        if (field.name == v) then
          print ("on: "..msg_name..". duplicate field name: "..v)
          coroutine.yield (-1)
        end
      end      
      table.insert (msg.fields, field)
    end
    --message init and finishing
    if tk[1] ~= keywords.message and tk[1] ~= keywords.endfile then
      print ("expected the "..keywords.message.." keyword")
      coroutine.yield (-1)
    end

    local yieldval = 0
    if (tk[1] == keywords.endfile) then
      yieldval = 1
    end
    if (msg_name ~= nil) then
      msg_postprocess (msg)
      msgs[msg_name] = msg;
      table.insert (order, msg_name)
    end
    msg = {}
    msg.fields = {}
    coroutine.yield (yieldval)
    if not valid_msg_name (tk[1], msgs) then
      coroutine.yield (-1)
    end
    msg_name = tk[1]
    coroutine.yield (0)
  end
end)
-------------------------------------------------------------------------------
function line_separator (out)
  return out.."/*"..string.rep ("-", 76).."*/\n"
end
-------------------------------------------------------------------------------
function indent (out, n)
  return out..string.rep (indent_str, n)
end
-------------------------------------------------------------------------------
function generate_struct (out, msg, name)
  local o = line_separator (out)
  o = o.."typedef struct "..name.." {\n"
  for k, v in ipairs (msg.fields) do
    o = indent (o, 1)
    if v.array_max == nil then
      o = o..v.type.." "..v.name..";\n"
    elseif v.itype == keywords.dynamic then
      o = o..v.type.."* "..v.name..";\n"
    else
      o = o..v.type.." "..v.name.."["..v.array_max.."];\n"
    end
  end
  o = o.."}\n"..name..";\n"
  return o
end
-------------------------------------------------------------------------------
function generate_wsize(out, msg, name)
  assert (out ~= nil, "wtf")
  local o = line_separator (out)
  o = o.."static inline u64 "..name..
        "_get_wire_size (const "..name.."* v)\n{\n"
  o = indent (o, 1).."bl_assert (v);\n"
  o = indent (o, 1).."u64 wsize = "..msg.fix_wire_size..";\n"
  for k, v in ipairs (msg.fields) do
    if v.is_msg then
      o = indent (o, 1)
      if v.array_max == nil then
        o = o.."wsize += "..v.type.."_get_wire_size (&v->"..v.name..");\n"
      else
        if (v.itype == keywords.dynamic) then
          o = o.."bl_assert (v->"..v.name..");\n"
          o = indent (o, 1)
        end
        o = o.."bl_assert (v->"..v.name.."_count <= "..v.array_max..");\n"
        o = indent (o, 1)
        o = o.."for (uword i = 0; i < v->"..v.name.."_count; ++i) {\n"
        o = indent (o, 2)
        o = o.."wsize += "..v.type.."_get_wire_size (&v->"..v.name.."[i]);\n"
        o = indent (o, 1).."}\n"
      end
    elseif v.array_max ~= nil then
      o = indent (o, 1)
      o = o.."wsize += ((u64)"..v.name.."_count ) * sizeof v->"..v.name..";\n"
    end
  end
  o = indent (o, 1).."return wsize;\n"
  o = o.."}\n"
  return o
end
-------------------------------------------------------------------------------
function generate_header(out, filename)
  def = "__"..filename:gsub ("%.","_").."_H__"
  def = def:upper()
  o   = "#ifndef "..def.."\n"
  o   = "#define "..def.."\n\n"
  o   = o.."#include <string.h>\n"
  o   = o.."#include <bl/base/platform.h>\n"
  o   = o.."#include <bl/base/assert.h>\n"
  o   = o.."#include <bl/base/integer_manipulation.h>\n"
  return def
end
-------------------------------------------------------------------------------
function generate_footer(out, def)
  local o = line_separator (out)
  o = o.."#endif /*"..def.."*/\n"
end
-------------------------------------------------------------------------------
-- MAIN
-------------------------------------------------------------------------------
if _PREMAKE_VERSION ~= nil then --using premake
  require "luainterpreter"
  k, v      = next (_ARGS)
  filename  = v
  k, v      = next (_ARGS, k)
  ptr_bytes = v
else
  filename = arg[1]
end

if filename == nil or ptr_bytes == nil then
  print ("usage param1=msg file param2=pointer size in bytes (1, 2, 4, 8)")
end

ptr_bytes = tonumber (ptr_bytes)
if ptr_bytes == nil or 
   ptr_bytes ~= 1 and
   ptr_bytes ~= 2 and
   ptr_bytes ~= 4 and
   ptr_bytes ~= 8 
then
  print ("usage param1=msg file param2=pointer size in bytes (1, 2, 4, 8)")
end

file = io.open (filename)
if (file == nil) then
  print ("unable to open file "..filename)
  return 1
end
f = file:read ("*all")
f = f:gsub ("\n", " ")
f = f:gsub ("(%S)%[", "%1 [")
f = f:gsub ("%s+", " ")

local tok_tbl = {}
for tok in f:gmatch ("%s?%S+") do
  tok = tok:gsub ("%s", "") 
  tok_tbl[1] = tok
  --print ("token:"..tok)
  local val = msg_parser (tok_tbl, msgs, order)
  if val < 0 then
    return 1
  end
end
tok_tbl[1] = keywords.endfile
if msg_parser (tok_tbl, msgs, order) ~= 1 then
  print ("unexpected end of file")
  return 1
end

local out=""
--[[
for _, msgname in ipairs (order) do
  local m = msgs[msgname]  
  print ("message:"..msgname.." csz:"..m.csize.." wsz:"..m.fix_wire_size)
  for _, v in ipairs (m.fields) do
    print("  "..v.type.." "..v.name.." csz:"..v.csize.." al:"..v.align)
  end
end
]]--

local def = generate_header (out, filename)

for _, msgname in ipairs (order) do
  local m = msgs[msgname]
  out     = generate_struct (out, m, msgname)
end

for _, msgname in ipairs (order) do
  local m = msgs[msgname]
  out     = generate_wsize (out, m, msgname)
end

generate_footer (out, def)

print (out)

return 0
-------------------------------------------------------------------------------

