cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0)

include(CTest)
enable_testing()

set(INCLUDE_POPPLER "C:\\msys64\\mingw64\\include\\poppler\\cpp")
set(LIB_POPPLER "C:\\msys64\\mingw64\\lib\\libpoppler-cpp.dll.a")
set(INCLUDE_MINGW "C:\\msys64\\mingw64\\include")

set(OUTPUT_BULLET_PARSER ".\\..\\output\\BulletParser")

include_directories(SYSTEM ${INCLUDE_POPPLER})
include_directories(SYSTEM ${INCLUDE_MINGW})

file(GLOB BULLET_PARSER_SRCS ./src/BulletParser.cpp ./src/PDFTopicReader.cpp ./src/Deck.cpp)
add_executable(BulletParser ${BULLET_PARSER_SRCS})
target_link_libraries(BulletParser ${LIB_POPPLER})
set_target_properties(BulletParser PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_BULLET_PARSER}
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_BULLET_PARSER}
)

add_executable(main ./src/main.cpp)
target_link_libraries(main "C:\\msys64\\mingw64\\lib\\libpoppler-cpp.dll.a")
set_target_properties(main PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ".\\..\\output"
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ".\\..\\output"
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
