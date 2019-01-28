/*
 * @file environment.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Environment storage
 */

#ifndef HV_CONFIGURATION_STORAGE_ENVIRONMENT_H
#define HV_CONFIGURATION_STORAGE_ENVIRONMENT_H

#include <iostream>
#include <vector>
#include <map>

#include "../../configuration/common.h"
#include "../storage-if.h"

HV_CONFIGURATION_OPEN_NAMESPACE

class Environment : public StorageIf {
public:
	Environment();

	~Environment() override HV_CPLUSPLUS_MEMBER_FUNCTION_DEFAULT;

public:
	void setValue(const std::string& key, const std::string& value) override;

	std::string getValue(const std::string& key) const override;

	std::map<std::string, std::string> getValues(const std::string& keyPrefix) const override;

	bool hasValue(const std::string& key) const override;

	void deleteValue(const std::string& key) override;

	bool reset() override;

protected:
	std::string getPrefixedKey(const std::string& key) const;

	std::map<std::string, std::string> storage;

private:
	const std::string prefix;
};

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_STORAGE_ENVIRONMENT_H
