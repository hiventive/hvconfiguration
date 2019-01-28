/*
 * @file param-base.hpp
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive base parameter implementation
 */

#ifndef HV_CONFIGURATION_PARAM_BASE_IMPL_H
#define HV_CONFIGURATION_PARAM_BASE_IMPL_H

#include "param-base.h"

HV_CONFIGURATION_OPEN_NAMESPACE

template<typename T>
ParamBase<T>::ParamBase(const std::string& name,
		const T& defaultValue) :
	name(name), value(defaultValue), defaultValue(defaultValue) {
}

template<typename T>
ParamBase<T>::ParamBase(const std::string& name,
		const T& defaultValue,
		const std::string& description) :
		name(name), value(defaultValue),
		defaultValue(defaultValue),
		description(description) {
}

template<typename T>
const std::string& ParamBase<T>::getName() const {
	return name;
}

template<typename T>
void ParamBase<T>::setValue(const T& value) {
	this->value = value;
}

template<typename T>
const T& ParamBase<T>::getValue() const {
	return value;
}

template<typename T>
ParamBase<T>& ParamBase<T>::operator= (const ParamBase& paramBase) {
	setValue(paramBase.getValue());
	return *this;
}

template<typename T>
ParamBase<T>& ParamBase<T>::operator= (const T& value) {
	setValue(value);
	return *this;
}

template<typename T>
ParamBase<T>::operator const T&() const {
	return getValue();
}

template<typename T>
const T& ParamBase<T>::getDefaultValue() const {
	return defaultValue;
}

template<typename T>
bool ParamBase<T>::isDefaultValue() const {
	return value == defaultValue;
}

template<typename T>
void ParamBase<T>::setDescription(const std::string& description) {
	this->description = description;
}

template<typename T>
std::string ParamBase<T>::getDescription() const {
	return description;
}

template<typename T>
bool ParamBase<T>::reset() {
	value = defaultValue;
	return true;
}

template<typename T>
bool ParamBase<T>::hasCallbacks() const {
	return true; // FIXME
}

template<typename T>
ParamBase<T>::~ParamBase() {
}

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_PARAM_BASE_IMPL_H
