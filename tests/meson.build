unittest_cpp_dep = dependency('unittest-cpp')

test_files = {
  'test_zarray' : {'src' : 'test_zarray.cpp', 'suite' : 'zodlib'},
  'test_defer'  : {'src' : 'test_defer.cpp', 'suite' : 'zodlib'},
  'test_mem'    : {'src' : 'test_mem.cpp', 'suite' : 'zodlib'},
}

foreach test_name, cfg : test_files
  test_exe = executable(test_name, 'test_main.cpp', cfg['src'], zodlib_src,
    include_directories : inc,
    dependencies : unittest_cpp_dep,
  )
  test(test_name, test_exe, suite : cfg['suite'])
endforeach
