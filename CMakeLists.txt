cmake_minimum_required(VERSION 3.20)

# Nombre del proyecto
project(CapacitatedFacilityLocationProblem VERSION 1.0)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Incluir el directorio de encabezados
include_directories(include)

# Añadir subdirectorios
add_subdirectory(src)
add_subdirectory(tests)

# Copiar directorio de instancias a la carpeta de compilación
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/instances DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Generar documentación con Doxygen
find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    add_custom_target(doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
endif()