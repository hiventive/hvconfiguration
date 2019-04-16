/*
 * @file param-callback-if.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date March, 2019
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Parameter callback interface
 */

#ifndef HV_CONFIGURATION_PARAM_CALLBACK_IF_H
#define HV_CONFIGURATION_PARAM_CALLBACK_IF_H

#include "../configuration/common.h"
#include "callback/param-callback.h"

HV_CONFIGURATION_OPEN_NAMESPACE

template<typename T>
class ParamCallbackIf {
public:
	/**
	 * Indicates whether the parameter has registered callbacks
	 *
	 * @return True if the parameter has registered callbacks otherwise false
     */
	virtual bool hasCallbacks() const = 0;

	/**
	 * Param a pre read callback
	 *
	 * @param cb Pre read callback
	 *
	 * @return Callback ID
	 */
	virtual ::hv::common::hvcbID_t registerPreReadCallback(const PreReadCallback<T> &cb) = 0;

	/**
	 * Param a post read callback
	 *
	 * @param cb Post read callback
	 *
	 * @return Callback ID
	 */
	virtual ::hv::common::hvcbID_t registerPostReadCallback(const PostReadCallback<T> &cb) = 0;

	/**
	 * Param a pre write callback
	 *
	 * @param cb Pre write callback
	 *
	 * @return Callback ID
	 */
	virtual ::hv::common::hvcbID_t registerPreWriteCallback(const PreWriteCallback<T> &cb) = 0;

	/**
	 * Param a post write callback
	 *
	 * @param cb Post write callback
	 *
	 * @return Callback ID
	 */
	virtual ::hv::common::hvcbID_t registerPostWriteCallback(const PostWriteCallback<T> &cb) = 0;

	/**
	 * Unregister a pre read callback
	 *
	 * @param id Callback ID to unregister
	 *
	 * @return True if unregister is a success. Otherwise False.
	 */
	virtual bool unregisterPreReadCallback(const ::hv::common::hvcbID_t &id) = 0;

	/**
	 * Unregister a post read callback
	 *
	 * @param id Callback ID to unregister
	 *
	 * @return True if unregister is a success. Otherwise False.
	 */
	virtual bool unregisterPostReadCallback(const ::hv::common::hvcbID_t &id) = 0;

	/**
	 * Unregister a pre write callback
	 *
	 * @param id Callback ID to unregister
	 *
	 * @return True if unregister is a success. Otherwise False.
	 */
	virtual bool unregisterPreWriteCallback(const ::hv::common::hvcbID_t &id) = 0;

	/**
	 * Unregister a post write callback
	 *
	 * @param id Callback ID to unregister
	 *
	 * @return True if unregister is a success. Otherwise False.
	 */
	virtual bool unregisterPostWriteCallback(const ::hv::common::hvcbID_t &id) = 0;

	/**
	 * Unregister all callbacks
	 *
	 * @return True if success otherwise false
     */
	virtual bool unregisterAllCallbacks() = 0;
};

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_PARAM_CALLBACK_IF_H
