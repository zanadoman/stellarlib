cmake_minimum_required(VERSION 3.29)
project(stellarlib VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_EXTENSIONS OFF)
include_directories(${CMAKE_SOURCE_DIR}/include)
file(READ ${CMAKE_SOURCE_DIR}/compile_flags.txt CMAKE_CXX_FLAGS)
string(REPLACE \n \  CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG -g\ -save-temps\ -Wa,-W\ -Wno-gnu-line-marker)
set(CMAKE_CXX_FLAGS_RELEASE -flto)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_program(CMAKE_CXX_CLANG_TIDY clang-tidy)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${SOURCES})
find_package(SDL3 REQUIRED)
target_link_libraries(${PROJECT_NAME} INTERFACE SDL3::SDL3)
if(WIN32)
	file(
		GLOB_RECURSE WIN32_RUNTIME_DLLS
		${CMAKE_FIND_ROOT_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}gcc*${CMAKE_SHARED_LIBRARY_SUFFIX}
		${CMAKE_FIND_ROOT_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}ssp*${CMAKE_SHARED_LIBRARY_SUFFIX}
		${CMAKE_FIND_ROOT_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}stdc++*${CMAKE_SHARED_LIBRARY_SUFFIX}
		${CMAKE_FIND_ROOT_PATH}/${CMAKE_SHARED_LIBRARY_PREFIX}winpthread*${CMAKE_SHARED_LIBRARY_SUFFIX}
	)
	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> ${WIN32_RUNTIME_DLLS}
		COMMAND_EXPAND_LISTS
	)
elseif(ANDROID)
	set(ANDROID_JNI_DIR ${CMAKE_SOURCE_DIR}/android/app/src/main/jniLibs)
	file(REMOVE_RECURSE ${ANDROID_JNI_DIR})
	set(ANDROID_JNI_DIR ${ANDROID_JNI_DIR}/${ANDROID_ABI})
	file(MAKE_DIRECTORY ${ANDROID_JNI_DIR})
	set(ANDROID_ASSETS_DIR ${CMAKE_SOURCE_DIR}/android/app/src/main/assets)
	file(REMOVE_RECURSE ${ANDROID_ASSETS_DIR})
endif()

file(GLOB_RECURSE ASSETS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/assets/*)
if(ANDROID)
	set(ASSETS_DIR ${ANDROID_ASSETS_DIR}/assets)
else()
	set(ASSETS_DIR ${CMAKE_BINARY_DIR}/assets)
endif()
add_custom_command(
	OUTPUT ${ASSETS_DIR}
	COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different ${CMAKE_SOURCE_DIR}/assets ${ASSETS_DIR}
	DEPENDS ${ASSETS}
)
add_custom_target(resources DEPENDS ${ASSETS_DIR})

file(GLOB_RECURSE EXAMPLES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/examples/*.cpp)
add_custom_target(examples)
if(ANDROID)
	add_custom_target(apks)
endif()
foreach(SOURCE ${EXAMPLES})
	get_filename_component(EXAMPLE ${SOURCE} NAME_WLE)
	if(ANDROID)
		add_library(${EXAMPLE} SHARED EXCLUDE_FROM_ALL ${SOURCE})
		target_link_options(${EXAMPLE} PRIVATE -uSDL_main)
	else()
		add_executable(${EXAMPLE} WIN32 EXCLUDE_FROM_ALL ${SOURCE})
		add_dependencies(${EXAMPLE} resources)
	endif()
	target_link_libraries(${EXAMPLE} PRIVATE ${PROJECT_NAME})
	if(WIN32)
		add_custom_command(
			TARGET ${EXAMPLE} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${EXAMPLE}> $<TARGET_RUNTIME_DLLS:${EXAMPLE}>
			COMMAND_EXPAND_LISTS
		)
	elseif(ANDROID)
		set(APK ${EXAMPLE}_apk)
		set_property(GLOBAL PROPERTY JOB_POOLS apk_pool=1)
		add_custom_command(
			OUTPUT ${CMAKE_BINARY_DIR}/${EXAMPLE}.apk
			COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL3::SDL3> ${ANDROID_JNI_DIR}
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				$<TARGET_FILE:${EXAMPLE}>
				${ANDROID_JNI_DIR}/${CMAKE_SHARED_LIBRARY_PREFIX}main${CMAKE_SHARED_LIBRARY_SUFFIX}
			COMMAND ${CMAKE_COMMAND} -E env
				ANDROID_APPLICATION_ID=org.stellarlib.${EXAMPLE}
				ANDROID_PLATFORM=${ANDROID_PLATFORM}
				ANDROID_VERSION_NAME=${PROJECT_VERSION}
				ANDROID_APP_NAME=${EXAMPLE}
				./gradlew assembleDebug
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
				${CMAKE_SOURCE_DIR}/android/app/build/outputs/apk/debug/app-debug.apk
				${CMAKE_BINARY_DIR}/${EXAMPLE}.apk
			DEPENDS ${EXAMPLE}
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/android
			JOB_POOL apk_pool
			COMMAND_EXPAND_LISTS
		)
		add_custom_target(${APK} DEPENDS ${CMAKE_BINARY_DIR}/${EXAMPLE}.apk)
		add_dependencies(${APK} resources)
		add_dependencies(apks ${APK})
	endif()
	add_dependencies(examples ${EXAMPLE})
endforeach()

find_package(GTest)
if(TARGET GTest::gtest_main)
	file(GLOB_RECURSE TESTS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/tests/*.cpp)
	foreach(SOURCE ${TESTS})
		get_filename_component(TEST ${SOURCE} NAME_WLE)
		set(TEST ${TEST}_test)
		add_executable(${TEST} EXCLUDE_FROM_ALL ${SOURCE})
		target_link_libraries(${TEST} PRIVATE GTest::gtest_main ${PROJECT_NAME})
		add_dependencies(${TEST} resources)
		if(WIN32)
			add_custom_command(
				TARGET ${TEST} POST_BUILD
				COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:tests> $<TARGET_RUNTIME_DLLS:tests>
				COMMAND_EXPAND_LISTS
			)
		endif()
	endforeach()
	add_executable(tests EXCLUDE_FROM_ALL ${TESTS})
	target_link_libraries(tests PRIVATE GTest::gtest_main ${PROJECT_NAME})
	add_dependencies(tests resources)
	if(WIN32)
		add_custom_command(
			TARGET tests POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:tests> $<TARGET_RUNTIME_DLLS:tests>
			COMMAND_EXPAND_LISTS
		)
	endif()
endif()

find_program(DOXYGEN doxygen)
if(DOXYGEN)
	file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME}/*.hpp)
	add_custom_command(
		OUTPUT ${CMAKE_BINARY_DIR}/html
		COMMAND ${CMAKE_COMMAND} -E env DOXYGEN_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR} ${DOXYGEN}
		DEPENDS ${CMAKE_SOURCE_DIR}/Doxyfile ${CMAKE_SOURCE_DIR}/README.md ${HEADERS} ${EXAMPLES}
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	)
	add_custom_target(docs ALL DEPENDS ${CMAKE_BINARY_DIR}/html)
endif()

include(GNUInstallDirs)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets)
install(EXPORT ${PROJECT_NAME}Targets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME} NAMESPACE ${PROJECT_NAME}::)
include(CMakePackageConfigHelpers)
configure_package_config_file(
	${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
	${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/licenses/${PROJECT_NAME})
install(DIRECTORY ${CMAKE_BINARY_DIR}/html DESTINATION ${CMAKE_INSTALL_DOCDIR} OPTIONAL)
