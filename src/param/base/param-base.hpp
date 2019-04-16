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
ParamBase<T>::ParamBase(const std::string& name, const T& defaultValue):
	name(name), value(defaultValue), defaultValue(defaultValue), cbIDCpt(0) {
	initName();
}

template<typename T>
ParamBase<T>::ParamBase(const std::string& name, const T& defaultValue, const std::string& description):
		name(name), value(defaultValue), defaultValue(defaultValue), description(description), cbIDCpt(0) {
	initName();
}

template<typename T>
ParamBase<T>::ParamBase(const ParamBase &paramBase):
		name(paramBase.name),
		value(paramBase.value),
		defaultValue(paramBase.defaultValue),
		description(paramBase.description),
		cbIDCpt(paramBase.cbIDCpt) {
}

template<typename T>
void ParamBase<T>::initName() {
	HV_LOG_TRACE("Initialiazing {}", name);

	// Hiventive parameter do not support parameter destruction / resurrection.
	// We only support relative unique name if not used by CCI
	std::string uniqueName = generateRelativeUniqueName(name);

	// We set uniqueName (maybe be updated later by CCI initialization)
	name = uniqueName;
}

template<typename T>
void ParamBase<T>::setName(const std::string& name) {
	this->name = name;
}

template<typename T>
void ParamBase<T>::setValue(const T& value) {
	T oldValue = this->value;
	if(runPreWriteCallbacks(value)) {
		this->value = value;
	}
	runPostWriteCallbacks(oldValue, value);
}

template<typename T>
const T& ParamBase<T>::getValue() const {
	runPreReadCallbacks(value);
	const T* tmpValue = &value;
	runPostReadCallbacks(value);
	return *tmpValue;
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
	preReadCallbacks.setCb(idTmp, cb);
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
	postReadCallbacks.setCb(idTmp, cb);
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
	preWriteCallbacks.setCb(idTmp, cb);
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
	postWriteCallbacks.setCb(idTmp, cb);
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
void ParamBase<T>::runPreReadCallbacks(const T& value) const
{
	HV_LOG_TRACE("runPreReadCallbacks");

	if (!preReadCallbacks.isUsing()) {
		preReadCallbacks.setUsing(true);

		for(auto const &preReadCallback : preReadCallbacks.getMap()) {
			const ParamReadEvent<T> ev(this->value, *this);
			(preReadCallback.second)(ev);
		}

		preReadCallbacks.setUsing(false);
	}
}

template<typename T>
void ParamBase<T>::runPostReadCallbacks(const T& value) const
{
	HV_LOG_TRACE("runPostReadCallbacks");

	if (!postReadCallbacks.isUsing()) {
		postReadCallbacks.setUsing(true);

		for(auto const &postReadCallback : postReadCallbacks.getMap()) {
			const ParamReadEvent<T> ev(this->value, *this);
			(postReadCallback.second)(ev);
		}

		postReadCallbacks.setUsing(false);
	}
}

template<typename T>
bool ParamBase<T>::runPreWriteCallbacks(const T& value) const
{
	HV_LOG_TRACE("runPreWriteCallbacks");

	if (!preWriteCallbacks.isUsing()) {
		preWriteCallbacks.setUsing(true);

		bool result = true;
		for(auto const &preWriteCallback : preWriteCallbacks.getMap()) {
			const ParamWriteEvent<T> ev(this->value, value, *this);
			if (!(preWriteCallback.second)(ev)) {
				HV_LOG_WARNING("The new value has been rejected by a callback.");
				result = false;
			}
		}

		preWriteCallbacks.setUsing(false);
		return result;
	}
	return false;
}

template<typename T>
void ParamBase<T>::runPostWriteCallbacks(const T& oldValue, const T& newValue) const
{
	HV_LOG_TRACE("runPostWriteCallbacks");

	if (!postWriteCallbacks.isUsing()) {
		postWriteCallbacks.setUsing(true);

		for(auto const &postWriteCallback : postWriteCallbacks.getMap()) {
			const ParamWriteEvent<T> ev(oldValue, newValue, *this);
			(postWriteCallback.second)(ev);
		}

		postWriteCallbacks.setUsing(false);
	}
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
