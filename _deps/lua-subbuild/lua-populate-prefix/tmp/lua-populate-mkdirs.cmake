# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/Users/user/Documents/Uczelnia/_4Czwarty Semestr/Silniki do gier/R_Projcet/RayEngine_Project_Minimal/_deps/lua-src")
  file(MAKE_DIRECTORY "/Users/user/Documents/Uczelnia/_4Czwarty Semestr/Silniki do gier/R_Projcet/RayEngine_Project_Minimal/_deps/lua-src")
endif()
file(MAKE_DIRECTORY
  "/Users/user/Documents/Uczelnia/_4Czwarty Semestr/Silniki do gier/R_Projcet/RayEngine_Project_Minimal/_deps/lua-build"
  "/Users/user/Documents/Uczelnia/_4Czwarty Semestr/Silniki do gier/R_Projcet/RayEngine_Project_Minimal/_deps/lua-subbuild/lua-populate-prefix"
  "/Users/user/Documents/Uczelnia/_4Czwarty Semestr/Silniki do gier/R_Projcet/RayEngine_Project_Minimal/_deps/lua-subbuild/lua-populate-prefix/tmp"
  "/Users/user/Documents/Uczelnia/_4Czwarty Semestr/Silniki do gier/R_Projcet/RayEngine_Project_Minimal/_deps/lua-subbuild/lua-populate-prefix/src/lua-populate-stamp"
  "/Users/user/Documents/Uczelnia/_4Czwarty Semestr/Silniki do gier/R_Projcet/RayEngine_Project_Minimal/_deps/lua-subbuild/lua-populate-prefix/src"
  "/Users/user/Documents/Uczelnia/_4Czwarty Semestr/Silniki do gier/R_Projcet/RayEngine_Project_Minimal/_deps/lua-subbuild/lua-populate-prefix/src/lua-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/user/Documents/Uczelnia/_4Czwarty Semestr/Silniki do gier/R_Projcet/RayEngine_Project_Minimal/_deps/lua-subbuild/lua-populate-prefix/src/lua-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/user/Documents/Uczelnia/_4Czwarty Semestr/Silniki do gier/R_Projcet/RayEngine_Project_Minimal/_deps/lua-subbuild/lua-populate-prefix/src/lua-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
