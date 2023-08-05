require "lua/grvbld"

grvbld.build_config = {
  cc = "gcc",
  std = "gnu11",
  inc = {"include"},
  warnings = {"all"},
  build_dir = "build"
}


grvbld.test("grv_common")
grvbld.test("grv_str")
grvbld.test("grv_strarr")
grvbld.test("grv_util")
grvbld.test("grv_fs") 
grvbld.test("grv_strmap")
