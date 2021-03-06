cmake_minimum_required(VERSION 3.0.0)
project(OpenPlotLib VERSION 0.1.0)

# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_CXX_STANDARD 17)

# if ( CMAKE_COMPILER_IS_GNUCC )
#         set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
# endif()
# if ( MSVC )
# set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
# endif()

include(CTest)
enable_testing()

SET(OPENGL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLEW/include ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/include)
SET(OPENGL_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLEW/lib/glew32s.lib ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/GLFW/lib-v2022/glfw3.lib Opengl32)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
# set(Boost_COMPILER "-vc143")
set(Boost_ARCHITECTURE "-x64")

cmake_policy(SET CMP0074 NEW)
find_package(Boost 1.78.0 COMPONENTS python37 program_options REQUIRED)
add_definitions(-DBOOST_PYTHON_STATIC_LIB)

find_package(Python 3.7 COMPONENTS Development REQUIRED)

add_executable(OpenPlotLibTest src/Application.cpp )
target_sources(OpenPlotLibTest
    PRIVATE
        src/Plots/Plot.h
        src/Plots/_2DPlot.cpp
        src/Plots/ScatterPlot.cpp
        src/GL/GUIManager.cpp
        src/GL/guimanager.h
        src/GL/VertexBuffer.h
        src/GL/VertexBuffer.cpp
        src/GL/shader.h
        src/GL/shader.cpp
        src/GL/program.h
        src/GL/program.cpp
        src/shaders.h
)
target_compile_definitions(OpenPlotLibTest PRIVATE GLEW_STATIC)
target_include_directories(OpenPlotLibTest PRIVATE ${Boost_INCLUDE_DIRS} ${Python_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
target_link_libraries(OpenPlotLibTest ${Boost_LIBRARIES} ${Python_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(SierpinskiApp src/SierpinskiApp.cpp )
target_sources(SierpinskiApp
    PRIVATE
        src/Plots/Plot.h
        src/Plots/_2DPlot.cpp
        src/Plots/SierpinskiPlot.cpp
        src/GL/GUIManager.cpp
        src/GL/guimanager.h
        src/GL/VertexBuffer.h
        src/GL/VertexBuffer.cpp
        src/GL/shader.h
        src/GL/shader.cpp
        src/GL/program.h
        src/GL/program.cpp
        src/shaders.h
)
target_compile_definitions(SierpinskiApp PRIVATE GLEW_STATIC)
target_include_directories(SierpinskiApp PRIVATE ${Boost_INCLUDE_DIRS} ${Python_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
target_link_libraries(SierpinskiApp ${Boost_LIBRARIES} ${Python_LIBRARIES} ${OPENGL_LIBRARIES})

add_executable(BarnsleyFernApp src/BarnsleyFernApp.cpp )
target_sources(BarnsleyFernApp
    PRIVATE
        src/Plots/Plot.h
        src/Plots/_2DPlot.cpp
        src/Plots/BarnsleyFernPlot.cpp
        src/GL/GUIManager.cpp
        src/GL/guimanager.h
        src/GL/VertexBuffer.h
        src/GL/VertexBuffer.cpp
        src/GL/shader.h
        src/GL/shader.cpp
        src/GL/program.h
        src/GL/program.cpp
        src/shaders.h
)
target_compile_definitions(BarnsleyFernApp PRIVATE GLEW_STATIC)
target_include_directories(BarnsleyFernApp PRIVATE ${Boost_INCLUDE_DIRS} ${Python_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
target_link_libraries(BarnsleyFernApp ${Boost_LIBRARIES} ${Python_LIBRARIES} ${OPENGL_LIBRARIES})

# add_library(OpenPlotLib MODULE src/OpenPlotLib.cpp )
# target_sources(OpenPlotLib
#     PRIVATE
#         src/GUIManager.cpp
#         src/GUIManager.h
#         src/ReflectRNG/Circle.cpp
#         src/ReflectRNG/Ray.cpp
#         src/ReflectRNG/Scene.cpp
#         src/ReflectRNG/Scene.h
#         src/Plots/Plot.h
#         src/Plots/_2DPlot.cpp
#         src/Plots/ScatterPlot.cpp
#         src/shaders.h
# )
# target_compile_definitions(OpenPlotLib PRIVATE GLEW_STATIC)
# target_include_directories(OpenPlotLib PRIVATE ${Boost_INCLUDE_DIRS} ${Python_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIRS})
# target_link_libraries(OpenPlotLib ${Boost_LIBRARIES} ${Python_LIBRARIES} ${OPENGL_LIBRARIES})
# set_target_properties(OpenPlotLib
#         PROPERTIES PREFIX ""
#                 OUTPUT_NAME "OpenPlotLib"
#                 SUFFIX ".pyd"
# )

# add_custom_command(TARGET OpenPlotLib PRE_BUILD 
#         COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/create_shader.cmake)
add_custom_command(TARGET OpenPlotLibTest PRE_BUILD 
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/create_shader.cmake)
add_custom_command(TARGET SierpinskiApp PRE_BUILD 
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/create_shader.cmake)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
