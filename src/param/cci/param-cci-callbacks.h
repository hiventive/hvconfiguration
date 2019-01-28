/*
 * @file param-cci-callbacks.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief CCI compliant parameter callbacks
 */

#ifndef HV_CONFIGURATION_PARAM_CCI_CALLBACKS_H
#define HV_CONFIGURATION_PARAM_CCI_CALLBACKS_H

#include <HVCommon>

namespace cci {

template<typename T>
struct cci_param_write_event;

template<typename T>
struct cci_param_read_event;

/// Parameter pre write callback
template <typename T = void>
using cci_param_pre_write_callback
  = ::hv::common::CallbackImpl<bool(const cci_param_write_event<T>)>;

/// Parameter pre write callback handle
template <typename T = void>
using cci_param_pre_write_callback_handle
  = ::hv::common::CallbackTypedHandle<bool(const cci_param_write_event<T>&)>;

/// Parameter post write callback
template <typename T = void>
using cci_param_post_write_callback
  = ::hv::common::CallbackImpl<void(const cci_param_write_event<T>&)>;

/// Parameter post write callback handle
template <typename T = void>
using cci_param_post_write_callback_handle
  = ::hv::common::CallbackTypedHandle<void(const cci_param_write_event<T>&)>;

/// Parameter pre read callback
template <typename T = void>
using cci_param_pre_read_callback
  = ::hv::common::CallbackImpl<void(const cci_param_read_event<T>&)>;

/// Parameter pre read callback handle
template <typename T = void>
using cci_param_pre_read_callback_handle
  = ::hv::common::CallbackTypedHandle<void(const cci_param_read_event<T>&)>;

/// Parameter post read callback
template <typename T = void>
using cci_param_post_read_callback
  = ::hv::common::CallbackImpl<void(const cci_param_read_event<T>&)>;

/// Parameter post read callback handle
template <typename T = void>
using cci_param_post_read_callback_handle
  = ::hv::common::CallbackTypedHandle<void(const cci_param_read_event<T>&)>;

typedef ::hv::common::CallbackUntypedHandle cci_callback_untyped_handle;

}

#endif // HV_CONFIGURATION_PARAM_CCI_CALLBACKS_H
