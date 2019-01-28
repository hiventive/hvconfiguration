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
class ParamBase : public ParamIf {
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
	virtual bool hasCallbacks() const;

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
	/// Parameter name
	std::string name;

	/// Parameter value
	T value;

	/// Parameter default value
	T defaultValue;

	/// Parameter description
	std::string description;

private:
	/// Callback item to store a callback
	template<typename U>
	class CallbackItem {
	public:
		CallbackItem(U callback) :
			callback(callback) {
		};
	private:
		T callback;
	};

	/// Callback vector item
	template<typename U>
	class CallbackVector {
	public:
		CallbackVector() :
			inUse(false) {
		};
	private:
		std::vector<U> callbackVector;
		mutable bool inUse;
	};

	/// Pre read callbacks
	//CallbackVector<CallbackItem<::cci::cci_callback_untyped_handle::type> > preReadCallbacks;
	// TODO

	/// Post read callbacks
	//CallbackVector<CallbackItem<::cci::cci_callback_untyped_handle::type> > postReadCallbacks;
	// TODO

	/// Pre write callbacks
	//CallbackVector<CallbackItem<::cci::cci_callback_untyped_handle::type> > preWriteCallbacks;
	// TODO

	/// Post write callbacks
	//CallbackVector<CallbackItem<::cci::cci_callback_untyped_handle::type> > postWriteCallbacks;
	// TODO
};

HV_CONFIGURATION_CLOSE_NAMESPACE

#include "param-base.hpp"

#endif // HV_CONFIGURATION_PARAM_BASE_H
