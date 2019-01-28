/*
 * @file broker-cci.hpp
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief CCI compliant broker implementaton
 */

#ifndef HV_CONFIGURATION_BROKER_CCI_IMPL_H
#define HV_CONFIGURATION_BROKER_CCI_IMPL_H

#include "broker-cci.h"
#include "../../configuration/common.h"

#define HV_BROKER_CCI_USED_PRESETS_PREFIX "used-presets."
#define HV_BROKER_CCI_LOCKED_PRESETS_PREFIX "locked-presets."

HV_CONFIGURATION_OPEN_NAMESPACE

BrokerCCI::BrokerCCI(BrokerBase& brokerBase,
		StorageIf* storage,
		bool registerCCI) :
	brokerBase(brokerBase), params(), presetOriginators(),
	createCallbacks(), destroyCallbacks(), ignoredUnconsumedPredicates() {
	if(!storage) {
		deleteStorage = true;
		presets = new Memory();
	} else {
		deleteStorage = false;
		presets = storage;
	}
	if(registerCCI) {
		::cci::cci_register_broker(this);
	}
}

const char* BrokerCCI::CCIName() const {
	return brokerBase.getName().c_str();
}

bool BrokerCCI::is_global_broker() const {
	return true;
}

::cci::cci_param_untyped_handle BrokerCCI::get_param_handle(const std::string& paramName,
		const ::cci::cci_originator& originator) const {
	if(hasCCIParam(paramName)) {
		return ::cci::cci_param_untyped_handle(*getCCIParam(paramName), originator);
	}
	return ::cci::cci_param_untyped_handle(originator);
}

::cci::cci_originator BrokerCCI::get_value_origin(const std::string& paramName) const {
	if(hasCCIParam(paramName)) {
		return getCCIParam(paramName)->get_value_origin();
	} else if(hasCCIPreset(paramName)) {
		return getCCIPresetOriginator(paramName);
	} else {
		return cci_broker_if::unknown_originator();
	}
}

::cci::cci_value BrokerCCI::get_cci_value(const std::string& paramName,
		const ::cci::cci_originator& originator) const {
	if(hasCCIParam(paramName)) {
		return getCCIParam(paramName)->get_cci_value(originator);
	} else {
		std::string errorMessage = std::string("[Broker] Unable to find the parameter with name: ") + paramName;
		::cci::cci_report_handler::get_param_failed(errorMessage.c_str());
		return ::cci::cci_value();
	}
}

std::vector<::cci::cci_param_untyped_handle> BrokerCCI::get_param_handles(
		const ::cci::cci_originator& originator = ::cci::cci_originator() ) const {
	std::vector<::cci::cci_param_untyped_handle> paramHandles;
	std::map<std::string, ::cci::cci_param_if*> params = getCCIParams();
	for(auto const &entry : params) {
		paramHandles.push_back(::cci::cci_param_untyped_handle(*entry.second, originator));
	}
	return paramHandles;
}

::cci::cci_param_range BrokerCCI::get_param_handles(::cci::cci_param_predicate& pred,
		const ::cci::cci_originator& originator) const {
	return ::cci::cci_param_range(pred,
			get_param_handles(originator));
}

bool BrokerCCI::has_preset_value(const std::string& paramName) const {
	return hasCCIPreset(paramName);
}

void BrokerCCI::set_preset_cci_value(
		const std::string& paramName,
		const ::cci::cci_value& cciValue,
		const ::cci::cci_originator& originator) {
	if (isCCIPresetLocked(paramName)) {
		std::string errorMessage = std::string("[Broker] Preset is already locked for parameter with name: ")
				+ paramName;
		::cci::cci_report_handler::set_param_failed(errorMessage.c_str());
		return;
	}
	setCCIPreset(paramName, cciValue);
	setCCIPresetOriginator(paramName, originator);
}

::cci::cci_value BrokerCCI::get_preset_cci_value(const std::string& paramName) const {
	if(hasCCIPreset(paramName)) {
		return getCCIPreset(paramName);
	} else {
		std::string errorMessage = std::string("[Broker] Unable to find the parameter with name: ") + paramName;
		::cci::cci_report_handler::get_param_failed(errorMessage.c_str());
		return ::cci::cci_value();
	}
}

::cci::cci_originator BrokerCCI::get_preset_value_origin(const std::string& paramName) const {
	if(hasCCIPresetOriginator(paramName)) {
		return getCCIPresetOriginator(paramName);
	} else {
		return cci_broker_if::unknown_originator();
	}
}

void BrokerCCI::lock_preset_value(const std::string& paramName) {
	setCCIPresetLocked(paramName);
}

std::vector<::cci::cci_name_value_pair> BrokerCCI::get_unconsumed_preset_values() const {
	std::vector<::cci::cci_name_value_pair> unconsumedPresetValues;
	auto pred = ignoredUnconsumedPredicates.begin();
	for (auto &entry : getCCIPresetUsed(false)) {
		::cci::cci_name_value_pair nameValuePair = std::make_pair(entry, getCCIPreset(entry));
		for (pred =  ignoredUnconsumedPredicates.begin();
			pred != ignoredUnconsumedPredicates.end(); ++pred) {
			const ::cci::cci_preset_value_predicate& p = *pred;
			if (p(nameValuePair)) {
				break;
			}
		}
		if (pred == ignoredUnconsumedPredicates.end()) {
			unconsumedPresetValues.push_back(nameValuePair);
		}
		unconsumedPresetValues.push_back(nameValuePair);
	}
	return unconsumedPresetValues;
}

::cci::cci_preset_value_range BrokerCCI::get_unconsumed_preset_values(
		const ::cci::cci_preset_value_predicate& pred) const {
	return ::cci::cci_preset_value_range(pred,
			get_unconsumed_preset_values());
}

void BrokerCCI::ignore_unconsumed_preset_values(const ::cci::cci_preset_value_predicate& pred) {
	ignoredUnconsumedPredicates.push_back(pred);
}

// FIXME: add override with a clean cci_broker_if
/* ::cci::cci_broker_handle BrokerCCI::create_broker_handle(const ::cci::cci_originator& originator) const {
	return ::cci::cci_broker_if::create_broker_handle(originator);
} */

::cci::cci_param_create_callback_handle BrokerCCI::register_create_callback(
		const ::cci::cci_param_create_callback& callback,
		const ::cci::cci_originator& originator) {
	createCallbacks.push_back(CCICallbackObject<::cci::cci_param_create_callback_handle::type>(callback,
			originator));
	return callback;
}

bool BrokerCCI::unregister_create_callback(const ::cci::cci_param_create_callback_handle& callback,
		const ::cci::cci_originator& originator) {
	for (auto it = createCallbacks.begin(); it != createCallbacks.end(); ++it) {
		if (it->callback == callback && it->originator == originator) {
			createCallbacks.erase(it);
			return true;
		}
	}
	return false;
}

::cci::cci_param_destroy_callback_handle BrokerCCI::register_destroy_callback(
		const ::cci::cci_param_destroy_callback& callback, const ::cci::cci_originator& originator) {
	destroyCallbacks.push_back(CCICallbackObject<::cci::cci_param_destroy_callback_handle::type>(callback,
			originator));
	return callback;
}

bool BrokerCCI::unregister_destroy_callback(const ::cci::cci_param_destroy_callback_handle& callback,
		const ::cci::cci_originator& originator) {
	for (auto it = destroyCallbacks.begin(); it != destroyCallbacks.end(); ++it) {
		if (it->callback == callback && it->originator == originator) {
			destroyCallbacks.erase(it);
			return true;
		}
	}
	return false;
}

bool BrokerCCI::unregister_all_callbacks(const ::cci::cci_originator&) {
	bool result = has_callbacks();
	createCallbacks.clear();
	destroyCallbacks.clear();
	return result;
}

bool BrokerCCI::has_callbacks() const {
	return !createCallbacks.empty() || !destroyCallbacks.empty();
}

void BrokerCCI::add_param(::cci::cci_param_if* param) {
	if(param) {
		if(!hasCCIParam(param->name())) {
			setCCIParam(param);

			for(auto &entry : createCallbacks) {
				entry.callback.invoke(param->create_param_handle(param->get_originator()));
			}

			if (!isCCIPresetUsed(param->name())) {
				setCCIPresetUsed(param->name(), true);
			}
		} else {
			// TODO: error
		}
	}
}

void BrokerCCI::remove_param(::cci::cci_param_if* param) {
	if (param) {
		if (hasCCIParam(param->name())) {
			removeCCIParam(param);

			for(auto &entry : destroyCallbacks) {
				entry.callback.invoke(param->create_param_handle(param->get_originator()));
			}

			if (isCCIPresetUsed(param->name())) {
				setCCIPresetUsed(param->name(), false);
			}
		} else {
			// TODO: error
		}
	}
}

::cci::cci_broker_handle BrokerCCI::create_broker_handle(const ::cci::cci_originator& originator) const {
	return ::cci::cci_broker_handle(
			*const_cast<::cci::cci_broker_if*>(
					static_cast<const ::cci::cci_broker_if*>(this)), originator);
}



// ---------------------------------------------------

void BrokerCCI::setCCIPresetLocked(const std::string& paramName) const {
	std::string paramKey = HV_BROKER_CCI_LOCKED_PRESETS_PREFIX + paramName;
	presets->setValue(paramKey, "1");
}

bool BrokerCCI::isCCIPresetLocked(const std::string& paramName) const {
	std::string paramKey = HV_BROKER_CCI_LOCKED_PRESETS_PREFIX + paramName;
	return presets->hasValue(paramKey);
}

void BrokerCCI::setCCIPresetUsed(const std::string& paramName, bool used) const {
	std::string paramKey = HV_BROKER_CCI_USED_PRESETS_PREFIX + paramName;
	presets->setValue(paramKey, std::to_string(static_cast<unsigned int>(used)));
}

bool BrokerCCI::isCCIPresetUsed(const std::string& paramName) const {
	std::string paramKey = HV_BROKER_CCI_USED_PRESETS_PREFIX + paramName;
	return presets->getValue(paramKey) == "1";
}

std::vector<std::string> BrokerCCI::getCCIPresetUsed(bool used) const {
	std::map<std::string, std::string> usedPresets = presets->getValues(HV_BROKER_CCI_USED_PRESETS_PREFIX);
	std::vector<std::string> result;
	for (auto &entry : usedPresets) {
		if (entry.second == std::to_string(static_cast<unsigned int>(used))) {
			result.push_back(entry.first.substr(entry.first.find(HV_BROKER_CCI_USED_PRESETS_PREFIX) + 1));
		}
	}
	return result;
}

bool BrokerCCI::hasCCIPreset(const std::string& paramName) const {
	return presets->hasValue(paramName);
}

::cci::cci_value BrokerCCI::getCCIPreset(const std::string& paramName) const {
	if(presets->hasValue(paramName)) {
		return ::cci::cci_value::from_json(presets->getValue(paramName));
	} else {
		return ::cci::cci_value();
	}
}

void BrokerCCI::setCCIPreset(const std::string& paramName, const ::cci::cci_value& value) {
	presets->setValue(paramName, value.to_json());
	setCCIPresetUsed(paramName, 0);
}




// ----------------------------

::cci::cci_originator BrokerCCI::getCCIPresetOriginator(const std::string& paramName) const {
	if(hasCCIPresetOriginator(paramName)) {
		return presetOriginators.at(paramName);
	} else {
		return ::cci::cci_originator();
	}
}

void BrokerCCI::setCCIPresetOriginator(const std::string& paramName, const ::cci::cci_originator& originator) {
	presetOriginators.insert(std::pair<std::string, const ::cci::cci_originator&>(paramName, originator));

}

bool BrokerCCI::hasCCIPresetOriginator(const std::string& paramName) const {
	return presetOriginators.find(paramName) != presetOriginators.end();
}

// ------------------

bool BrokerCCI::hasCCIParam(const std::string& paramName) const {
	return params.find(paramName) != params.end();
}

::cci::cci_param_if* BrokerCCI::getCCIParam(const std::string& paramName) const {
	if(hasCCIParam(paramName)) {
		return params.at(paramName);
	} else {
		return nullptr;
	}
}

std::map<std::string, ::cci::cci_param_if*> BrokerCCI::getCCIParams() const {
	return params;
}

void BrokerCCI::setCCIParam(::cci::cci_param_if* param) {
	params[param->name()] = param;
}

void BrokerCCI::removeCCIParam(::cci::cci_param_if* param) {
	params.erase(param->name());
}

BrokerCCI::~BrokerCCI() {
	if(deleteStorage) {
		delete presets;
	}
}

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_BROKER_CCI_IMPL_H
