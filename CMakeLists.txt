cmake_minimum_required(VERSION 3.23)
set(PROJECT_NAME proyectoFinal)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 14)

set(GLEW_LOCATION      ../libraries2122/glew)
set(GLFW_LOCATION      ../libraries2122/glfw)
set(GLM_LOCATION       ../libraries2122/glm)
set(ASSIMP_LOCATION    ../libraries2122/assimp)
set(FREEIMAGE_LOCATION ../libraries2122/freeimage)

include_directories(src ${HEADERS_LOCATION} ${GLEW_LOCATION}/include ${GLFW_LOCATION}/include ${GLM_LOCATION}/include ${ASSIMP_LOCATION}/include ${FREEIMAGE_LOCATION}/include)
link_directories(${LIBS_LOCATION} ${GLEW_LOCATION}/lib ${GLFW_LOCATION}/lib ${GLM_LOCATION}/lib ${ASSIMP_LOCATION}/lib ${FREEIMAGE_LOCATION}/lib)

include_directories(src ${HEADERS_LOCATION})
link_directories(${LIBS_LOCATION})

file (GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} opengl32 glu32 glew32 glfw3 assimp freeimage)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG binary)
