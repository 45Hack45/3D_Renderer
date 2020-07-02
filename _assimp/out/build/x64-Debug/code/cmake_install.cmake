# Install script for directory: D:/Users/Hack45/source/repos/Renderer3D/_assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "D:/Users/Hack45/source/repos/Renderer3D/_assimp/out/install/x64-Debug")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Users/Hack45/source/repos/Renderer3D/_assimp/out/build/x64-Debug/code/assimp-vc141-mtd.lib")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "D:/Users/Hack45/source/repos/Renderer3D/_assimp/out/build/x64-Debug/code/assimp-vc141-mtd.dll")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/anim.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/aabb.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/ai_assert.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/camera.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/color4.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/color4.inl"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/out/build/x64-Debug/code/../include/assimp/config.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/defs.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Defines.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/cfileio.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/light.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/material.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/material.inl"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/matrix3x3.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/matrix3x3.inl"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/matrix4x4.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/matrix4x4.inl"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/mesh.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/pbrmaterial.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/postprocess.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/quaternion.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/quaternion.inl"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/scene.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/metadata.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/texture.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/types.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/vector2.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/vector2.inl"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/vector3.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/vector3.inl"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/version.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/cimport.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/importerdesc.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Importer.hpp"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/DefaultLogger.hpp"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/ProgressHandler.hpp"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/IOStream.hpp"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/IOSystem.hpp"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Logger.hpp"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/LogStream.hpp"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/NullLogger.hpp"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/cexport.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Exporter.hpp"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/DefaultIOStream.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/DefaultIOSystem.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/ZipArchiveIOSystem.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/SceneCombiner.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/fast_atof.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/qnan.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/BaseImporter.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Hash.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/MemoryIOWrapper.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/ParsingUtils.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/StreamReader.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/StreamWriter.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/StringComparison.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/StringUtils.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/SGSpatialSort.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/GenericProperty.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/SpatialSort.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/SkeletonMeshBuilder.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/SmoothingGroups.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/SmoothingGroups.inl"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/StandardShapes.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/RemoveComments.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Subdivision.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Vertex.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/LineSplitter.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/TinyFormatter.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Profiler.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/LogAux.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Bitmap.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/XMLTools.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/IOStreamBuffer.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/CreateAnimMesh.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/irrXMLWrapper.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/BlobIOSystem.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/MathFunctions.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Macros.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Exceptional.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/ByteSwapper.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xassimp-devx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Compiler/pushpack1.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Compiler/poppack1.h"
    "D:/Users/Hack45/source/repos/Renderer3D/_assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Users/Hack45/source/repos/Renderer3D/_assimp/out/build/x64-Debug/code/assimp-vc141-mtd.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE FILES "D:/Users/Hack45/source/repos/Renderer3D/_assimp/out/build/x64-Debug/code/assimp-vc141-mt.pdb")
  endif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
endif()

