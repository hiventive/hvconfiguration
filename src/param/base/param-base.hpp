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
	init();
}

template<typename T>
ParamBase<T>::ParamBase(const std::string& name,
		const T& defaultValue,
		const std::string& description) :
		name(name), value(defaultValue),
		defaultValue(defaultValue),
		description(description) {
	init();
}

template<typename T>
void ParamBase<T>::init() {
	std::string hierarchicalName = getRelativeUniqueName(name);
	HV_LOG_TRACE("Initialiazing {}", hierarchicalName);
	this->name = hierarchicalName;
	_registerParam(this);
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
	return (!preReadCallbacks.isEmpty() ||
			!postReadCallbacks.isEmpty() ||
			!preWriteCallbacks.isEmpty() ||
			!postWriteCallbacks.isEmpty());
}

template<typename T>
::hv::common::hvcbID_t ParamBase<T>::registerPreReadCallback(const PreReadCallback<T> &cb) {
	::hv::common::hvcbID_t idTmp = this->genCallbackID();
	preReadCallbacks.setCb(this->genCallbackID(), cb);
	return idTmp;
}

template<typename T>
template<typename U>
::hv::common::hvcbID_t ParamBase<T>::registerPreReadCallback(bool (U::*cb)(const ParamReadEvent<T>&), U *obj) {
	return registerPreReadCallback(std::bind(cb, obj, std::placeholders::_1));
}


template<typename T>
::hv::common::hvcbID_t ParamBase<T>::registerPostReadCallback(const PostReadCallback<T> &cb) {
	::hv::common::hvcbID_t idTmp = this->genCallbackID();
	postReadCallbacks.setCb(this->genCallbackID(), cb);
	return idTmp;
}

template<typename T>
template<typename U>
::hv::common::hvcbID_t ParamBase<T>::registerPostReadCallback(void (U::*cb)(const ParamReadEvent<T>&), U *obj) {
	return registerPostReadCallback(std::bind(cb, obj, std::placeholders::_1));
}

template<typename T>
::hv::common::hvcbID_t ParamBase<T>::registerPreWriteCallback(const PreWriteCallback<T> &cb) {
	::hv::common::hvcbID_t idTmp = this->genCallbackID();
	preWriteCallbacks.setCb(this->genCallbackID(), cb);
	return idTmp;
}

template<typename T>
template<typename U>
::hv::common::hvcbID_t ParamBase<T>::registerPreWriteCallback(bool (U::*cb)(const ParamWriteEvent<T>&), U *obj) {
	return registerPreWriteCallback(std::bind(cb, obj, std::placeholders::_1));
}

template<typename T>
::hv::common::hvcbID_t ParamBase<T>::registerPostWriteCallback(const PostWriteCallback<T> &cb) {
	::hv::common::hvcbID_t idTmp = this->genCallbackID();
	postWriteCallbacks.setCb(this->genCallbackID(), cb);
	return idTmp;
}

template<typename T>
template<typename U>
::hv::common::hvcbID_t ParamBase<T>::registerPostWriteCallback(void (U::*cb)(const ParamWriteEvent<T>&), U *obj) {
	return registerPostWriteCallback(std::bind(cb, obj, std::placeholders::_1));
}

template<typename T>
bool ParamBase<T>::unregisterPreReadCallback(const ::hv::common::hvcbID_t &id) {
	if(preReadCallbacks.hasID(id)) {
		preReadCallbacks.erase(id);
		return true;
	}
	return false;
}

template<typename T>
bool ParamBase<T>::unregisterPostReadCallback(const ::hv::common::hvcbID_t &id) {
	if(postReadCallbacks.hasID(id)) {
		postReadCallbacks.erase(id);
		return true;
	}
	return false;
}

template<typename T>
bool ParamBase<T>::unregisterPreWriteCallback(const ::hv::common::hvcbID_t &id) {
	if(preWriteCallbacks.hasID(id)) {
		preWriteCallbacks.erase(id);
		return true;
	}
	return false;
}

template<typename T>
bool ParamBase<T>::unregisterPostWriteCallback(const ::hv::common::hvcbID_t &id) {
	if(postWriteCallbacks.hasID(id)) {
		postWriteCallbacks.erase(id);
		return true;
	}
	return false;
}

template<typename T>
bool ParamBase<T>::unregisterAllCallbacks() {
	preReadCallbacks.clear();
	postReadCallbacks.clear();
	preWriteCallbacks.clear();
	postWriteCallbacks.clear();
	return true;
}

template<typename T>
::hv::common::hvcbID_t ParamBase<T>::genCallbackID() {
	return cbIDCpt++;
}

template<typename T>
ParamBase<T>::~ParamBase() {
}

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_PARAM_BASE_IMPL_H
