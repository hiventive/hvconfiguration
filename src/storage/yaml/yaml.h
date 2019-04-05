/*
 * @file yaml.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief YAML configuration loader
 */

#include <iostream>
#include <vector>
#include <map>
#include <yaml-cpp/yaml.h>

#include "../../configuration/common.h"
#include "../storage-if.h"

HV_CONFIGURATION_OPEN_NAMESPACE

class YAML : public StorageIf {
public:
	YAML(const std::string& filename);

public:
	void setValue(const std::string& key, const std::string& value) override;

	std::string getValue(const std::string& key) const override;

	std::map<std::string, std::string> getValues(const std::string& keyPrefix) const override;

	bool hasValue(const std::string& key) const override;

	void deleteValue(const std::string& key) override;

	bool reset() override;

protected:
	std::string getPrefixedKey(const std::string& key) const;

	void parseNode(const ::YAML::Node& node, const std::string& parentKey);

	bool isHexValue(const std::string& value) const;

protected:
	std::map<std::string, std::string> storage;

private:
	const std::string prefix;

	const std::string filepath;
};

HV_CONFIGURATION_CLOSE_NAMESPACE
