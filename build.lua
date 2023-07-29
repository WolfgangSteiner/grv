require "lua/grvbld"

grvbld.build_config = {
  cc = "gcc",
  std = "gnu11",
  inc = {"include"},
  warnings = {"all"},
  build_dir = "build"
}


grvbld.build_test("str")
grvbld.build_test("strarr")
grvbld.build_test("util")
grvbld.build_test("fs") 
grvbld.build_test("strmap")
grvbld.run_tests()
