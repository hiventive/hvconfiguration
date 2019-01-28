/*
 * @file broker-cci-if-helper.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief CCI broker interface helper
 */

#ifndef HV_CONFIGURATION_BROKER_CCI_IF_HELPER_H
#define HV_CONFIGURATION_BROKER_CCI_IF_HELPER_H

#include "../../configuration/common.h"
#include "../../configuration/common-cci.h"

HV_CONFIGURATION_OPEN_NAMESPACE

class BrokerCCIIfHelper : public ::cci::cci_broker_if {
public:
	/// @copydoc cci_broker_if::name
	inline const char* name() const override {
		return CCIName();
	}

	/// FIXME: doc
	virtual const char* CCIName() const = 0;
};

HV_CONFIGURATION_CLOSE_NAMESPACE

// #include "broker-cci-if-helper.hpp"  // FIXME : duplicate symbol

#endif // HV_CONFIGURATION_BROKER_CCI_IF_HELPER_H
