cmake_minimum_required (VERSION 3.10)

project(Tutorial04_Instancing CXX)

set(SOURCE
    src/Tutorial04_Instancing.cpp
    ../Common/src/TexturedCube.cpp
)

set(INCLUDE
    src/Tutorial04_Instancing.hpp
    ../Common/src/TexturedCube.hpp
)

set(SHADERS
    assets/cube_inst.vsh
    assets/cube_inst.psh
)

set(ASSETS
    assets/DGLogo.png
)

add_sample_app("Tutorial04_Instancing" "DiligentSamples/Tutorials" "${SOURCE}" "${INCLUDE}" "${SHADERS}" "${ASSETS}")
