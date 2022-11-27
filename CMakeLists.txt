cmake_minimum_required(VERSION 3.21)

project(SkyrimScripting.Stop VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DEPLOY_MOD "Copy mod .dll and resource into %SKYRIM_MODS_DEPLOY_ROOT%\\${PROJECT_NAME}" OFF)

# Override <DEPLOY_ROOT> to set the path of a mod to deploy files to.
# The SKSE plugin files will be writen to <DEPLOY_ROOT>/<PROJECT_NAME>/SKSE/Plugins
# You can use the environment variable SKYRIM_MODS_DEPLOY_ROOT to set a default <DEPLOY_ROOT>
# set(DEPLOY_ROOT "C:/some/path/to/mod/path")

set(POST_LOAD_PROJECT SkyrimScripting.Stop.PostLoad)
find_package(CommonLibSSE CONFIG REQUIRED)
add_commonlibsse_plugin(${POST_LOAD_PROJECT} SOURCES Stop_PostLoad.cpp)
target_compile_features(${POST_LOAD_PROJECT} PRIVATE cxx_std_23)
target_precompile_headers(${POST_LOAD_PROJECT} PUBLIC PCH.h)
set_target_properties(${POST_LOAD_PROJECT} PROPERTIES OUTPUT_NAME "~~~~.SkyrimScripting.Stop.PostLoad")

set(DATA_LOADED_PROJECT SkyrimScripting.Stop.DataLoaded)
find_package(CommonLibSSE CONFIG REQUIRED)
add_commonlibsse_plugin(${DATA_LOADED_PROJECT} SOURCES Stop_DataLoaded.cpp)
target_compile_features(${DATA_LOADED_PROJECT} PRIVATE cxx_std_23)
target_precompile_headers(${DATA_LOADED_PROJECT} PUBLIC PCH.h)
set_target_properties(${DATA_LOADED_PROJECT} PROPERTIES OUTPUT_NAME "~~~~.SkyrimScripting.Stop.DataLoaded")

if(DEPLOY_MOD)
	if(EXISTS "${DEPLOY_ROOT}")

        set(POST_LOAD_MOD_FOLDER "${DEPLOY_ROOT}/${POST_LOAD_PROJECT}")
        set(POST_LOAD_DLL_FOLDER "${POST_LOAD_MOD_FOLDER}/SKSE/Plugins")
        message(STATUS "Post Load plugin output directory: ${POST_LOAD_MOD_FOLDER}")
        add_custom_command(
            TARGET "${POST_LOAD_PROJECT}"
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E make_directory    "${POST_LOAD_DLL_FOLDER}"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${POST_LOAD_PROJECT}>"     "${POST_LOAD_DLL_FOLDER}/$<TARGET_FILE_NAME:${POST_LOAD_PROJECT}>"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${POST_LOAD_PROJECT}>" "${POST_LOAD_DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${POST_LOAD_PROJECT}>"
            VERBATIM
        )

        set(DATA_LOADED_MOD_FOLDER "${DEPLOY_ROOT}/${DATA_LOADED_PROJECT}")
        set(DATA_LOADED_DLL_FOLDER "${DATA_LOADED_MOD_FOLDER}/SKSE/Plugins")
        message(STATUS "Post Load plugin output directory: ${DATA_LOADED_MOD_FOLDER}")
        add_custom_command(
            TARGET "${DATA_LOADED_PROJECT}"
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E make_directory    "${DATA_LOADED_DLL_FOLDER}"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${DATA_LOADED_PROJECT}>"     "${DATA_LOADED_DLL_FOLDER}/$<TARGET_FILE_NAME:${DATA_LOADED_PROJECT}>"
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${DATA_LOADED_PROJECT}>" "${DATA_LOADED_DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${DATA_LOADED_PROJECT}>"
            VERBATIM
        )

    endif()
endif()
