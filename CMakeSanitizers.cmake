# Idea taken from https://github.com/OSSIA/cmake-modules/blob/master/Sanitize.cmake

function(target_sanitize_clang theTarget)
    target_compile_options(${theTarget} PUBLIC
        $<$<CONFIG:Debug>:-fsanitize=undefined>
        $<$<CONFIG:Debug>:-fsanitize=integer>
        $<$<CONFIG:Debug>:-fsanitize=address>
        $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        )
    target_link_libraries(${theTarget} PUBLIC
        $<$<CONFIG:Debug>:-fsanitize=address>
        )
endfunction()

function(target_sanitize_appleclang theTarget)
target_compile_options(${theTarget} PUBLIC
    $<$<CONFIG:Debug>:-fsanitize=address>
    $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
  )
target_link_libraries(${theTarget} PUBLIC
    $<$<CONFIG:Debug>:-fsanitize=address>
  )
endfunction()

function(target_sanitize_gcc theTarget)
    if(NOT SCORE_COTIRE) ## Sanitizer won't work with PCHs
      target_compile_options(${theTarget} PUBLIC
        $<$<CONFIG:Debug>:-fsanitize=undefined>
        $<$<CONFIG:Debug>:-fuse-ld=gold>
        $<$<CONFIG:Debug>:-fsanitize=address>
        $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
      )
      target_link_libraries(${theTarget} PUBLIC
      $<$<CONFIG:Debug>:-fsanitize=address>
      $<$<CONFIG:Debug>:-fsanitize=undefined>
      $<$<CONFIG:Debug>:-fuse-ld=gold>
      )
    endif()
endfunction()

function(target_sanitize_msvc theTarget)
endfunction()

function(target_sanitize theTarget)
    if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "AppleClang")
        target_sanitize_appleclang(${theTarget})
    elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        if(APPLE)
            target_sanitize_appleclang(${theTarget})
        else()
            target_sanitize_clang(${theTarget})
        endif()
    elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
        target_sanitize_msvc(${theTarget})
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        target_sanitize_gcc(${theTarget})
    endif()
endfunction()