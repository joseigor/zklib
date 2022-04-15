unity_dep = dependency('unity', method : 'pkg-config' , fallback : ['unity', 'unity_dep'])

test_zk_slist = \
    executable(
        'test_zk_slist',
        sources: [src_files, 'test_zk_slist.c'],
        dependencies: [
            unity_dep,
            zk_slist_lib_dep
        ]
    )

test('test_zk_slist', test_zk_slist)
