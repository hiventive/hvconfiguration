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

std::string generateRelativeUniqueName(const std::string& name) {
	if(name.empty()) {
		HV_LOG_CRITICAL("Empty parameter name is not allowed");
		HV_EXIT_FAILURE();
	}

	// Hierarchical name
	std::string hierarchicalName = name;
	sc_core::sc_object* currentObj = sc_core::sc_get_current_object();
	for (sc_core::sc_process_handle current_proc(currentObj);
		 current_proc.valid();
		 current_proc = sc_core::sc_process_handle(currentObj)) {
		currentObj = current_proc.get_parent_object();
	}
	if(currentObj) {
		hierarchicalName = std::string(currentObj->name()) + sc_core::SC_HIERARCHY_CHAR + name;
	}

	// Unique name
	// SystemC >= 2.3.2 required
	if(sc_core::sc_hierarchical_name_exists(hierarchicalName.c_str())) {
		const char* newName = sc_core::sc_gen_unique_name(hierarchicalName.c_str());
		HV_LOG_WARNING("{} is already used in the SystemC hierarchy, using {} instead", hierarchicalName, newName);
		return std::string(newName);
	} else {
		return hierarchicalName;
	}
}

bool registerName(const std::string& name) {
	return sc_core::sc_register_hierarchical_name(name.c_str());
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

void _hasPresetValue(const std::string& name) {
	if(_globalBroker) {
		_globalBroker->hasPresetValue(name);
	} else {
		HV_LOG_ERROR("Unable to check preset value of param {}. No broker available.", name);
	}
}

template <typename T>
T convertStringToT(const std::string& str)
{
    std::istringstream ss(str);
    T num;
    ss >> num;
    return num;
}

HV_CONFIGURATION_CLOSE_NAMESPACE
