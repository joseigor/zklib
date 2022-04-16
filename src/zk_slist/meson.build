zk_slist_src = [
    'zk_slist.c'
]

src_files += files([zk_slist_src])

zk_slist = \
    library(
        'zkslist',
        sources: zk_slist_src,
        include_directories: inc_dir,
        dependencies:[
            zk_common_dep
        ],
        pic: true,
        version: meson.project_version(),
        install : true
    )
