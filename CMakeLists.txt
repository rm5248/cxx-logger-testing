project(Log_Testing)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(log_testing
    logger.h logger.cpp
    logevent.h logevent.cpp
	main.cpp
)
target_link_libraries(log_testing PRIVATE -lstdc++exp)
