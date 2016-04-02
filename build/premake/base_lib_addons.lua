dofile ("os_addons.lua")
-------------------------------------------------------------------------------
function file_to_str(filepath)
    local f = io.open (filepath, "rb")
    if f == nil then
      return ""
    end
    local content = f:read ("*all")
    f:close()
    return content
end
-------------------------------------------------------------------------------
--[[
 Detects dependencies for .h files on hdr_dir contained on some of the
 include_dirs.

 This is made for base_library code style, which:
 
 -Never #includes using quotes to make easy folder refactoring by using 
  simple sed substitutions on the file content.
  
 -Always uses slashes ("/") as the folder separator.

 -Separates words on the file name by the underscore ("_") character.
]]--
-------------------------------------------------------------------------------
function get_header_dependencies (hdr_dir, include_dirs)  
  local scan        = {}
  local new_results = {}
  local results     = {}
    
  local function append_file_includes(filepath, includes)  
    content = file_to_str (filepath)
    for match in content:gmatch ("#%s*include%s*<%s*[%a%d\-_\\/.]+%s*>") do
      str = match:gsub ("#%s*include%s*<%s*([%a%d\-_\\/.]+)%s*>", "%1")
      includes[str] = 1
    end
  end
    
  matches = os.matchfiles (hdr_dir .. "/**.h")  
  for unused, file in pairs (matches) do    
    append_file_includes (file, scan)
  end
  
  for file, v in pairs (scan) do
    for k, include_dir in pairs (include_dirs) do      
      if os.file_exists (include_dir .. "/" .. file) then                
        new_results[include_dir .. "/" .. file] = file
      end
    end    
  end
  
  repeat
    scan = {}
    for fpath, relpath in pairs (new_results) do
      results[fpath] = relpath
      append_file_includes (fpath, scan) 
    end
    new_results     = {}
    has_new_results = false
    
    for file, v in pairs (scan) do
      for k, include_dir in pairs (include_dirs) do
        fullpath = include_dir .. "/" .. file
        if os.file_exists (fullpath) and results[fullpath] == nil then                
          new_results[include_dir .. "/" .. file] = file
          has_new_results                         = true
        end
      end    
    end
  until (has_new_results == false)

  return results
end
-------------------------------------------------------------------------------
--[[
 Scans all the C headers (.h files) on "scan_dir" for files found on
 "include_dirs" and copies those that are found to "dst_base".
]]--
-------------------------------------------------------------------------------
function copy_header_dependencies (scan_dir, include_dirs, dst_base)
  dst_base = path.getabsolute (dst_base)
  deps     = get_header_dependencies (scan_dir, include_dirs)
  
  if deps == nil then 
    print ("no deps")
    return
  end
  
  for full, relative in pairs (deps) do
    dst = dst_base .. "/" .. relative
    os.mkdir (path.getdirectory (dst))
    if os.copyfile (full, dst) ~= true then
      print ("unable to copy file: " .. full .. " to: " .. dst .. "\n")
    end
  end  
end
-------------------------------------------------------------------------------
