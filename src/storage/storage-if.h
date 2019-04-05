/*
 * @file storage-if.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Storage interface
 */

#ifndef HV_CONFIGURATION_STORAGE_IF_H
#define HV_CONFIGURATION_STORAGE_IF_H

#include "../configuration/common.h"

#include <iostream>
#include <map>

#define HV_CONFIGURATION_STORAGE_SEPARATOR "."

HV_CONFIGURATION_OPEN_NAMESPACE

class StorageIf {
public:
	virtual void setValue(const std::string& key, const std::string& value) = 0;

	virtual std::string getValue(const std::string& key) const = 0;

	virtual std::map<std::string, std::string> getValues(const std::string& keyPrefix = "") const = 0;

	virtual bool hasValue(const std::string& key) const = 0;

	virtual void deleteValue(const std::string& key) = 0;

	virtual bool reset() = 0;

	// FIXME
	// virtual StorageIf& getObject(const std::string& key) const = 0;

	virtual ~StorageIf() HV_CPLUSPLUS_MEMBER_FUNCTION_DEFAULT;

	/* template<typename T>
	virtual const T& getValues() const = 0; */
};

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_STORAGE_IF_H
