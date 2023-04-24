project(harbour-raw-view CXX)
cmake_minimum_required(VERSION 3.5)

find_package(Qt5 COMPONENTS Core Qml Quick LinguistTools REQUIRED)

include(FindPkgConfig)
pkg_search_module(SAILFISH sailfishapp REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external)

add_custom_target(extract_libraw
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/external/LibRaw-0.21.1.tar.gz
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/external/include/libraw/libraw.h
    DEPENDS extract_libraw
    COMMAND ./configure --disable-openmp --prefix=${CMAKE_BINARY_DIR}/external
    COMMAND make -j 4
    COMMAND make install
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/LibRaw-0.21.1
)

set_target_properties(extract_libraw PROPERTIES ADDITIONAL_CLEAN_FILES
    ${CMAKE_BINARY_DIR}/external/LibRaw-0.21.1
    ${CMAKE_BINARY_DIR}/external/bin
    ${CMAKE_BINARY_DIR}/external/include
    ${CMAKE_BINARY_DIR}/external/lib
    ${CMAKE_BINARY_DIR}/external/share)

set(CMAKE_SKIP_RPATH OFF)
set(CMAKE_INSTALL_RPATH /usr/share/harbour-raw-view/lib)

add_executable(harbour-raw-view
    src/harbour-raw-view.cpp
    src/AppModel.hpp
    src/AppModel.cpp
    src/Image.hpp
    src/Image.cpp
    src/ImageItem.hpp
    src/ImageItem.cpp
    src/ImageListModel.hpp
    src/ImageListModel.cpp
    src/ImagePropertiesModel.hpp
    src/ImagePropertiesModel.cpp
    ${CMAKE_BINARY_DIR}/external/include/libraw/libraw.h
)
set_property(SOURCE ${CMAKE_BINARY_DIR}/external/include/libraw/libraw.h PROPERTY SKIP_AUTOMOC ON)
target_compile_definitions(harbour-raw-view PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    SAILJAIL
)
target_include_directories(harbour-raw-view PRIVATE
    $<BUILD_INTERFACE:
    ${SAILFISH_INCLUDE_DIRS}
>)
target_link_libraries(harbour-raw-view
    Qt5::Core Qt5::Quick
    ${SAILFISH_LDFLAGS}
    ${CMAKE_BINARY_DIR}/external/lib/libraw.so m z
)
target_include_directories(harbour-raw-view
    PRIVATE ${CMAKE_BINARY_DIR}/external/include
)

set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION "translations")
qt5_add_translation(QM_FILES ${TS_FILES})

install(FILES
    ${CMAKE_BINARY_DIR}/external/lib/libraw.so
    ${CMAKE_BINARY_DIR}/external/lib/libraw.so.23
    ${CMAKE_BINARY_DIR}/external/lib/libraw.so.23.0.0
    DESTINATION /usr/share/${CMAKE_PROJECT_NAME}/lib
    )
install(TARGETS harbour-raw-view
    RUNTIME DESTINATION bin
)
install(DIRECTORY qml
    DESTINATION share/harbour-raw-view
)
install(DIRECTORY translations
    DESTINATION share/harbour-raw-view
    FILES_MATCHING PATTERN "*.qm"
)
install(FILES harbour-raw-view.desktop
    DESTINATION share/applications
)
install(FILES icons/86x86/harbour-raw-view.png
    DESTINATION share/icons/hicolor/86x86/apps
)
install(FILES icons/108x108/harbour-raw-view.png
    DESTINATION share/icons/hicolor/108x108/apps
)
install(FILES icons/128x128/harbour-raw-view.png
    DESTINATION share/icons/hicolor/128x128/apps
)
install(FILES icons/172x172/harbour-raw-view.png
    DESTINATION share/icons/hicolor/172x172/apps
)

# Get the other files reachable from the project tree in Qt Creator
FILE(GLOB TsFiles "translations/*.ts")
add_custom_target(distfiles
    SOURCES
        harbour-raw-view.desktop
        qml/harbour-raw-view.qml
        qml/cover/CoverPage.qml
        qml/pages/MainPage.qml
        qml/pages/SecondPage.qml
        rpm/harbour-raw-view.changes.in
        rpm/harbour-raw-view.changes.run.in
        rpm/harbour-raw-view.spec
        rpm/harbour-raw-view.yaml
        ${TsFiles})

# Tell Qt Creator where the application executable(s) would be located on the
# device.
#
# It is not necessary to list other deployables than executables (runtime
# targets) here. The deployment process of Sailfish OS projects is opaque to
# Qt Creator and the information contained in QtCreatorDeployment.txt is only
# used to locate the executable associated with the active run configuration
# on the device in order to run it.
#
# Search the Qt Creator Manual to learn about the QtCreatorDeployment.txt file
# format.
file(WRITE "${CMAKE_BINARY_DIR}/QtCreatorDeployment.txt"
    "${CMAKE_INSTALL_PREFIX}
${CMAKE_BINARY_DIR}/harbour-raw-view:bin
")
