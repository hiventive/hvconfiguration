/*
 * @file param-cci.hpp
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief CCI compliant parameter implementaton
 */

#ifndef HV_CONFIGURATION_PARAM_CCI_IMPL_H
#define HV_CONFIGURATION_PARAM_CCI_IMPL_H

#include "param-cci.h"
#include "../../configuration/common.h"

HV_CONFIGURATION_OPEN_NAMESPACE

template<typename T,
        ::cci::cci_param_mutable_type TM>
ParamCCI<T, TM>::ParamCCI(ParamBase<T>& paramBase,
		::cci::cci_name_type nameType,
		const ::cci::cci_originator& originator):
		ParamCCI(paramBase, findBrokerConvenience(originator), nameType, originator) {
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
ParamCCI<T, TM>::ParamCCI(ParamBase<T>& paramBase,
	    ::cci::cci_broker_handle privateBroker,
		::cci::cci_name_type nameType,
		const ::cci::cci_originator& originator):
		paramBase(paramBase), originator(originator),
		brokerHandle(privateBroker),
		valueOriginator(originator) {
	// Generate the right name
	if(nameType == ::cci::CCI_RELATIVE_NAME) {
		::sc_core::sc_object* currentObject = ::sc_core::sc_get_current_object();
		for (sc_core::sc_process_handle currentProcess(currentObject);
		     currentProcess.valid();
		     currentProcess = ::sc_core::sc_process_handle(currentObject)) {
			currentObject = currentProcess.get_parent_object();
		}
		if(currentObject) {
			paramBase.name = std::string(currentObject->name()) + ::sc_core::SC_HIERARCHY_CHAR + paramBase.name;
		}
	}

	// Handle name collision and destruction / resurrection
	std::string uniqueName = std::string(::cci::cci_gen_unique_name(paramBase.name.c_str()));
	if (uniqueName != paramBase.name && (::sc_core::sc_hierarchical_name_exists(paramBase.name.c_str())
	                                     || brokerHandle.has_preset_value(paramBase.name))) {
		paramBase.name = uniqueName;
	}

	// Set preset value (if available)
	if (brokerHandle.has_preset_value(paramBase.getName())) {
		::cci::cci_value presetCciValue = brokerHandle.get_preset_cci_value(paramBase.getName());
		T typedValue;
		if (presetCciValue.try_get<T>(typedValue)) {
			paramBase.value = typedValue;
		} else {
			// TODO: replace with HV_LOG(fatal, )
			std::cerr << "Unable to load preset CCI value for parameter " << paramBase.getName() << std::endl;
		}
	}
	this->init(brokerHandle);
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
ParamCCI<T, TM>::~ParamCCI() {
	// FIXME
	// ::sc_core::sc_assert(paramHandles.empty());

	if(!paramBase.name.empty()) {
		::cci::cci_unregister_name(name());
	}
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
const char* ParamCCI<T, TM>::name() const {
	return paramBase.getName().c_str();
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
void ParamCCI<T, TM>::set_description(const std::string& description) {
	paramBase.setDescription(description);
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
std::string ParamCCI<T, TM>::get_description() const {
	return paramBase.getDescription();
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
::cci::cci_param_data_category ParamCCI<T, TM>::get_data_category() const {
	switch (get_cci_value().category()) {
		case ::cci::CCI_BOOL_VALUE:
			return ::cci::CCI_BOOL_PARAM;
		case ::cci::CCI_INTEGRAL_VALUE:
			return ::cci::CCI_INTEGRAL_PARAM;
		case ::cci::CCI_REAL_VALUE:
			return ::cci::CCI_REAL_PARAM;
		case ::cci::CCI_STRING_VALUE:
			return ::cci::CCI_STRING_PARAM;
		case ::cci::CCI_LIST_VALUE:
			return ::cci::CCI_LIST_PARAM;
		case ::cci::CCI_NULL_VALUE:
		case ::cci::CCI_OTHER_VALUE:
		default:
			return ::cci::CCI_OTHER_PARAM;
	}
}

template<typename T,
		::cci::cci_param_mutable_type TM>
const std::type_info& ParamCCI<T, TM>::get_type_info() const {
	return typeid(paramBase.value);
}

template<typename T,
		::cci::cci_param_mutable_type TM>
::cci::cci_param_mutable_type ParamCCI<T, TM>::get_mutable_type() const {
	return TM;
}

template<typename T,
		::cci::cci_param_mutable_type TM>
void ParamCCI<T, TM>::set_cci_value(const ::cci::cci_value& cciValue,
		const void* password,
		const ::cci::cci_originator& originator) {
#if !defined(__clang__) && !defined(_MSC_VER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
	T typedValue = cciValue.get<T>();
#if !defined(__clang__) && !defined(_MSC_VER)
#pragma GCC diagnostic pop
#endif
	set_raw_value(&typedValue, password, originator);
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
::cci::cci_value ParamCCI<T, TM>::get_cci_value(const ::cci::cci_originator &originator) const {
	return ::cci::cci_value(*static_cast<const T*>(get_raw_value(originator)));
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
::cci::cci_value ParamCCI<T, TM>::get_default_cci_value() const {
	return ::cci::cci_value(paramBase.defaultValue);
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
bool ParamCCI<T, TM>::is_default_value() const {
	return paramBase.isDefaultValue();
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
bool ParamCCI<T, TM>::is_preset_value() const {
	if (brokerHandle.has_preset_value(paramBase.getName())) {
		::cci::cci_value presetCciValue = brokerHandle.get_preset_cci_value(paramBase.getName());
		T typedValue;
		if (presetCciValue.try_get<T>(typedValue)) {
			return typedValue == paramBase.value;
		}
	}
	return false;
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
bool ParamCCI<T, TM>::lock(const void* password) {
	if (!password) {
		password = this;
	}
	if (password != lockPassword && lockPassword != nullptr) {
		return false;
	} else {
		lockPassword = password;
		return true;
	}
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
::cci::cci_originator ParamCCI<T, TM>::get_originator() const {
	return originator;
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
::cci::cci_originator ParamCCI<T, TM>::get_value_origin() const {
	return valueOriginator;
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
bool ParamCCI<T, TM>::unlock(const void *password) {
	if (!password) {
		password = this;
	}
	if (password == lockPassword) {
		lockPassword = nullptr;
		return true;
	}
	return false;
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
bool ParamCCI<T, TM>::is_locked() const {
	return lockPassword != nullptr;
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
::cci::cci_value_map ParamCCI<T, TM>::get_metadata() const {
	return metadata;
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
void ParamCCI<T, TM>::add_metadata(const std::string &name,
				  const ::cci::cci_value& cciValue,
				  const std::string& description) {
	metadata.push_entry(name,
			::cci::cci_value_list().push_back(cciValue).push_back(description));
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
void ParamCCI<T, TM>::add_param_handle(::cci::cci_param_untyped_handle* paramHandle) {
	paramHandles.push_back(paramHandle);
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
void ParamCCI<T, TM>::remove_param_handle(::cci::cci_param_untyped_handle* paramHandle) {
	paramHandles.erase(std::remove(paramHandles.begin(),
			paramHandles.end(),
			paramHandle),
					paramHandles.end());
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
void ParamCCI<T, TM>::invalidate_all_param_handles() {
	while(!paramHandles.empty()) {
		paramHandles.front()->invalidate();
	}
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
bool ParamCCI<T, TM>::has_callbacks() const {
	return !preWriteCallbacks.empty()
	|| !postWriteCallbacks.empty()
	|| !preReadCallbacks.empty()
	|| !postReadCallbacks.empty();
}

template<typename T,
        ::cci::cci_param_mutable_type TM>
bool ParamCCI<T, TM>::unregister_all_callbacks(const ::cci::cci_originator& orig) {
	if (has_callbacks()) {
		preWriteCallbacks.clear();
		postWriteCallbacks.clear();
		preReadCallbacks.clear();
		postReadCallbacks.clear();
		return true;
	}
	return false;
}

template<typename T,
		::cci::cci_param_mutable_type TM>
bool ParamCCI<T, TM>::reset() {
	paramBase.reset();
	// FIXME
	return true;
}

template<typename T,
		::cci::cci_param_mutable_type TM>
void ParamCCI<T, TM>::preset_cci_value(const ::cci::cci_value& cciValue, const ::cci::cci_originator& originator) {
	const T& newValue = cciValue.get<T>();

	// TODO: Pre write callback

	// Save old value
	// T oldValue = paramBase.value; // FIXME

	// Write new value
#if !defined(__clang__) && !defined(_MSC_VER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
	paramBase.value = newValue;
#if !defined(__clang__) && !defined(_MSC_VER)
#pragma GCC diagnostic pop
#endif

	// Update value originator
	valueOriginator = originator;

	// TODO: Post write callback
}

template<typename T,
		::cci::cci_param_mutable_type TM>
void ParamCCI<T, TM>::set_raw_value(const void* value, const void* password,
				   const ::cci::cci_originator& originator) {
	const T& newValue = *static_cast<const T*>(value);

	// FIXME
	/* if (!paramBase.set_cci_value_allowed(TM)) {
		return;
	} */

	if(!password) {
		if (is_locked()) {
			::cci::cci_report_handler::set_param_failed("Parameter locked.", __FILE__, __LINE__);
			return;
		}
	} else {
		if (password != lockPassword) {
			::cci::cci_report_handler::set_param_failed("Wrong key.", __FILE__, __LINE__);
			return;
		}
	}

	// TODO: Pre write callback

	// Save old value
	// T oldValue = paramBase.value; // FIXME

	// Write new value
#if !defined(__clang__) && !defined(_MSC_VER)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
	paramBase.value = newValue;
#if !defined(__clang__) && !defined(_MSC_VER)
#pragma GCC diagnostic pop
#endif

	// Update value originator
	valueOriginator = originator;

	// TODO: Post write callback
}

template<typename T,
		::cci::cci_param_mutable_type TM>
const void* ParamCCI<T, TM>::get_raw_value(const ::cci::cci_originator& originator) const {
	return &(paramBase.value);
}

template<typename T,
		::cci::cci_param_mutable_type TM>
const void* ParamCCI<T, TM>::get_raw_default_value() const {
	return &(paramBase.defaultValue);
}

template<typename T, ::cci::cci_param_mutable_type TM>
::cci::cci_callback_untyped_handle ParamCCI<T, TM>::register_pre_write_callback(
			const ::cci::cci_callback_untyped_handle& callback,
			const ::cci::cci_originator& originator) {
	preWriteCallbacks.push_back(CCICallbackObject<::cci::cci_callback_untyped_handle::type>(callback,
			originator));
	return callback;
}

template<typename T, ::cci::cci_param_mutable_type TM>
bool ParamCCI<T, TM>::unregister_pre_write_callback(
		const ::cci::cci_callback_untyped_handle& callback,
		const ::cci::cci_originator& originator) {
	for (auto it = preWriteCallbacks.begin(); it != preWriteCallbacks.end(); ++it) {
		if (it->callback == callback && it->originator == originator) {
			preWriteCallbacks.erase(it);
			return true;
		}
	}
	return false;
}

template<typename T, ::cci::cci_param_mutable_type TM>
::cci::cci_callback_untyped_handle ParamCCI<T, TM>::register_post_write_callback(
		const ::cci::cci_callback_untyped_handle& callback,
		const ::cci::cci_originator& originator) {
	postWriteCallbacks.push_back(CCICallbackObject<::cci::cci_callback_untyped_handle::type>(callback,
			originator));
	return callback;
}

template<typename T, ::cci::cci_param_mutable_type TM>
bool ParamCCI<T, TM>::unregister_post_write_callback(
		const ::cci::cci_callback_untyped_handle& callback,
		const ::cci::cci_originator& originator) {
	for (auto it = postWriteCallbacks.begin(); it != postWriteCallbacks.end(); ++it) {
		if (it->callback == callback && it->originator == originator) {
			postWriteCallbacks.erase(it);
			return true;
		}
	}
	return false;
}

template<typename T, ::cci::cci_param_mutable_type TM>
::cci::cci_callback_untyped_handle ParamCCI<T, TM>::register_pre_read_callback(
		const ::cci::cci_callback_untyped_handle& callback,
		const ::cci::cci_originator& originator) {
	preReadCallbacks.push_back(CCICallbackObject<::cci::cci_callback_untyped_handle::type>(callback,
			originator));
	return callback;
}

template<typename T, ::cci::cci_param_mutable_type TM>
bool ParamCCI<T, TM>::unregister_pre_read_callback(
		const ::cci::cci_callback_untyped_handle& callback,
		const ::cci::cci_originator& originator) {
	for (auto it = preReadCallbacks.begin(); it != preReadCallbacks.end(); ++it) {
		if (it->callback == callback && it->originator == originator) {
			preReadCallbacks.erase(it);
			return true;
		}
	}
	return false;
}

template<typename T, ::cci::cci_param_mutable_type TM>
::cci::cci_callback_untyped_handle ParamCCI<T, TM>::register_post_read_callback(
		const ::cci::cci_callback_untyped_handle& callback,
		const ::cci::cci_originator& originator) {
	postReadCallbacks.push_back(CCICallbackObject<::cci::cci_callback_untyped_handle::type>(callback,
			originator));
	return callback;
}

template<typename T, ::cci::cci_param_mutable_type TM>
bool ParamCCI<T, TM>::unregister_post_read_callback(
		const ::cci::cci_callback_untyped_handle& callback,
		const ::cci::cci_originator& originator) {
	for (auto it = postReadCallbacks.begin(); it != postReadCallbacks.end(); ++it) {
		if (it->callback == callback && it->originator == originator) {
			postReadCallbacks.erase(it);
			return true;
		}
	}
	return false;
}

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_PARAM_CCI_IMPL_H
