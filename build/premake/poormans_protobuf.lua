--UNFINISHED, UNTESTED!!!!
--EXAMPLE
--[[
message msg1
  u32 a
  u16 b

message msg2
  u32 a
  u32 b

message msg3
  msg2  a
  msg1  b

message msg4
  msg2  a
  u8    b[5] dynamic

message msg5
  msg2  a
  u8    b[5] static

message msg6
  msg2  a
  u8    b[5] fixed

message msg7
  msg2  a
  u32   b[5] dynamic

message msg8
  msg2  a
  u32   b[5] static

message msg9
  msg2  a
  u32   b[5] fixed

message msg10
  msg2  a
  msg1  b[5] dynamic

message msg11
  msg2  a
  msg1  b[5] static

message msg12
  msg2  a
  msg1  b[5] fixed

message msg13
  msg2  a
  msg1  b[5] dynamic
  u8    c[7] dynamic

message msg14
  msg2  a
  msg1  b[5] static
  u8    c[7] dynamic

message msg15
  msg2  a
  msg1  b[5] fixed
  u8    c[7] dynamic
]]--
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
local msg_layout = {
  fixed    = "fixed",
  variable = "variable",
  multi    = "multi"
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
local function msg_before_insert_reorder_and_classify (msg, msgs)
  msg.csize                 = 0
  msg.fix_wire_size         = 0 --contains builtins and fixed builtin arrays
  msg.variable_count        = 0
  msg.dynamic_count         = 0
  msg.nested_count          = 0
  msg.nested_variable_count = 0
  msg.align                 = 1 --recursive

  for _, v in ipairs (msg.fields) do
    msg.csize = msg.csize + v.csize
    if v.is_msg then
      msg.nested_count          = msg.nested_count + 1
      local nested              = msgs[v.type]
      msg.nested_variable_count = 
        msg.nested_variable_count + nested.nested_variable_count
      msg.nested_variable_count = 
        msg.nested_variable_count + nested.variable_count
    end
    if v.array_max == nil and v.is_msg == false then
      --message wsizes just contain the higher level, not the whole nesting
      msg.fix_wire_size = msg.fix_wire_size + v.csize;
    end
    if v.array_max ~= nil and 
       v.is_msg == false and 
       v.itype == keywords.fixed then
      --message wsizes just contain the higher level, not the whole nesting
      msg.fix_wire_size = msg.fix_wire_size + v.csize;
    end
    if v.itype == keywords.dynamic then
      assert (v.array_max ~= nil, "bug")
      msg.dynamic_count = msg.dynamic_count + 1
    end
    if v.array_max ~= nil and v.itype ~= keywords.fixed then
      msg.variable_count = msg.variable_count + 1
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
      field.itype   = keywords.fixed
      field.is_size = false
      field.is_msg  = type_is_msg (tk[1], msg_name, msgs, msg_name)
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
        if tk[1] ~= keywords.fixed then
          --adding the count field to the struct
          local cfield = {} 
          cfield.type    = get_array_index_type (field.array_max)
          cfield.is_msg  = false
          cfield.name    = field.name.."_size"
          cfield.csize   = type_sizes[cfield.type]
          cfield.align   = cfield.csize
          cfield.is_size = true
          cfield.itype   = keywords.fixed
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
      msg_before_insert_reorder_and_classify (msg, msgs)
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
function generate_structs (out, msg, name)
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
  o = o.."} "..name..";\n"
  return o
end
-------------------------------------------------------------------------------
function generate_constants (out, msg, name)
  local o = line_separator (out)
  o = o.."enum "..name.."_constants {\n"
  o = indent (o, 1)..name.."_fix_wsize = \n"
  count=0
  for k, v in ipairs (msg.fields) do
    --wsizes don't contain the whole nesting
    if v.itype == keywords.fixed and v.is_msg == false then 
      if count ~= 0 then
        o = o.." +\n"
      end
      count = count + 1
      o = indent (o, 2)
      o = o.."sizeof (("..name.."*) 0)->"..v.name..")"
    end
  end
  if count == 0 then
    o = indent (o, 2).."0,\n"    
  else
    o = o..",\n"
  end

  for k, v in ipairs (msg.fields) do
    if v.itype ~= keywords.fixed then
      o = indent (o, 1)..name.."_"..v.name.."_max = "..v.array_max..",\n"
    end
  end
  o = o.."};\n"
  return o
end
-------------------------------------------------------------------------------
function generate_wsize(out, msg, name)
  assert (out ~= nil, "wtf")
  local o = line_separator (out)
  o = o..string.format(
    "static inline u64 %s_get_wire_size (%s const* v)\n{\n", name, name
    )
  o = indent (o, 1).."bl_assert (v);\n"
  o = indent (o, 1).."static_assert(\n"
  o = indent (o, 2)..name.."_fix_wsize == "..msg.fix_wire_size..",\n"
  o = indent (o, 2).."\"bug on the code generator\"\n"
  o = indent (o, 2)..");\n"
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
        if (v.itype ~= keywords.fixed) then
          o = o.."bl_assert (v->"..v.name.."_size <= "..name.."_"..v.name
              .."_max);\n"
          o = indent (o, 1)
          o = o.."for (uword i = 0; i < v->"..v.name.."_size; ++i) {\n"
          o = indent (o, 2)
          o = o.."wsize += "..v.type.."_get_wire_size (&v->"..v.name.."[i]);\n"
          o = indent (o, 1).."}\n"
        end
      end
    elseif v.array_max ~= nil and v.itype ~= keywords.fixed then
      --dynamic array
      o = indent (o, 1)
      o = o.."wsize += ((u64) "..v.name.."_size) * sizeof v->"
            ..v.name.."[0];\n"
    end
  end
  o = indent (o, 1).."return wsize;\n"
  o = o.."}\n"
  return o
end
-------------------------------------------------------------------------------
function reorder_for_wire_transfer (msg, msgs)
  local total_var = msg.nested_variable_count + msg.variable_count
  if total_var == 0 then
    msg.layout = msg_layout.fixed
    return 0
  elseif total_var == 1 then
    msg.layout = msg_layout.variable
  else
    msg.layout = msg_layout.multi
  end

  local fixed    = {}
  local variable = {}

  for _, v in ipairs (msg.fields) do
    if v.is_msg then
      if v.array_max ~= nil and v.itype ~= keywords.fixed then
        table.insert (variable, v)
      elseif msgs[v.type].variable_count > 0 or 
             msgs[v.type].nested_variable_count > 0 then
        table.insert (variable, v)
      else
        table.insert (fixed, v)
      end
    else
      if v.array_max ~= nil and v.itype ~= keywords.fixed then
        table.insert (variable, v)
      else
        table.insert (fixed, v)
      end
    end
  end
  
  msg.fields = {}
  for _, v in ipairs (fixed) do
    table.insert (msg.fields, v)
  end
  for _, v in ipairs (variable) do
    table.insert (msg.fields, v)
  end
end
-------------------------------------------------------------------------------
function generate_builtin_pack (o, field, dst_var, msg_ptr, indentlvl, idx)
  if idx == nil then
    idx = ""
  end
  o = indent (o, indentlvl)
  if     field.type == "u8"  or field.type == "i8" then
    o = o..string.format(
      "*%s = (u8) %s->%s%s;\n", dst_var, msg_ptr, field.name, idx
      )
  elseif field.type == "u16" or field.type == "i16" then
    o = o..string.format(
      "write_u16_le (%s, %s->%s%s);\n", dst_var, msg_ptr, field.name, idx
      )
  elseif field.type == "u32" or field.type == "i32" then
    o = o..string.format(
      "write_u32_le (%s, %s->%s%s);\n", dst_var, msg_ptr, field.name, idx
      )
  elseif field.type == "u64" or field.type == "i64" then
    o = o..string.format(
      "write_u64_le (%s, %s->%s%s);\n", dst_var, msg_ptr, field.name, idx
      )
  else
    assert (false, "bug:")
  end
  o = indent (o, indentlvl)..string.format(
    "%s += sizeof %s->%s%s;\n", dst_var, msg_ptr, field.name, idx
    )
  return o
end
-------------------------------------------------------------------------------
function generate_builtin_unpack (o, field, src_var, msg_ptr, indentlvl, idx)
  if idx == nil then
    idx = ""
  end
  o = indent (o, indentlvl)
  if     field.type == "u8"  or field.type == "i8" then
    o = o..string.format(
      "%s->%s%s = (u8) *%s;\n", msg_ptr, field.name, idx, src_var
      )
  elseif field.type == "u16" or field.type == "i16" then
    o = o..string.format(
      "%s->%s%s = read_u16_le (%s);\n", msg_ptr, field.name, idx, src_var
      )
  elseif field.type == "u32" or field.type == "i32" then
    o = o..string.format(
      "%s->%s%s = read_u32_le (%s);\n", msg_ptr, field.name, idx, src_var
            )
  elseif field.type == "u64" or field.type == "i64" then
    o = o..string.format(
      "%s->%s%s = read_u64_le \%s);\n", msg_ptr, field.name, idx, src_var
      )
  else
    assert (false, "bug:")
  end
  o = indent (o, indentlvl)..string.format(
    "%s += sizeof %s->%s%s;\n", src_var, msg_ptr, field.name, idx
    )
  return o
end
-------------------------------------------------------------------------------
function generate_nested_pack(
  o, field, dst_var, msg_ptr, indentlvl, idx, alloc, goto_err_tag, err_val
  )
  if idx == nil then
    idx = ""
  end
  o = indent (o, indentlvl)..string.format(
      "i64 %s_res = %s_pack (%s, %s_size, &%s->%s%s",
      field.name, field.type, dst_var, dst_var, msg_ptr, field.name, idx
      )
  if alloc == nil then
    o = o..");\n"
  else
    o = o..", "..alloc..");\n"
  end
  o = indent (o, indentlvl)..string.format(
    "if (unlikely (%s_res < 0)) {\n", field.name
    )
  if goto_err_tag == nil then
    o = indent (o, indentlvl + 1).."return "..field.name.."_res;\n"
  else
    o = indent (o, indentlvl + 1)..err_val.." = "..field.name.."_res;\n"
    o = indent (o, indentlvl + 1).."goto "..goto_err_tag..";\n"
  end
  o = indent (o, indentlvl).."}\n"
  o = indent (o, indentlvl)..string.format(
    "%s += %s_res;\n", dst_var, field.name
    )
  return o
end
-------------------------------------------------------------------------------
function generate_nested_unpack(
  o, field, src_var, msg_ptr, indentlvl, alloc, goto_err_tag, err_val
  )
  if idx == nil then
    idx = ""
  end
  o = indent (o, indentlvl)..string.format(
      "i64 %s_res = %s_unpack (&%s->%s%s, %s, %s_size",
      field.name, field.type, msg_ptr, field.name, idx, src_var, src_var
      )
  if alloc == nil then
    o = o..");\n"
  else
    o = o..", "..alloc..");\n"
  end
  o = indent (o, indentlvl)..string.format(
    "if (unlikely (%s_res < 0)) {\n", field.name
    )
  if goto_err_tag == nil then
    o = indent (o, indentlvl + 1).."return "..field.name.."_res;\n"
  else
    o = indent (o, indentlvl + 1)..err_val.." = "..field.name.."_res;\n"
    o = indent (o, indentlvl + 1).."goto "..goto_err_tag..";\n"
  end
  o = indent (o, indentlvl).."}\n"
  o = indent (o, indentlvl)..string.format(
    "%s += %s_res;\n", src_var, field.name
    )
  return o
end
-------------------------------------------------------------------------------
function generate_builtin_array_pack (o, field, dst_var, msg_ptr, indentlvl)
  assert (field.array_max ~= nil, "BUG")

  if field.type ~= "u8" and field.type ~= "i8" then
    o = o.."#ifdef BL_LITTLE_ENDIAN\n"
  end

  if field.itype == keywords.fixed then
    o = indent (o, indentlvl)..string.format(
      "memcpy (%s, %s->%s, sizeof %s->%s);\n", 
      dst_var, msg_ptr, field.name, msg_ptr, field.name
      )
      o = indent (o, indentlvl)..string.format(
        "%s += sizeof %s->%s;\n",
        dst_var, msg_ptr, field.name
        )
  else
    o = indent (o, indentlvl)..string.format(
      "memcpy (%s, %s->%s, %s_size * sizeof %s->%s[0]);\n",
      dst_var, msg_ptr, field.name, field.name, msg_ptr, field.name
      )
    o = indent (o, indentlvl)..string.format(
        "%s += %s_size * sizeof %s->%s[0];\n",
        dst_var, field.name, msg_ptr, field.name
        )
  end

  if field.type == "u8" or field.type == "i8" then
    return o
  end

  o = o.."#else\n"
  if field.itype == keywords.fixed then
    o = indent (o, indentlvl)..string.format(
      "for (u64 i = 0; i < arr_elems (%s->%s); ++i) {\n", msg_ptr, field.name
      )
  else
    o = indent (o, indentlvl)..string.format(
      "for (u64 i = 0; i < %s->%s_size; ++i) {\n", msg_ptr, field.name
      )
  end
  o = generate_builtin_pack (o, field, dst_var, msg_ptr, indentlvl + 1, "[i]")
  o = indent (o, indentlvl).."}\n"
  o = o.."#endif\n"
  return o
end
-------------------------------------------------------------------------------
function generate_builtin_array_unpack(
  o, field, src_var, msg_ptr, indentlvl, alloc, goto_err_tag, err_val
  )
  assert (field.array_max, "BUG")
  if alloc ~= nil then
    assert (field.itype == keywords.dynamic, "BUG")
    o = indent (o, indentlvl)..string.format(
      "%s->%s = (%s*) bl_alloc (&%s, %s->%s_size * sizeof %s->%s[0]);\n",
      msg_ptr, field.name, field.type, alloc, msg_ptr, field.name, msg_ptr, 
      field.name
      )
    o = indent (o, indentlvl)..string.format(
      "if (!%s->%s) {\n", msg_ptr, field.name
      )
    if goto_err_tag ~= nil then
      o = indent (o, indentlvl + 1)..string.format ("%s = -bl_alloc;\n", err_val)
      o = indent (o, indentlvl + 1)..string.format ("goto %s;\n", goto_err_tag)
    else
      o = indent (o, indentlvl + 1).."return -bl_alloc;\n"
    end
    o = indent (o, indentlvl).."}\n"
  end

  if field.type ~= "u8" and field.type ~= "i8" then
    o = o.."#ifdef BL_LITTLE_ENDIAN\n"
  end

  if field.itype == keywords.fixed then
    o = indent (o, indentlvl)..string.format(
      "memcpy (%s->%s, %s, sizeof %s->%s);\n", 
      msg_ptr, field.name, src_var, msg_ptr, field.name
      )
      o = indent (o, indentlvl)..string.format(
        "%s += sizeof %s->%s;\n",
        src_var, msg_ptr, field.name
        )
  else
    o = indent (o, indentlvl)..string.format(
      "memcpy (%s->%s, %s, %s_size * sizeof %s->%s[0]);\n",
      msg_ptr, field.name, src_var, field.name, msg_ptr, field.name
      )
    o = indent (o, indentlvl)..string.format(
      "%s += %s_size * sizeof %s->%s[0];\n",
      src_var, field.name, msg_ptr, field.name
      )
  end

  if field.type == "u8" or field.type == "i8" then
    return o
  end

  o = o.."#else\n"
  if field.itype == keywords.fixed then
    o = indent (o, indentlvl)..string.format(
      "for (u64 i = 0; i < arr_elems (%s->%s); ++i) {\n", msg_ptr, field.name
      )
  else
    o = indent (o, indentlvl)..string.format(
      "for (u64 i = 0; i < %s->%s_size; ++i) {\n", msg_ptr, field.name
      )
  end
  o = generate_builtin_unpack(
    o, field, src_var, msg_ptr, indentlvl + 1, "[i]"
    )
  o = indent (o, indentlvl).."}\n"
  o = o.."#endif\n"
  return o
end
-------------------------------------------------------------------------------
--data structures containing just built-in types and simple nested
function generate_basic (out, msg, name)
  local o = line_separator (out)
  o = o.."i64 "..name.."_pack (u8* dst, u64 dst_size, "..name.." const* m)\n{\n"
  o = indent (o, 1).."if (unlikely (dst_size < "..name.."_fix_wsize)) {\n"
  o = indent (o, 2).."return -bl_invalid;\n"
  o = indent (o, 1).."}\n"
  o = indent (o, 1).."u8* d = dst;\n"
  for _, v in ipairs (msg.fields) do
    if v.array_max ~= nil then 
      if v.is_msg == false then
        o = generate_builtin_array_pack (o, v, "d", "m", 1)
      else
        o = o.."TODO\n"
      end
    else
      if v.is_msg == false then
        o = generate_builtin_pack (o, v, "d", "m", 1)
      else
        o = generate_nested_pack (o, v, "d", "m", 1)
      end   
    end
  end
  o = indent (o, 1).."return d - dst;\n}\n"
  o = line_separator (o)
  o = o.."i64 "..name.."_unpack ("..name.."* m, u8* src, u64 src_size)\n{\n"
  o = indent (o, 1).."if (unlikely (src_size < "..name.."_fix_wsize)) {\n"
  o = indent (o, 2).."return -bl_invalid;\n"
  o = indent (o, 1).."}\n"
  o = indent (o, 1).."u8* s = src;\n"
  for _, v in ipairs (msg.fields) do
    if v.array_max ~= nil then 
      if v.is_msg == false then
        --o = generate_builtin_array_unpack (o, v, "s", "m", 1)
      else
        o = o.."TODO\n"
      end
    else
      if v.is_msg == false then
        o = generate_builtin_unpack (o, v, "s", "m", 1)
      else
        o = generate_nested_unpack (o, v, "s", "m", 1)
      end   
    end
  end
  o = indent (o, 1).."return s - src;\n}\n"
  return o
end
-------------------------------------------------------------------------------
function generate_pack_unpack (out, msg, msg_name)
--  msg.variable_count        = 0
--  msg.dynamic_count         = 0
--  msg.nested_count          = 0
--  msg.nested_variable_count = 0
  if msg.variable_count == 0 and msg.nested_variable_count == 0 then
    out = generate_basic (out, msg, msg_name)
  end
  return out
end
-------------------------------------------------------------------------------
function generate_header(out, filename)
  local def = "__"..filename:gsub ("%.","_").."_H__"
  def       = def:upper()
  local o   = out.."#ifndef "..def.."\n"
  o         = o.."#define "..def.."\n\n"
  o         = o.."#include <string.h>\n"
  o         = o.."#include <bl/base/utility.h>\n"
  o         = o.."#include <bl/base/platform.h>\n"
  o         = o.."#include <bl/base/assert.h>\n"
  o         = o.."#include <bl/base/integer_manipulation.h>\n"
  return def, o
end
-------------------------------------------------------------------------------
function generate_footer(out, def)
  local o = line_separator (out)
  o = o.."#endif /*"..def.."*/\n"
  return o
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
--f = f:gsub ("//[^\n]*\n", "")
f = f:gsub ("\n", " ")
f = f:gsub ("\t", " ")
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
local def, out = generate_header (out, filename)

for _, msgname in ipairs (order) do
  local m = msgs[msgname]
  out     = generate_structs (out, m, msgname)
  out     = generate_constants (out, m, msgname)
end

for _, msgname in ipairs (order) do
  local m = msgs[msgname]
  out     = generate_wsize (out, m, msgname)
end

for _, msgname in ipairs (order) do
  local m = msgs[msgname]
  reorder_for_wire_transfer (m, msgs)
end

for _, msgname in ipairs (order) do
  local m = msgs[msgname]
  out     = generate_pack_unpack (out, m, msgname)
end

out = generate_footer (out, def)
print (out)
--[[
for _, msgname in ipairs (order) do
  local m = msgs[msgname]  
  print ("message:"..msgname.." csz:"..m.csize.." wsz:"..m.fix_wire_size)
  for _, v in ipairs (m.fields) do
    print("  "..v.type.." "..v.name.." csz:"..v.csize.." al:"..v.align)
  end
end
]]--
return 0
-------------------------------------------------------------------------------

