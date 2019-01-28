/*
 * @file common-cci.cpp
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive common CCI
 */

#include "common-cci.h"

HV_CONFIGURATION_OPEN_NAMESPACE

::cci::cci_broker_handle findBrokerConvenience(const ::cci::cci_originator& originator)
{
	if (!sc_core::sc_get_current_object()) {
		return ::cci::cci_get_global_broker(originator);
	} else {
		return ::cci::cci_get_broker();
	}
}

HV_CONFIGURATION_CLOSE_NAMESPACE
