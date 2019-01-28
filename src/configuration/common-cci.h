/*
 * @file common-cci.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Common CCI header
 */

#ifndef HV_CONFIGURATION_COMMON_CCI_H
#define HV_CONFIGURATION_COMMON_CCI_H

#include "common.h"

// TODO: remove me
// #define HV_CCI_CALLBACK_TYPES
// #include "../param/cci/param-cci-callbacks.h"
// #include "cci_core/cci_meta.h"
// #include "../broker/cci/broker-cci-callbacks.h"

#include <cci_configuration>

HV_CONFIGURATION_OPEN_NAMESPACE

template<class T>
class CCICallbackObject {
public:
	CCICallbackObject(T callback, const ::cci::cci_originator& originator):
			callback(callback), originator(originator) {

	}
	T callback;
	::cci::cci_originator originator;
};

::cci::cci_broker_handle findBrokerConvenience(const ::cci::cci_originator& originator);

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_COMMON_CCI_H
