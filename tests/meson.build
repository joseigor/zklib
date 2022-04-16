unity_dep = dependency('unity', method : 'pkg-config' , fallback : ['unity', 'unity_dep'])

test_zk_slist = \
    executable(
        'test_zk_slist',
        sources: ['test_zk_slist.c'],
        dependencies: [
            unity_dep,
            zklib_dep
        ],
        include_directories : inc_dir
    )

test('test_zk_slist', test_zk_slist)
