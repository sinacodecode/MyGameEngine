find_package(Catch2 3 REQUIRED)

add_executable(engine_tests
	test_main.cpp
	test_renderer.cpp
	test_Transformations.cpp
)

target_link_libraries(engine_tests
	PRIVATE
		Engine
		Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(engine_tests)