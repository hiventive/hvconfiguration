/*
 * @file common.cpp
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date March, 2019
 * @copyright Copyright (C) 2019, Hiventive.
 *
 * @brief Common implementation
 */

#include "common.h"
#include "../broker/broker.h"

#include <systemc>

HV_CONFIGURATION_OPEN_NAMESPACE

// Private
Broker* _globalBroker;

Broker* getBroker() {
	return _globalBroker;
}

std::string getRelativeUniqueName(const std::string& name) {
	if(name.empty()) {
		HV_LOG_CRITICAL("Empty parameter name is not allowed");
		HV_EXIT_FAILURE();
	}

	// Hierarchical name
	std::string hierarchicalName = name;
	sc_core::sc_object* current_obj = sc_core::sc_get_current_object();
	for (sc_core::sc_process_handle current_proc(current_obj);
		 current_proc.valid();
		 current_proc = sc_core::sc_process_handle(current_obj)) {
		current_obj = current_proc.get_parent_object();
	}
	if(current_obj) {
		hierarchicalName = std::string(current_obj->name()) + sc_core::SC_HIERARCHY_CHAR + name;
	}

	// Unique name
	// SystemC >= 2.3.2 required
	if (!sc_core::sc_register_hierarchical_name(hierarchicalName.c_str())) {
		const char* new_name = sc_core::sc_gen_unique_name(hierarchicalName.c_str());
		sc_core::sc_register_hierarchical_name(new_name);
		HV_LOG_WARNING("{} is already used in the SystemC hierarchy, using {} instead", hierarchicalName, new_name);
		return sc_core::sc_get_hierarchical_name(new_name);
	}
	return sc_core::sc_get_hierarchical_name(hierarchicalName.c_str());
}

void _registerGlobalBroker(Broker* broker) {
	_globalBroker = broker;
}

void _unregisterGlobalBroker() {
	_globalBroker = nullptr;
}

void _registerParam(ParamIf* param) {
	if(_globalBroker) {
		_globalBroker->addParam(param);
	} else {
		HV_LOG_ERROR("Unable to register the param {}. No broker available.", param->getName());
	}
}

HV_CONFIGURATION_CLOSE_NAMESPACE
