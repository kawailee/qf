cmake_minimum_required (VERSION 3.10)

include(CMakeForceCompiler)
set( CMAKE_C_COMPILER /opt/rocm/llvm/bin/clang )
set( CMAKE_CXX_COMPILER /opt/rocm/llvm/bin/clang++ )
set( CMAKE_INCLUDE_CURRENT_DIR ON )

project( qf )

add_executable( coupon_date coupon_date.cpp)

find_package(QuantLib REQUIRED )

target_link_libraries(coupon_date QuantLib::QuantLib)

set_property(TARGET coupon_date PROPERTY CXX_STANDARD 17 )


