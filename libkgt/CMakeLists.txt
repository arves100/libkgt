set(SOURCES
    dll.h
    gpu.h
    hosts.c
    hosts.h
    main.h
    nuklear.c
    nuklear.h
    pattern.c
    pattern.h
    pch.h
    process.h
    rez.h
    ui.h
    utls.h
    utls.c
    utls.inl
)

if (WIN32)
    list(APPEND SOURCES
        dll_win32.c
        main_win32.c
        rez_win32.c
        ui_win32.c
        gpu_d3d9.c
        nuklear_d3d9.h
        process_win32.c
        win32_internal.h
    )
else()
    list(APPEND SOURCES
        main_dummy.c
        nuklear_xlib_gl2.h
    )
endif()

add_library(kgt STATIC ${SOURCES})
target_include_directories(kgt PUBLIC ../)

if (WIN32)
    target_compile_definitions(kgt PUBLIC -D_UNICODE)
endif()
