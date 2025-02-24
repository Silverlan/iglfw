include(${CMAKE_SOURCE_DIR}/cmake/pr_common.cmake)

set(PROJ_NAME iglfw)
pr_add_library(${PROJ_NAME} SHARED)

pr_add_dependency(${PROJ_NAME} vfilesystem TARGET PUBLIC)
pr_add_dependency(${PROJ_NAME} glfw TARGET PUBLIC)

pr_init_module(${PROJ_NAME})

pr_add_compile_definitions(
	${PROJ_NAME}
		-DGLFW_DLL
		-DDLLGLFW_EX
)
pr_add_compile_definitions(${PROJ_NAME} -DGLFW_INCLUDE_NONE PUBLIC)

pr_finalize(${PROJ_NAME})
