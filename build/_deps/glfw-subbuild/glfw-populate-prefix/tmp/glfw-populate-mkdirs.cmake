# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/coding-work/complex_projection_rl_like/build/_deps/glfw-src"
  "D:/coding-work/complex_projection_rl_like/build/_deps/glfw-build"
  "D:/coding-work/complex_projection_rl_like/build/_deps/glfw-subbuild/glfw-populate-prefix"
  "D:/coding-work/complex_projection_rl_like/build/_deps/glfw-subbuild/glfw-populate-prefix/tmp"
  "D:/coding-work/complex_projection_rl_like/build/_deps/glfw-subbuild/glfw-populate-prefix/src/glfw-populate-stamp"
  "D:/coding-work/complex_projection_rl_like/build/_deps/glfw-subbuild/glfw-populate-prefix/src"
  "D:/coding-work/complex_projection_rl_like/build/_deps/glfw-subbuild/glfw-populate-prefix/src/glfw-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/coding-work/complex_projection_rl_like/build/_deps/glfw-subbuild/glfw-populate-prefix/src/glfw-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/coding-work/complex_projection_rl_like/build/_deps/glfw-subbuild/glfw-populate-prefix/src/glfw-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
