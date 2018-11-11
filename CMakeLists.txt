cmake_minimum_required( VERSION 3.8 FATAL_ERROR )
project( lala VERSION 0.0.1 LANGUAGES CXX )

# build settings:

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Release FORCE )
endif()

set( CMAKE_CXX_STANDARD 14 )
set( CXX_STANDARD_REQUIRED ON )
set( CXX_EXTENSIONS OFF )
set( CMAKE_CXX_FLAGS_RELEASE -O3 )

# 3rd party dependencies:

set( Boost_USE_STATIC_LIBS ON )
find_package( Boost 1.67 REQUIRED )

# executable:

add_executable( 
    lala
    src/main.cpp )

target_link_libraries( lala 
    PRIVATE 
    Boost::boost )