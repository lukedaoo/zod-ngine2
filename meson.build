project('zod-ngine2', 'cpp',
  version : '0.1.0',
  default_options : [
    'cpp_std=c++23',
    'warning_level=3',
    'buildtype=debug',
  ]
)

inc = include_directories('src')

if get_option('debug')
  add_project_arguments('-D_DEBUG', language : 'cpp')
endif

zodlib_src = files(
  'src/zodlib/sys/sys_mem.cpp',
  'src/zodlib/sys/sys_mem_area.cpp',
  'src/zodlib/sys/sys_mem_tracker.cpp',
)

executable('zod-ngine2', 'main.cpp', zodlib_src, include_directories : inc)

subdir('tests')
