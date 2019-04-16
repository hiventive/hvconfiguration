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
		HV_LOG_WARNING("Unable to find parameter with name {}", paramName);
		return T();
	}
}

template <typename T>
void BrokerBase::setValue(const std::string& paramName,
		const T& value) {
	if(hasParam(paramName)) {
		params[paramName]->getParamTyped<T>().setValue(value);
	} else {
		HV_LOG_WARNING("Unable to find parameter with name {}", paramName);
	}
}

std::string BrokerBase::getPresetValue(const std::string &paramName) {
	HV_LOG_ERROR("getPresetValue() is not implemented");
	// TODO: Missing string conversion to type T
	/*if(presets->hasValue(paramName)) {
		return presets->getValue(paramName);
	} else {
		HV_LOG_WARNING("Unable to find parameter with name {}", paramName);
		return std::string();
	}*/
	return std::string();
}

void BrokerBase::setPresetValue(const std::string& paramName,
		const std::string& presetValue) {
	HV_LOG_ERROR("setPresetValue() is not implemented");
	// TODO: Missing  string conversion to type T
	//presets->setValue(paramName, presetValue);
}

std::vector<std::pair<std::string, std::string> > BrokerBase::getUnconsumedPresetValues() const {
	return std::vector<std::pair<std::string,std::string> >(presets->getValues().begin(), presets->getValues().end());
}

void BrokerBase::lockPresetValue(const std::string& paramName) {
	// TODO
	HV_LOG_ERROR("setPresetValue() is not implemented");
}

bool BrokerBase::hasPresetValue(const std::string& paramName) const {
	return presets->hasValue(paramName);
}

void BrokerBase::addParam(ParamIf* paramBase) {
	if(paramBase) {
		HV_LOG_TRACE("Broker adding param {}", paramBase->getName());
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

ParamIf* BrokerBase::getParam(const std::string& paramName) {
	if(hasParam(paramName)) {
		return params.at(paramName);
	} else {
		return nullptr;
	}
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
