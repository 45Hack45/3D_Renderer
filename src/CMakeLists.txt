
message(STATUS "-- Generating Core Engine")

set(3D_Renderer_core_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Asset.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Color.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Component.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Editor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Entity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FluidSimulation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FrameBuffer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IO.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Material.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MeshRenderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Model.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ParticleSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderPass_Opaque.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderPass_ShadowMap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderPass_Transparent.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderPass_VolumetricLight.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Texture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Transform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VolumetricCloud.cpp
)

set(3D_Renderer_core_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Asset.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Color.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Component.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Editor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Engine.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Entity.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FluidSimulation.h
    ${CMAKE_CURRENT_SOURCE_DIR}/FrameBuffer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/IEditorGUISelectable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Input.h
    ${CMAKE_CURRENT_SOURCE_DIR}/IO.h
    ${CMAKE_CURRENT_SOURCE_DIR}/KeyCode.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LightSource.h
    ${CMAKE_CURRENT_SOURCE_DIR}/logger.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Material.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MaterialManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Mesh.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MeshRenderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Model.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ParticleSystem.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Renderer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderPass_Opaque.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderPass_ShadowMap.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderPass_Transparent.h
    ${CMAKE_CURRENT_SOURCE_DIR}/RenderPass_VolumetricLight.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Scene.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Shader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ShaderManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TestRender.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Texture.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TextureManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Transform.h
    ${CMAKE_CURRENT_SOURCE_DIR}/VolumetricCloud.h
)

set(3D_Renderer_imgui
    ${ENGINE_INCLUDE}/imgui/imconfig.h
    ${ENGINE_INCLUDE}/imgui/imgui_demo.cpp
    ${ENGINE_INCLUDE}/imgui/imgui_draw.cpp
    ${ENGINE_INCLUDE}/imgui/imgui_internal.h
    ${ENGINE_INCLUDE}/imgui/imgui_widgets.cpp
    ${ENGINE_INCLUDE}/imgui/imgui.cpp
    ${ENGINE_INCLUDE}/imgui/imgui.h
    ${ENGINE_INCLUDE}/imgui/imstb_rectpack.h
    ${ENGINE_INCLUDE}/imgui/imstb_textedit.h
    ${ENGINE_INCLUDE}/imgui/imstb_truetype.h
    ${ENGINE_INCLUDE}/imgui/examples/imgui_impl_glfw.cpp
    ${ENGINE_INCLUDE}/imgui/examples/imgui_impl_glfw.h
    ${ENGINE_INCLUDE}/imgui/examples/imgui_impl_opengl3.h
    ${ENGINE_INCLUDE}/imgui/examples/imgui_impl_opengl3.cpp
)

set(3D_Renderer_assimp
    ${ENGINE_INCLUDE}/assimp/aabb.h
    ${ENGINE_INCLUDE}/assimp/ai_assert.h
    ${ENGINE_INCLUDE}/assimp/anim.h
    ${ENGINE_INCLUDE}/assimp/BaseImporter.h
    ${ENGINE_INCLUDE}/assimp/Bitmap.h
    ${ENGINE_INCLUDE}/assimp/BlobIOSystem.h
    ${ENGINE_INCLUDE}/assimp/ByteSwapper.h
    ${ENGINE_INCLUDE}/assimp/camera.h
    ${ENGINE_INCLUDE}/assimp/cexport.h
    ${ENGINE_INCLUDE}/assimp/cfileio.h
    ${ENGINE_INCLUDE}/assimp/cimport.h
    ${ENGINE_INCLUDE}/assimp/color4.h
    ${ENGINE_INCLUDE}/assimp/color4.inl
    ${ENGINE_INCLUDE}/assimp/config.h
    ${ENGINE_INCLUDE}/assimp/config.h.in
    ${ENGINE_INCLUDE}/assimp/CreateAnimMesh.h
    ${ENGINE_INCLUDE}/assimp/DefaultIOStream.h
    ${ENGINE_INCLUDE}/assimp/DefaultIOSystem.h
    ${ENGINE_INCLUDE}/assimp/DefaultLogger.hpp
    ${ENGINE_INCLUDE}/assimp/Defines.h
    ${ENGINE_INCLUDE}/assimp/defs.h
    ${ENGINE_INCLUDE}/assimp/Exceptional.h
    ${ENGINE_INCLUDE}/assimp/Exporter.hpp
    ${ENGINE_INCLUDE}/assimp/fast_atof.h
    ${ENGINE_INCLUDE}/assimp/GenericProperty.h
    ${ENGINE_INCLUDE}/assimp/Hash.h
    ${ENGINE_INCLUDE}/assimp/Importer.hpp
    ${ENGINE_INCLUDE}/assimp/importerdesc.h
    ${ENGINE_INCLUDE}/assimp/IOStream.hpp
    ${ENGINE_INCLUDE}/assimp/IOStreamBuffer.h
    ${ENGINE_INCLUDE}/assimp/IOSystem.hpp
    ${ENGINE_INCLUDE}/assimp/irrXMLWrapper.h
    ${ENGINE_INCLUDE}/assimp/light.h
    ${ENGINE_INCLUDE}/assimp/LineSplitter.h
    ${ENGINE_INCLUDE}/assimp/LogAux.h
    ${ENGINE_INCLUDE}/assimp/Logger.hpp
    ${ENGINE_INCLUDE}/assimp/LogStream.hpp
    ${ENGINE_INCLUDE}/assimp/Macros.h
    ${ENGINE_INCLUDE}/assimp/material.h
    ${ENGINE_INCLUDE}/assimp/material.inl
    ${ENGINE_INCLUDE}/assimp/MathFunctions.h
    ${ENGINE_INCLUDE}/assimp/matrix3x3.h
    ${ENGINE_INCLUDE}/assimp/matrix3x3.inl
    ${ENGINE_INCLUDE}/assimp/matrix4x4.h
    ${ENGINE_INCLUDE}/assimp/matrix4x4.inl
    ${ENGINE_INCLUDE}/assimp/MemoryIOWrapper.h
    ${ENGINE_INCLUDE}/assimp/mesh.h
    ${ENGINE_INCLUDE}/assimp/metadata.h
    ${ENGINE_INCLUDE}/assimp/NullLogger.hpp
    ${ENGINE_INCLUDE}/assimp/ParsingUtils.h
    ${ENGINE_INCLUDE}/assimp/pbrmaterial.h
    ${ENGINE_INCLUDE}/assimp/postprocess.h
    ${ENGINE_INCLUDE}/assimp/Profiler.h
    ${ENGINE_INCLUDE}/assimp/ProgressHandler.hpp
    ${ENGINE_INCLUDE}/assimp/qnan.h
    ${ENGINE_INCLUDE}/assimp/quaternion.h
    ${ENGINE_INCLUDE}/assimp/quaternion.inl
    ${ENGINE_INCLUDE}/assimp/RemoveComments.h
    ${ENGINE_INCLUDE}/assimp/scene.h
    ${ENGINE_INCLUDE}/assimp/SceneCombiner.h
    ${ENGINE_INCLUDE}/assimp/SGSpatialSort.h
    ${ENGINE_INCLUDE}/assimp/SkeletonMeshBuilder.h
    ${ENGINE_INCLUDE}/assimp/SmoothingGroups.h
    ${ENGINE_INCLUDE}/assimp/SmoothingGroups.inl
    ${ENGINE_INCLUDE}/assimp/SpatialSort.h
    ${ENGINE_INCLUDE}/assimp/StandardShapes.h
    ${ENGINE_INCLUDE}/assimp/StreamReader.h
    ${ENGINE_INCLUDE}/assimp/StreamWriter.h
    ${ENGINE_INCLUDE}/assimp/StringComparison.h
    ${ENGINE_INCLUDE}/assimp/StringUtils.h
    ${ENGINE_INCLUDE}/assimp/Subdivision.h
    ${ENGINE_INCLUDE}/assimp/texture.h
    ${ENGINE_INCLUDE}/assimp/TinyFormatter.h
    ${ENGINE_INCLUDE}/assimp/types.h
    ${ENGINE_INCLUDE}/assimp/vector2.h
    ${ENGINE_INCLUDE}/assimp/vector2.inl
    ${ENGINE_INCLUDE}/assimp/vector3.h
    ${ENGINE_INCLUDE}/assimp/vector3.inl
    ${ENGINE_INCLUDE}/assimp/version.h
    ${ENGINE_INCLUDE}/assimp/Vertex.h
    ${ENGINE_INCLUDE}/assimp/XMLTools.h
    ${ENGINE_INCLUDE}/assimp/ZipArchiveIOSystem.h
)

message(STATUS "stb_image: ${ENGINE_INCLUDE}/stb_image.h")

set(3D_Renderer_FILES
    ${3D_Renderer_assimp}
    ${3D_Renderer_imgui}
    ${ENGINE_INCLUDE}/stb_image.h
    ${ENGINE_INCLUDE}/glad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/glad.c
    ${ENGINE_INCLUDE}/KHR/khrplatform.h
    ${3D_Renderer_core_SOURCES}
    ${3D_Renderer_core_HEADERS}
)

# Add the 3D_Renderer library to the project
message(STATUS "-- Adding Core Engine Library")
add_library(3D_Renderer ${3D_Renderer_FILES})

target_include_directories(3D_Renderer PUBLIC
    ${ENGINE_INCLUDE}
)

target_link_libraries(3D_Renderer PRIVATE
    ${ENGINE_LIB}/glfw3.lib
    ${ENGINE_LIB}/assimp-vc142-mtd.lib
)

# target_compile_options(3D_Renderer PRIVATE --std=c++17)

add_executable(3D_Renderer_exe ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp ${3D_Renderer_FILES})

target_include_directories(3D_Renderer_exe PUBLIC
    ${ENGINE_INCLUDE}
)

target_link_libraries(3D_Renderer_exe PRIVATE
    ${ENGINE_LIB}/glfw3.lib
    ${ENGINE_LIB}/assimp-vc142-mtd.lib
)