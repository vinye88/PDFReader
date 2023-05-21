cmake_minimum_required(VERSION 3.0.0)
project(main VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(SYSTEM "C:\\msys64\\mingw64\\include\\poppler\\cpp")
include_directories(SYSTEM "C:\\msys64\\mingw64\\include\\")

file(GLOB SOURCES ./src/PDFTopicReader.cpp ./src/Deck.cpp)

add_executable(BulletParser ./src/BulletParser.cpp ${SOURCES})

target_link_libraries(BulletParser "C:\\msys64\\mingw64\\lib\\libpoppler-cpp.dll.a")

set_target_properties(BulletParser PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ".\\..\\output"
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ".\\..\\output"
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
