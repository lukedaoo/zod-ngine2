project('zod-ngine2', 'cpp',
  version : '0.1.0',
  default_options : [
    'cpp_std=c++23',
    'warning_level=3',
    'buildtype=debug',
  ]
)

inc = include_directories('src')

executable('zod-ngine2', 'main.cpp', include_directories : inc)

unittest_cpp_dep = dependency('unittest-cpp')

test_files = {
  'test_zarray' : {'src' : 'tests/test_zarray.cpp', 'suite' : 'unit'},
}

foreach test_name, cfg : test_files
  test_exe = executable(test_name, 'tests/test_main.cpp', cfg['src'],
    include_directories : inc,
    dependencies : unittest_cpp_dep,
  )
  test(test_name, test_exe, suite : cfg['suite'])
endforeach
