/*
 * @file broker-cci-callbacks.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief CCI compliant broker callbacks
 */

#ifndef HV_CONFIGURATION_BROKER_CCI_CALLBACKS_H
#define HV_CONFIGURATION_BROKER_CCI_CALLBACKS_H

#include <HVCommon>
#include "cci_cfg/cci_param_typed_handle.h"

namespace cci {

/// Parameter creation callback
typedef ::hv::common::CallbackImpl<void(const ::cci::cci_param_untyped_handle&)>
		cci_param_create_callback;

/// Parameter creation callback handle
typedef ::hv::common::CallbackTypedHandle<void(const ::cci::cci_param_untyped_handle&)>
		cci_param_create_callback_handle;

/// Parameter destruction callback
typedef ::hv::common::CallbackImpl<void(const ::cci::cci_param_untyped_handle&)>
		cci_param_destroy_callback;

/// Parameter destruction callback handle
typedef ::hv::common::CallbackTypedHandle<void(const ::cci::cci_param_untyped_handle&)>
		cci_param_destroy_callback_handle;

/// Parameter predicate
typedef ::hv::common::CallbackImpl<bool(const ::cci::cci_param_untyped_handle&)>
		cci_param_predicate;

/// Parameter predicate handle
typedef ::hv::common::CallbackTypedHandle<bool(const ::cci::cci_param_untyped_handle&)>
		cci_param_predicate_handle;

/// Preset value predicate
typedef ::hv::common::CallbackImpl<bool(const std::pair<std::string, ::cci::cci_value>&)>
		cci_preset_value_predicate;

/// Preset value predicate handle
typedef ::hv::common::CallbackTypedHandle<bool(const std::pair<std::string, ::cci::cci_value>&)>
        cci_preset_value_predicate_handle;

}

#endif // HV_CONFIGURATION_BROKER_CCI_CALLBACKS_H
