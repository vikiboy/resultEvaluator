cmake_minimum_required (VERSION 2.6)

PROJECT (evaluation)

SET (NEEDED_LIBS resultEvaluator)

ADD_EXECUTABLE (${PROJECT_NAME} main.cpp)

target_link_libraries (${PROJECT_NAME} ${NEEDED_LIBS})
