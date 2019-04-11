/*
 * @file common.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Common headers
 */

#ifndef HV_CONFIGURATION_COMMON_H
#define HV_CONFIGURATION_COMMON_H

#include <hv/common/cplusplus.h>
#include <hv/common/log.h>
#include <hv/common/hvutils.h>

// HV Configuration namespace
#define HV_CONFIGURATION_OPEN_NAMESPACE namespace hv { \
	namespace cfg {

#define HV_CONFIGURATION_CLOSE_NAMESPACE } }

#endif // HV_CONFIGURATION_COMMON_H
