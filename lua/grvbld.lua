grvbld = {}

grvbld.test_executables = {}
grvbld.build_config = {
  build_dir = "build"
}

grvbld.LOG = {
  INFO = "INFO",
  WARNING = "WARNING",
  ERROR = "ERROR"
}

function grvbld.join(arr, join_str)
  if #arr == 0 then
    return ""
  end

  local res = arr[1]
  join_str = join_str or " "
  for i = 2,#arr do
    res = res .. join_str .. tostring(arr[i])
  end

  return res
end


function grvbld.append_table(dst, src)
  for i = 1, #src do
    dst[#dst + 1] = src[i]
  end
end

function grvbld._init()
  print("Initializing build system")
end

grvbld._init()
function grvbld.append(table, value)
  table[#table + 1] = value
end

function grvbld.log(msg, level)
  level = level or grvbld.LOG.INFO
  print("[" .. level .. "] " .. msg)
end


function grvbld.build_cmd() 
  local c = grvbld.build_config
  local cc = c.cc or "gcc"
  local std = c.std or "gnu11"
  local inc = c.inc
  local warnings = c.warnings

  local cmd = cc
  cmd = cmd .. " --std=" .. std

  if inc then
    for i = 1, #inc do
      cmd = cmd .. " -I" .. inc[i]
    end
  end

  if warnings then
    for i = 1, #warnings do
      cmd = cmd .. " -W" .. warnings[i]
    end
  end

  return cmd
end

function grvbld.make_path(...)
  local components = {...}
  if #components == 0 then
    return ""
  else
    return grvbld.join(components, "/")
  end
end


function grvbld.build_exe(name, ...)
  local dst = grvbld.make_path(grvbld.build_config.build_dir, name)
  local cmd = {grvbld.build_cmd(), "-o", dst}
  grvbld.append_table(cmd, {...})
  cmd = grvbld.join(cmd)
  grvbld.log(cmd)
  local success = os.execute(cmd)
  return success
end


function grvbld.build_test(name)
  local dst = "test_grv_" .. name
  local src = "test/" .. dst .. ".c"
  local success = grvbld.build_exe(dst, "-g", src, "src/grv.c")
  if success then
    grvbld.append(
      grvbld.test_executables,
      grvbld.make_path(grvbld.build_config.build_dir, dst))
  end
end


function grvbld.run_tests()
  for i = 1,#grvbld.test_executables do
    local test = grvbld.test_executables[i]
    print()
    grvbld.log(test)
    os.execute(test)
  end
end


return grvbld