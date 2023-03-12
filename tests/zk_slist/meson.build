test_zk_slist_reverse = \
    executable(
        'test_zk_slist_reverse',
        sources: ['test_zk_slist_reverse.c'],
        dependencies: [ unity_dep, zklib_dep ],
        include_directories : [inc_dir, tests_inc_dir]
    )

test_zk_slist_length = \
    executable(
        'test_zk_slist_length',
        sources: ['test_zk_slist_length.c'],
        dependencies: [ unity_dep, zklib_dep ],
        include_directories : [inc_dir, tests_inc_dir]
    )

test('test_zk_slist_reverse', test_zk_slist_reverse, suite: 'zk_slist')
test('test_zk_slist_length', test_zk_slist_length, suite: 'zk_slist')
