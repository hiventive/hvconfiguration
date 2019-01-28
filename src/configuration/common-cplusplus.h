/*
 * @file common-cplusplus.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date July, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Common C++ macros
 */

#if defined(_MSC_VER) && _MSC_VER <= 1700
	#define HV_CPLUSPLUS_MEMBER_FUNCTION_DEFAULT {}
	#define HV_CPLUSPLUS_MEMBER_FUNCTION_DELETE
#else
	#define HV_CPLUSPLUS_MEMBER_FUNCTION_DEFAULT = default
	#define HV_CPLUSPLUS_MEMBER_FUNCTION_DELETE = delete
#endif


