#pragma once


#include <cassert>
#include <iostream>

#define ASSERT(expr, message) \
do { \
    if (!(expr)) { \
        std::cerr << "ASSERTION FAILED: " << message << std::endl; \
        assert(expr); \
    } \
} while (false)


