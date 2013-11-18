cmake_minimum_required(VERSION 2.6)
project(GLDemo)
add_definitions(-std=c++11)

#Make GLSW library
set(GLSW_loc ${CMAKE_SOURCE_DIR}/lib/glsw)
set(GLSW_src ${GLSW_loc}/bstrlib.c ${GLSW_loc}/glsw.c)
add_library(glsw ${GLSW_src})

#Make SOIL library
set(SOIL_loc ${CMAKE_SOURCE_DIR}/lib/SOIL/src)
set(SOIL_src ${SOIL_loc}/image_DXT.c ${SOIL_loc}/image_helper.c ${SOIL_loc}/SOIL.c ${SOIL_loc}/stb_image_aug.c)
add_library(SOIL ${SOIL_src})

#Make GLDemo library
set(GLDemo_src src/Camera.cpp src/Entity.cpp src/Mesh.cpp src/Shader.cpp src/Text.cpp src/Texture.cpp)
add_library(GLDemo ${GLDemo_src})
target_link_libraries(GLDemo GL GLEW openctm glsw SOIL glut)
include_directories(${GLSW_loc} ${SOIL_loc} ${CMAKE_SOURCE_DIR}/src)

#Make Silhouette demo
set(Silhouette_src examples/Silhouette/Demo.cpp)
add_executable(Silhouette ${Silhouette_src})
target_link_libraries(Silhouette GLDemo)

#Make QuarticFlame demo
set(QuarticFlame_src examples/QuarticFlame/Demo.cpp examples/QuarticFlame/QuarticFlame.cpp)
add_executable(QuarticFlame ${QuarticFlame_src})
target_link_libraries(QuarticFlame GLDemo)
