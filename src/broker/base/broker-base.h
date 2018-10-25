/*
 * @file broker-base.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive base broker
 */

#ifndef HV_CONFIGURATION_BROKER_BASE_H
#define HV_CONFIGURATION_BROKER_BASE_H

#include <iostream>
#include <vector>

#include "../../configuration/common.h"
#include "../../storage/memory/memory.h"
#include "../../storage/storage-if.h"
#include "../../param/base/param-base.h"

HV_CONFIGURATION_OPEN_NAMESPACE

class BrokerBase {
public:
	/**
	 * Get parameter name
	 *
	 * @return Parameter name
	 */
	virtual const std::string& getName() const;

	/**
	 * Get parameter value by name
	 *
	 * @param paramName Parameter name
	 *
	 * @return Parameter value
	 */
	template <typename T>
	const T& getValue(const std::string& paramName) const;

	/**
	 * Set parameter value by name
	 *
	 * @param paramName Parameter name
	 * @param value Parameter value
	 */
	template <typename T>
	void setValue(const std::string& paramName,
			const T& value);

	/**
	 * Get parameter preset value by name used for parameter initialization
	 *
	 * @param paramName Parameter name
	 *
	 * @return Parameter preset value
	 */
	template <typename T>
	const T& getPresetValue(const std::string &paramName);

	/**
	 * Set parameter preset value by name used for parameter initialization
	 *
	 * @param paramName Parameter name
	 * @param value Parameter preset value
	 */
	template <typename T>
	void setPresetValue(const std::string& paramName,
			const T& presetValue);


	/**
	 * Get unconsumed preset values (for which no parameter is ever created)
	 *
	 * @return Vector of pair of name / value of parameters
	 */
	template <typename T>
	std::vector<std::pair<std::string, T> > getUnconsumedPresetValues() const;

	// TODO
	/* virtual PresetValueRange getUnconsumedPresetValues(
			const PresetValuePredicate &pred) const = 0;

	// TODO
	virtual void ignoreUnconsumedPresetValues(
			const PresetValuePredicate &pred) = 0; */

	/**
	 * Lock a parameter's preset value to avoid override.
	 *
	 * @param paramName Parameter name
	 */
	virtual void lockPresetValue(const std::string& paramName);

	/**
	 * Check if the parameter has a preset value
	 *
	 * @param paramName Parameter name
	 *
	 * @return True if the parameter has a preset value, otherwise False
	 */
	virtual bool hasPresetValue(const std::string& paramName) const;


	/**
	 * Add a parameter to the broker
	 *
	 * @param paramBase Parameter to add
	 */
	virtual void addParam(ParamIf* paramBase);


	/**
	 * Remove a parameter from the broker
	 *
	 * @param paramBase Parameter to remove
	 */
	virtual void removeParam(ParamIf* paramBase);

	/**
	 * Check if a parameter is registered with provided name
	 *
	 * @param paramBase Parameter to look for
	 *
	 * @return True if a parameter exists, otherwise False
	 */
	virtual bool hasParam(const std::string& paramName) const;

	/**
	 * Get all registered parameters
	 *
	 * @return Vector of parameters
	 */
	virtual std::vector<ParamIf*> getParams() const;

	// TODO
	// virtual ParamRange getParams(ParamPredicate& pred) const;

	/**
	 * Destructor
     */
	virtual ~BrokerBase();

protected:
	/**
	 * Constructor
	 *
     * @param name Broker name
     */
	BrokerBase(const std::string& name,
			StorageIf* storage = nullptr);

protected:
	/// Broker name
	const std::string name;

	/// Broker params storage
	std::map<std::string, ParamIf*> params;

	/// Broker presets storage
	StorageIf* presets;

	/// Wether storage should be removed
	bool deleteStorage;
};

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_BROKER_BASE_H
