/*
 * @file broker-base.cpp
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Base broker implementaton
 */

#include "../../storage/memory/memory.h"
#include "broker-base.h"

HV_CONFIGURATION_OPEN_NAMESPACE

BrokerBase::BrokerBase(const std::string& name, StorageIf* storage) :
	name(name), deleteStorage(false) {
	if(storage == nullptr) {
		this->presets = new Memory();
		this->deleteStorage = true;
	}
}

const std::string& BrokerBase::getName() const {
	return name;
}

template <typename T>
const T& BrokerBase::getValue(const std::string& paramName) const {
	if(hasParam(paramName)) {
		ParamIf* param = params.at(paramName);
		ParamBase<T>* paramTyped = param->getParamTyped<T>();
		return paramTyped->getValue();
	} else {
		// TODO: write an error
		return T();
	}
}

template <typename T>
void BrokerBase::setValue(const std::string& paramName,
		const T& value) {
	if(hasParam(paramName)) {
		params[paramName]->getParamTyped<T>().setValue(value);
	} else {
		// TODO: write an error
	}
}

template <typename T>
const T& BrokerBase::getPresetValue(const std::string &paramName) {
	if(presets->hasValue(paramName)) {
		// FIXME
		// return presets->getValue<T>(paramName);
	} else {
		// TODO: write an error
		return T();
	}
}

template <typename T>
void BrokerBase::setPresetValue(const std::string& paramName,
		const T& presetValue) {
	if(presets->hasValue(paramName)) {
		// FIXME
		// presets->setValue<T>(paramName, presetValue);
	} else {
		// TODO: write an error
	}
}

template <typename T>
std::vector<std::pair<std::string, T> > BrokerBase::getUnconsumedPresetValues() const {

}

void BrokerBase::lockPresetValue(const std::string& paramName) {

}

bool BrokerBase::hasPresetValue(const std::string& paramName) const {
	return presets->hasValue(paramName);
}

void BrokerBase::addParam(ParamIf* paramBase) {
	if(paramBase) {
		params[paramBase->getName()] = paramBase;
	}
}


void BrokerBase::removeParam(ParamIf* paramBase) {
	if(paramBase) {
		std::string paramName = paramBase->getName();
		if(hasParam(paramName)) {
			params.erase(paramName);
		}
	}
}


std::vector<ParamIf*> BrokerBase::getParams() const {
	std::vector<ParamIf*> result;
	for(auto const &entry : params) {
		result.push_back(entry.second);
	}
	return result;
}

bool BrokerBase::hasParam(const std::string& paramName) const {
	return params.find(paramName) != params.end();
}


BrokerBase::~BrokerBase() {
	if(deleteStorage) {
		delete presets;
	}
}

HV_CONFIGURATION_CLOSE_NAMESPACE
