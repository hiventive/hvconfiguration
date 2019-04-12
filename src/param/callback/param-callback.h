/*
 * @file param-callback.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date March, 2019
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Parameter callback
 */

#ifndef HV_CONFIGURATION_PARAM_CALLBACK_H
#define HV_CONFIGURATION_PARAM_CALLBACK_H

#include "../../configuration/common.h"
#include <hv/common/callback.h>

HV_CONFIGURATION_OPEN_NAMESPACE

// Forward declaration of ParamIf class
class ParamIf;

template<typename T>
struct ParamReadEvent {

	ParamReadEvent(const T& value, const ParamIf& param);

	virtual ~ParamReadEvent() HV_CPLUSPLUS_MEMBER_FUNCTION_DEFAULT;

	/**
	 * Parameter value
	 */
	T value;

	/**
	 * Param handle
	 */
	const ParamIf& ph;
};

template<typename T>
struct ParamWriteEvent {

	ParamWriteEvent(const T& oldValue, const T& newValue, const ParamIf& param);

	virtual ~ParamWriteEvent() HV_CPLUSPLUS_MEMBER_FUNCTION_DEFAULT;

	/**
	 * Old parameter value
	 */
	T oldValue;

	/**
	 * New parameter value
	 */
	T newValue;

	/**
	 * Param handle
	 */
	const ParamIf& ph;
};

template <typename T = void>
struct PreReadCallback: ::hv::common::CallbackImpl<bool(const ParamReadEvent<T>&)> {};

template <typename T = void>
struct PostReadCallback: ::hv::common::CallbackImpl<void(const ParamReadEvent<T>&)> {};

template <typename T = void>
struct PreWriteCallback: ::hv::common::CallbackImpl<bool(const ParamWriteEvent<T>&)> {};

template <typename T = void>
struct PostWriteCallback: ::hv::common::CallbackImpl<void(const ParamWriteEvent<T>&)> {};

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_PARAM_CALLBACK_H
