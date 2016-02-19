-------------------------------------------------------------------------------
local function os_is_posix()
  return os.get() ~= "windows"
end

os.is_posix = os_is_posix
-------------------------------------------------------------------------------
if os.is_posix() then
  function copydir (src, dst)
    return os.executef ("cp -rf %s %s", src, dst)
  end
elseif os.is("windows") then
  function copydir (src, dst)    
    return os.executef ("robocopy %s %s /E /NJS /nc /ns /np", src, dst)    
  end
else
  print ("unsuported os. the script will fail to run")
end

os.copydir = copydir
-------------------------------------------------------------------------------
local function file_exists(filepath)
  file = io.open (filepath, "rb")
  res  = file ~= nil
  if res then
    file:close()
  end
  return res
end

os.file_exists = file_exists
-------------------------------------------------------------------------------