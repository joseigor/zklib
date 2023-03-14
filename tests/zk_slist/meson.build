test_zk_slist_reverse = \
    executable(
        'test_zk_slist_reverse',
        sources: ['test_zk_slist_reverse.c'],
        dependencies: [ unity_dep, zklib_dep ],
        include_directories : [inc_dir, tests_inc_dir]
    )

test_zk_slist_size = \
    executable(
        'test_zk_slist_size',
        sources: ['test_zk_slist_size.c'],
        dependencies: [ unity_dep, zklib_dep ],
        include_directories : [inc_dir, tests_inc_dir]
    )

test('test_zk_slist_reverse', test_zk_slist_reverse, suite: 'zk_slist')
test('test_zk_slist_size', test_zk_slist_size, suite: 'zk_slist')
