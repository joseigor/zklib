unity_dep = dependency('unity', method : 'pkg-config' , fallback : ['unity', 'unity_dep'])

tests_src_files = []
tests_inc_dir = []

tests_inc_dir += include_directories('.')

subdir('common')

test_zk_c_slist = \
    executable(
        'test_zk_c_slist',
        sources: ['test_zk_c_slist.c',tests_src_files],
        dependencies: [ unity_dep, zklib_dep ],
        include_directories : [inc_dir, tests_inc_dir]
    )

test_zk_dlist = \
    executable(
        'test_zk_dlist',
        sources: ['test_zk_dlist.c',tests_src_files],
        dependencies: [ unity_dep, zklib_dep ],
        include_directories : [inc_dir, tests_inc_dir]
    )

test_zk_slist = \
    executable(
        'test_zk_slist',
        sources: ['test_zk_slist.c',tests_src_files],
        dependencies: [ unity_dep, zklib_dep ],
        include_directories : [inc_dir, tests_inc_dir]
    )

test('test_zk_c_slist', test_zk_c_slist)
test('test_zk_dlist', test_zk_dlist)
test('test_zk_slist', test_zk_slist)
