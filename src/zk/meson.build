
src_files +=[]

zklib = \
    library(
        'zklib',
        sources: src_files,
        include_directories: inc_dir,
        pic: true,
        version: meson.project_version(),
        install : true
    )

zklib_dep = declare_dependency(link_with: zklib)

