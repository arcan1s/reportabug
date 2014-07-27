# set files
file (GLOB SOURCES *.cpp)
file (GLOB HEADERS *.h)
file (GLOB FORMS *.ui)

# include_path
include_directories (${CMAKE_CURRENT_BINARY_DIR}/../
                     ${CMAKE_SOURCE_DIR}
                     ${CMAKE_BINARY_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR})

if (USE_QT5)
    find_package(Qt5Core REQUIRED)
    find_package(Qt5Widgets REQUIRED)
    add_definitions(${Qt5Core_DEFINITIONS})
    add_definitions(${Qt5Widgets_DEFINITIONS})
    add_definitions(${Qt5LinguistTools_DEFINITIONS})
    qt5_wrap_cpp (MOC_SOURCES ${HEADERS})
    qt5_wrap_ui (UI_HEADERS ${FORMS})

    source_group ("Header Files" FILES ${HEADERS})
    source_group ("Source Files" FILES ${SOURCES})
    source_group ("Generated Files" FILES ${MOC_SOURCES})

    include_directories (${Qt5Core_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
    add_executable (${SUBPROJECT} ${UI_HEADERS} ${HEADERS} ${SOURCES} ${MOC_SOURCES})
    target_link_libraries (${SUBPROJECT} ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES})
else ()
    find_package (Qt4 REQUIRED)
    include (${QT_USE_FILE})
    qt4_wrap_cpp (MOC_SOURCES ${HEADERS})
    qt4_wrap_ui (UI_HEADERS ${FORMS})

    source_group ("Header Files" FILES ${HEADERS})
    source_group ("Source Files" FILES ${SOURCES})
    source_group ("Generated Files" FILES ${MOC_SOURCES})

    add_executable (${SUBPROJECT} ${UI_HEADERS} ${HEADERS} ${SOURCES} ${MOC_SOURCES})
    target_link_libraries (${SUBPROJECT} ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})
endif()

# install properties
install (TARGETS ${SUBPROJECT} DESTINATION bin)
