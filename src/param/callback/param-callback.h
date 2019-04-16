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
struct PreReadCallback: public ::hv::common::CallbackImpl<bool(const ParamReadEvent<T>&)> {
	PreReadCallback(): ::hv::common::CallbackImpl<bool(const ParamReadEvent<T>&)>() {}

	template<typename U>
	PreReadCallback(const U &fIn): ::hv::common::CallbackImpl<bool(const ParamReadEvent<T>&)>(fIn) {}

	template<typename U>
	PreReadCallback(bool (U::*cbIn)(const ParamReadEvent<T>&), U *objIn):
		::hv::common::CallbackImpl<bool(const ParamReadEvent<T>&)>(cbIn, objIn) {}

	PreReadCallback(const ::hv::common::CallbackImpl<bool(const ParamReadEvent<T>&)> &src):
		::hv::common::CallbackImpl<bool(const ParamReadEvent<T>&)>(src) {}
};

template <typename T = void>
struct PostReadCallback: public ::hv::common::CallbackImpl<void(const ParamReadEvent<T>&)> {
	PostReadCallback(): ::hv::common::CallbackImpl<void(const ParamReadEvent<T>&)>() {}

	template<typename U>
	PostReadCallback(const U &fIn): ::hv::common::CallbackImpl<void(const ParamReadEvent<T>&)>(fIn) {}

	template<typename U>
	PostReadCallback(bool (U::*cbIn)(const ParamReadEvent<T>&), U *objIn):
			::hv::common::CallbackImpl<void(const ParamReadEvent<T>&)>(cbIn, objIn) {}

	PostReadCallback(const ::hv::common::CallbackImpl<void(const ParamReadEvent<T>&)> &src):
			::hv::common::CallbackImpl<void(const ParamReadEvent<T>&)>(src) {}
};

template <typename T = void>
struct PreWriteCallback: public ::hv::common::CallbackImpl<bool(const ParamWriteEvent<T>&)> {
	PreWriteCallback(): ::hv::common::CallbackImpl<bool(const ParamWriteEvent<T>&)>() {}

	template<typename U>
	PreWriteCallback(const U &fIn): ::hv::common::CallbackImpl<bool(const ParamWriteEvent<T>&)>(fIn) {}

	template<typename U>
	PreWriteCallback(bool (U::*cbIn)(const ParamWriteEvent<T>&), U *objIn):
			::hv::common::CallbackImpl<bool(const ParamWriteEvent<T>&)>(cbIn, objIn) {}

	PreWriteCallback(const ::hv::common::CallbackImpl<bool(const ParamWriteEvent<T>&)> &src):
			::hv::common::CallbackImpl<bool(const ParamWriteEvent<T>&)>(src) {}
};

template <typename T = void>
struct PostWriteCallback: public ::hv::common::CallbackImpl<void(const ParamWriteEvent<T>&)> {
	PostWriteCallback(): ::hv::common::CallbackImpl<void(const ParamWriteEvent<T>&)>() {}

	template<typename U>
	PostWriteCallback(const U &fIn): ::hv::common::CallbackImpl<void(const ParamWriteEvent<T>&)>(fIn) {}

	template<typename U>
	PostWriteCallback(bool (U::*cbIn)(const ParamWriteEvent<T>&), U *objIn):
			::hv::common::CallbackImpl<void(const ParamWriteEvent<T>&)>(cbIn, objIn) {}

	PostWriteCallback(const ::hv::common::CallbackImpl<void(const ParamWriteEvent<T>&)> &src):
			::hv::common::CallbackImpl<void(const ParamWriteEvent<T>&)>(src) {}
};

HV_CONFIGURATION_CLOSE_NAMESPACE

#include "param-callback.hpp"

#endif // HV_CONFIGURATION_PARAM_CALLBACK_H
