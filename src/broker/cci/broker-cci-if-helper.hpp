/*
 * @file broker-cci-if-helper.hpp
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief CCI broker interface helper implementation
 */

#include "broker-cci-if-helper.h"

#ifndef HV_CONFIGURATION_BROKER_CCI_IF_HELPER_IMPL_H
#define HV_CONFIGURATION_BROKER_CCI_IF_HELPER_IMPL_H

HV_CONFIGURATION_OPEN_NAMESPACE

const char* BrokerCCIIfHelper::name() const {
	return CCIName();
}

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_BROKER_CCI_IF_HELPER_IMPL_H
