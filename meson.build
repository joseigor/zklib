project(
    'zklib',
    'c',
    version : '1.0.0',
    meson_version: '>=0.61.2',
    default_options: ['warning_level=3', 'werror=true', 'c_std=gnu11', 'cpp_std=gnu++17', 'optimization=g'],
)

#  options
unit_test = get_option('unit_test')

if unit_test
    add_project_arguments('-DUNIT_TEST', language : 'c')
endif

subdir('src')

if unit_test
    subdir('tests')
endif
