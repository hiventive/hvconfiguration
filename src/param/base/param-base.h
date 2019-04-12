/*
 * @file param-base.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive base parameter
 */

#ifndef HV_CONFIGURATION_PARAM_BASE_H
#define HV_CONFIGURATION_PARAM_BASE_H

#include <iostream>
#include <vector>

#include "../../configuration/common.h"
#include "../../configuration/common-cci.h"
#include "../param-if.h"

HV_CONFIGURATION_OPEN_NAMESPACE

template<typename T>
class ParamBase : public ParamIf, public ParamCallbackIf<T> {
	template<typename U, ::cci::cci_param_mutable_type TM> friend class ParamCCI;
public:

	/**
	 * Get parameter name
	 *
	 * @return Parameter name
	 */
	virtual const std::string& getName() const override;

	/**
     * Set parameter value
     *
     * @param value Parameter value
     */
	virtual void setValue(const T& value);

	/**
     * Set parameter
     *
     * @param value Parameter
     *
     * @return Parameter
     */
	ParamBase& operator= (const ParamBase& paramBase);

	/**
     * Set parameter value
     *
     * @param value Parameter value
     *
     * @return Parameter
     */
	ParamBase& operator= (const T& value);

	/**
	 * Get parameter value
	 *
	 * @return Parameter value
	 */
	virtual const T& getValue() const;

	/**
	 * Get parameter value (operator)
	 *
	 * @return Parameter value
	 */
	operator const T& () const;


	/**
	 * Get parameter default value (the one provided during construction)
	 *
	 * @return Parameter default value
	 */
	virtual const T& getDefaultValue() const;

	/**
	 * Indicates whether the current value is the one provided during construction
	 *
	 * @return True if it is the default value, otherwise False
	 */
	bool isDefaultValue() const;

	/**
     * Set human readable parameter description
     *
     * @param description Human readable description
     */
	virtual void setDescription(const std::string& description);

	/**
	 * Get the parameter's description
	 *
     * @return Parameter description
     */
	virtual std::string getDescription() const;

	/**
	 * Reset the parameter to its default value
	 *
	 * @return True if successful otherwise false
     */
	virtual bool reset();

	/**
	 * Indicates whether the parameter has registered callbacks
	 *
	 * @return True if the parameter has registered callbacks otherwise false
     */
	virtual bool hasCallbacks() const override;

	/**
	 * Param a pre read callback
	 *
	 * @param cb Pre read callback
	 *
	 * @return Callback ID
	 */
	virtual ::hv::common::hvcbID_t registerPreReadCallback(const PreReadCallback<T> &cb) override;

	template<typename U>
	::hv::common::hvcbID_t registerPreReadCallback(bool (U::*cb)(const ParamReadEvent<T>&), U *obj);

	/**
	 * Param a post read callback
	 *
	 * @param cb Post read callback
	 *
	 * @return Callback ID
	 */
	virtual ::hv::common::hvcbID_t registerPostReadCallback(const PostReadCallback<T> &cb) override;

	template<typename U>
	::hv::common::hvcbID_t registerPostReadCallback(void (U::*cb)(const ParamReadEvent<T>&), U *obj);

	/**
	 * Param a pre write callback
	 *
	 * @param cb Pre write callback
	 *
	 * @return Callback ID
	 */
	virtual ::hv::common::hvcbID_t registerPreWriteCallback(const PreWriteCallback<T> &cb) override;

	template<typename U>
	::hv::common::hvcbID_t registerPreWriteCallback(bool (U::*cb)(const ParamWriteEvent<T>&), U *obj);

	/**
	 * Param a post write callback
	 *
	 * @param cb Post write callback
	 *
	 * @return Callback ID
	 */
	virtual ::hv::common::hvcbID_t registerPostWriteCallback(const PostWriteCallback<T> &cb) override;

	template<typename U>
	::hv::common::hvcbID_t registerPostWriteCallback(void (U::*cb)(const ParamWriteEvent<T>&), U *obj);

	/**
	 * Unregister a pre read callback
	 *
	 * @param id Callback ID to unregister
	 *
	 * @return True if unregister is a success. Otherwise False.
	 */
	virtual bool unregisterPreReadCallback(const ::hv::common::hvcbID_t &id) override;

	/**
	 * Unregister a post read callback
	 *
	 * @param id Callback ID to unregister
	 *
	 * @return True if unregister is a success. Otherwise False.
	 */
	virtual bool unregisterPostReadCallback(const ::hv::common::hvcbID_t &id) override;

	/**
	 * Unregister a pre write callback
	 *
	 * @param id Callback ID to unregister
	 *
	 * @return True if unregister is a success. Otherwise False.
	 */
	virtual bool unregisterPreWriteCallback(const ::hv::common::hvcbID_t &id) override;

	/**
	 * Unregister a post write callback
	 *
	 * @param id Callback ID to unregister
	 *
	 * @return True if unregister is a success. Otherwise False.
	 */
	virtual bool unregisterPostWriteCallback(const ::hv::common::hvcbID_t &id) override;

	/**
	 * Unregister all callbacks
	 *
	 * @return True if success otherwise false
     */
	virtual bool unregisterAllCallbacks() override;

	/**
	 * Generate a unique callback ID
	 * @return Unique ID
	 */
	::hv::common::hvcbID_t genCallbackID();


	/**
	 * Destructor
     */
	virtual ~ParamBase();

protected:
	/**
	 * Constructor
	 *
     * @param name Parameter name
     */
	ParamBase(const std::string& name,
			const T& defaultValue);

	/**
	 * Constructor
	 *
     * @param name Parameter name
     * @param description Parameter description
     */
	ParamBase(const std::string& name,
			const T& defaultValue,
			const std::string& description);

protected:
	void init();

	bool runPreWriteCallbacks(T value);

protected:
	/// Parameter name
	std::string name;

	/// Parameter value
	T value;

	/// Parameter default value
	T defaultValue;

	/// Parameter description
	std::string description;

private:
	/// Callback vector item
	template<typename U>
	class CallbackMap {
	public:
		CallbackMap() :
			inUse(false) {
		};

		void setCb(::hv::common::hvcbID_t id, U cb) {
			if(!hasID(id)) {
				map.insert(std::pair<::hv::common::hvcbID_t, U>(id, cb));
				//map[id] = cb;
			} else {
				HV_LOG_ERROR("A callback with this ID is already registered.");
			}
		}

		bool hasID(::hv::common::hvcbID_t id) {
			return map.find(id) != map.end() ;
		}

		void erase(::hv::common::hvcbID_t id) {
			if(hasID(id)) {
				map.erase(id);
			}
		}

		bool isEmpty() const {
			return map.empty();
		}

		void clear() {
			map.clear();
		}

		std::map<::hv::common::hvcbID_t, U> getMap() {
			return map;
		}

		void setUsing(bool inUse) {
			this->inUse = inUse;
		}

		bool isUsing() const {
			return this->inUse;
		}
	private:
		std::map<::hv::common::hvcbID_t, U> map;
		mutable bool inUse;
	};

	/// Pre read callbacks
	CallbackMap<PreReadCallback<T> > preReadCallbacks;

	/// Post read callbacks
	CallbackMap<PostReadCallback<T> > postReadCallbacks;

	/// Pre write callbacks
	CallbackMap<PreWriteCallback<T> > preWriteCallbacks;

	/// Post write callbacks
	CallbackMap<PostWriteCallback<T> > postWriteCallbacks;

	/// Callback ID counter
	::hv::common::hvcbID_t cbIDCpt;
};

HV_CONFIGURATION_CLOSE_NAMESPACE

#include "param-base.hpp"

#endif // HV_CONFIGURATION_PARAM_BASE_H
