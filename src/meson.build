inc_dir += include_directories('.')

src_files += files([
    'zk_slist.c',
])

# FIXME: change from static library to shared library

zk_slist_lib = static_library('runtime', sources: src_files, include_directories: inc_dir , install : true)

zk_slist_lib_dep = declare_dependency(link_with: zk_slist_lib, include_directories: inc_dir)
