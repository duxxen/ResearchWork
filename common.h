#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#define IS_GMP_TEST 0

#if IS_GMP_TEST
#include "gmptest.h"
#else 
#include "ntltest.h"
#endif