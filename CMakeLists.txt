cmake_minimum_required(VERSION 3.18)

project(harbour-raw-view VERSION 0.4.0 LANGUAGES CXX)

find_package(Qt5 COMPONENTS Core Qml Quick LinguistTools REQUIRED)

include(FindPkgConfig)
pkg_search_module(SAILFISH sailfishapp REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/external)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/translations)

add_custom_target(extract_libraw
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/external/LibRaw-0.21.1.tar.gz
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/external/include/libraw/libraw.h
    DEPENDS extract_libraw
    COMMAND ./configure --disable-openmp --prefix=${CMAKE_BINARY_DIR}/external
    COMMAND make
    COMMAND make install
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/LibRaw-0.21.1
)

set_target_properties(extract_libraw PROPERTIES ADDITIONAL_CLEAN_FILES
    ${CMAKE_BINARY_DIR}/external/LibRaw-0.21.1
    ${CMAKE_BINARY_DIR}/external/bin
    ${CMAKE_BINARY_DIR}/external/include
    ${CMAKE_BINARY_DIR}/external/lib
    ${CMAKE_BINARY_DIR}/external/share)

add_custom_target(extract_libexiv2
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_SOURCE_DIR}/external/exiv2-0.28.0.tar.gz
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/external/include/exiv2/exiv2.hpp
    DEPENDS extract_libexiv2
    COMMAND rm -rf build
    COMMAND mkdir build
    COMMAND cmake -DCMAKE_INSTALL_LIBDIR=lib -DBUILD_SHARED_LIBS=Off -DCMAKE_POSITION_INDEPENDENT_CODE=On -DEXIV2_ENABLE_XMP=Off -DBUILD_WITH_STACK_PROTECTOR=Off -DEXIV2_ENABLE_BROTLI=Off -DEXIV2_ENABLE_INIH=Off -DEXIV2_BUILD_EXIV2_COMMAND=Off -DEXIV2_ENABLE_VIDEO=Off -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/external -B build -S .
    COMMAND cmake --build build
    COMMAND cmake --build build --target install
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/external/exiv2-0.28.0
)

set(CMAKE_SKIP_RPATH OFF)
set(CMAKE_INSTALL_RPATH /usr/share/harbour-raw-view/lib)

set(QML_FILES
    qml/harbour-raw-view.qml
    qml/pages/MainPage.qml
    qml/pages/About.qml
    qml/cover/CoverPage.qml
    qml/components/ThirdPartyComponent.qml
    )

set(TS_FILES
    translations/harbour-raw-view.ts
    translations/harbour-raw-view-de.ts)
list(TRANSFORM TS_FILES REPLACE "ts" "qm" OUTPUT_VARIABLE QM_FILES)
list(TRANSFORM QM_FILES PREPEND ${CMAKE_BINARY_DIR}/)

configure_file(src/Version.h.in ${CMAKE_BINARY_DIR}/src/Version.h @ONLY)

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
    ${CMAKE_BINARY_DIR}/external/include/exiv2/exiv2.hpp
)
set_property(SOURCE
        ${CMAKE_BINARY_DIR}/external/include/libraw/libraw.h
        ${CMAKE_BINARY_DIR}/external/include/exiv2/exiv2.hpp
    PROPERTY SKIP_AUTOMOC ON)
target_compile_definitions(harbour-raw-view PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
    SAILJAIL
)
target_include_directories(harbour-raw-view PRIVATE
    $<BUILD_INTERFACE:
    ${SAILFISH_INCLUDE_DIRS}
>
    ${CMAKE_BINARY_DIR}/src)
target_link_libraries(harbour-raw-view
    Qt5::Core Qt5::Quick
    ${SAILFISH_LDFLAGS}
    ${CMAKE_BINARY_DIR}/external/lib/libraw.so ${CMAKE_BINARY_DIR}/external/lib/libexiv2.a stdc++fs m z
)
target_include_directories(harbour-raw-view
    PRIVATE ${CMAKE_BINARY_DIR}/external/include
)
foreach(ts qm IN ZIP_LISTS TS_FILES QM_FILES)
    add_custom_command(OUTPUT ${qm}
        DEPENDS ${CMAKE_SOURCE_DIR}/${ts}
        COMMAND lrelease ${CMAKE_SOURCE_DIR}/${ts} -qm ${qm})
    install(FILES ${qm} DESTINATION share/harbour-raw-view/translations)
endforeach()
add_custom_target(generate_qm_files ALL DEPENDS ${QM_FILES})

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
        ${QML_FILES}
        ${TsFiles}
#        rpm/harbour-raw-view.changes.in
#        rpm/harbour-raw-view.changes.run.in
        rpm/harbour-raw-view.spec
#        rpm/harbour-raw-view.yaml
)

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
