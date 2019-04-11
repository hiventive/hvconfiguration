/*
 * @file broker.cpp
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Broker implementation
 */

#include "../configuration/common.h"
#include "broker.h"

HV_CONFIGURATION_OPEN_NAMESPACE

Broker::Broker(const std::string& name,
		bool registerCCI) :
	BrokerBase(name),
	brokerCCI(*this, nullptr, registerCCI) {
	_registerGlobalBroker(this);
}

Broker::Broker(const std::string& name,
		StorageIf* storage,
		bool registerCCI) :
	BrokerBase(name, storage),
	brokerCCI(*this, storage, registerCCI) {
	_registerGlobalBroker(this);
}

::cci::cci_broker_if& Broker::getCCIBroker() {
	return brokerCCI;
}

Broker::operator ::cci::cci_broker_if*() {
	return &brokerCCI;
}

Broker::operator ::cci::cci_broker_if& () {
	return brokerCCI;
}

Broker::~Broker() {
	_unregisterGlobalBroker();
}

HV_CONFIGURATION_CLOSE_NAMESPACE
