/*
 * @file broker.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Broker implementation
 */

#ifndef HV_CONFIGURATION_BROKER_H
#define HV_CONFIGURATION_BROKER_H

#include "../configuration/common.h"
#include "base/broker-base.h"
#include "cci/broker-cci.h"

HV_CONFIGURATION_OPEN_NAMESPACE

class Broker : public BrokerBase {
public:
	explicit Broker(const std::string& name, bool registerCCI = true);

	Broker(const std::string& name,
			StorageIf* storage,
			bool registerCCI = true);

	~Broker() override;

	explicit operator ::cci::cci_broker_if* ();

	operator ::cci::cci_broker_if& ();

	::cci::cci_broker_if& getCCIBroker();

private:
	BrokerCCI brokerCCI;
};

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_BROKER_H
