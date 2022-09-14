project(
    'zklib',
    'c',
    version : '1.0.0',
    meson_version: '>=0.61.2',
    default_options: ['warning_level=3', 'werror=true', 'c_std=gnu11', 'cpp_std=gnu++17', 'optimization=g'],
)

#  options
unit_test = get_option('unit_test')

subdir('src')
subdir('examples')

if unit_test
    subdir('tests')
endif
