
function(embed_resource resource_file_name source_file_name variable_name)

# if(EXISTS ${source_file_name})
#     if(${source_file_name} IS_NEWER_THAN ${resource_file_name})
#         return()
#     endif()
# endif()

message(${resource_file_name})

file(READ ${resource_file_name} str_content)

# string(REPEAT "[0-9a-f]" 32 pattern)
# string(REGEX REPLACE "(${pattern})" "\\1\n" content ${str_content})
# string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1, " content ${content})
# string(REGEX REPLACE ", $" "" content ${content})
# string(REGEX REPLACE ";" "\;" str_content "${str_content}")

set(source "    { \"${variable_name}\", R\"\"\"\"\(${str_content}\)\"\"\"\" },\n")
# set(source "// Auto generated file.\n${array_definition}\n")
file(APPEND ${source_file_name} "${source}")

endfunction()



SET(SHADER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../src/shaders.h)
SET(SHADER_FILE_START "#include <string>\n\
#include <map>\n\
\n\
static const std::map<std::string, std::string> shaders = {\n")
SET(SHADER_FILE_END "}\;")

if(
    NOT EXISTS ${SHADER_FILE} OR
    ${CMAKE_CURRENT_SOURCE_DIR}/../res/shaders/scatter_plot_shaders/points_fragment.frag IS_NEWER_THAN ${SHADER_FILE} OR
    ${CMAKE_CURRENT_SOURCE_DIR}/../res/shaders/scatter_plot_shaders/points_vertex.vert IS_NEWER_THAN ${SHADER_FILE} OR
    ${CMAKE_CURRENT_SOURCE_DIR}/../res/shaders/empty.vert IS_NEWER_THAN ${SHADER_FILE} OR
    ${CMAKE_CURRENT_SOURCE_DIR}/../res/shaders/empty.frag IS_NEWER_THAN ${SHADER_FILE}
)
    file(WRITE ${SHADER_FILE} ${SHADER_FILE_START})
    embed_resource(${CMAKE_CURRENT_SOURCE_DIR}/../res/shaders/scatter_plot_shaders/points_fragment.frag ${SHADER_FILE} scatter_plot_points_fragment)
    embed_resource(${CMAKE_CURRENT_SOURCE_DIR}/../res/shaders/scatter_plot_shaders/points_vertex.vert ${SHADER_FILE} scatter_plot_points_vertex)
    embed_resource(${CMAKE_CURRENT_SOURCE_DIR}/../res/shaders/empty.vert ${SHADER_FILE} empty_vertex)
    embed_resource(${CMAKE_CURRENT_SOURCE_DIR}/../res/shaders/empty.frag ${SHADER_FILE} empty_fragment)
    
    file(APPEND ${SHADER_FILE} ${SHADER_FILE_END})
endif()
