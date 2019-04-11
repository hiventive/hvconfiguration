/*
 * @file yaml.cpp
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief YAML configuration loader
 */

#include <iostream>

#include "../../configuration/common.h"
#include "yaml.h"

HV_CONFIGURATION_OPEN_NAMESPACE

YAML::YAML(const std::string& filepath):
		storage(), filepath(filepath) {
	HV_LOG_DEBUG("Opening {}", filepath);
	try {
		::YAML::Node configFile = ::YAML::LoadFile(filepath);
		parseNode(configFile, "");
	} catch (const ::YAML::BadFile& e) {
		HV_LOG_CRITICAL("Unable to open the configuration file: {} with error: {}", filepath, e.what());
	} catch(const ::YAML::Exception& e) {
		HV_LOG_CRITICAL("Unable to parse the configuration file {} with error: {}", filepath, e.what());
		HV_EXIT_FAILURE();
	}
}

void YAML::parseNode(const ::YAML::Node& node, const std::string& parentKey) {
	for (::YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
		std::string currentKey = parentKey + it->first.as<std::string>();
		HV_LOG_TRACE("Map with current key {}", currentKey);
		switch(it->second.Type()) {
			case ::YAML::NodeType::Map :
				parseNode(it->second, currentKey + ".");
				break;
			case ::YAML::NodeType::Scalar : {
				std::string value(it->second.as<std::string>());
				if(isHexValue(value)) {
					// Assume all values are unsigned
					value = std::to_string(std::stoul(value, nullptr, 16));
				}
				storage[currentKey] = value;
				HV_LOG_TRACE("Loaded {} = {}", currentKey, storage[currentKey]);
				break;
			}
			case ::YAML::NodeType::Null :
				HV_LOG_ERROR("UNSUPPORTED YAML::NodeType::Null");
				break;
			case ::YAML::NodeType::Sequence :
				HV_LOG_ERROR("UNSUPPORTED YAML::NodeType::Sequence");
				break;
			case ::YAML::NodeType::Undefined :
				HV_LOG_ERROR("UNSUPPORTED YAML::NodeType::Undefined");
				break;
		}
	}
}

void YAML::setValue(const std::string& key, const std::string& value) {
	HV_LOG_TRACE("YAML::setValue with key {} and value {}", key, value);
	storage[getPrefixedKey(key)] = value;
}

std::string YAML::getValue(const std::string& key) const {
	HV_LOG_TRACE("YAML::getValue with key {}", key);
	if (hasValue(getPrefixedKey(key))) {
		if(hasNonPrefixedValue(getPrefixedKey(key))) {
			return storage.at(getPrefixedKey(key));
		} else {
			std::map<std::string, std::string> valuesWithPrefix = getPrefixedValues(getPrefixedKey(key));
			std::stringstream result;
			result << "{";
			bool first = true;
			for(auto const &entry : valuesWithPrefix) {
				std::string s = entry.first.substr(getPrefixedKey(key).size() + 1, entry.first.size() - getPrefixedKey(key).size() - 1); // .
				if(first) {
					first = false;
				} else {
					result << ", ";
				}
				result << "\"" << s << "\": ";
				if(isNumber(entry.second)) {
					result << entry.second;
				} else {
					result << "\"" << entry.second << "\"";
				}
			}
			result << "}";
			HV_LOG_TRACE("-- YAML::getValue {}", result.str());
			return result.str();
		}
	}
	else {
		return std::string();
	}
}

std::map<std::string, std::string> YAML::getValues(const std::string& keyPrefix) const {
	std::map<std::string, std::string> result;
	if(!keyPrefix.empty()) {
		for(auto const &entry : storage) {
			if (entry.first.find(keyPrefix) != std::string::npos) {
				result[entry.first] = entry.second;
			}
		}
		return result;
	} else {
		return storage;
	}
}

bool YAML::hasValue(const std::string& key) const {
	return hasNonPrefixedValue(key) || hasPrefixedValue(getPrefixedKey(key));
}

bool YAML::hasNonPrefixedValue(const std::string& key) const {
	return storage.find(getPrefixedKey(key)) != storage.end();
}

void YAML::deleteValue(const std::string& key) {
	if (hasValue(getPrefixedKey(key))) {
		storage.erase(getPrefixedKey(key));
	}
}

bool YAML::reset() {
	storage.clear();
	return true;
}

std::string YAML::getPrefixedKey(const std::string& key) const {
	if(!prefix.empty()) {
#if defined(_MSC_VER) && _MSC_VER <= 1700
		std::stringstream prefixedKey;
		prefixedKey << prefix << HV_CONFIGURATION_STORAGE_SEPARATOR << key;
		return prefixedKey.str();
#else
		return prefix + HV_CONFIGURATION_STORAGE_SEPARATOR + key;
#endif
	} else {
		return key;
	}
}

bool YAML::isHexValue(const std::string& value) const {
	return value.compare(0, 2, "0x") == 0
		   && value.size() > 2
		   && value.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
}

// Specific to YAML, a MAP can be available
bool YAML::hasPrefixedValue(const std::string& searchPrefix) const {
	std::map<std::string, std::string>::const_iterator i = storage.lower_bound(searchPrefix);
	if (i != storage.end()) {
		const std::string& key = i->first;
		if (key.compare(0, searchPrefix.size(), searchPrefix) == 0)
			return true;
		HV_LOG_TRACE("YAML::hasPrefixedValue Results for prefix {}", searchPrefix);
	}
	HV_LOG_TRACE("YAML::hasPrefixedValue No result for prefix {}", searchPrefix);
	return false;
}

// Specific to YAML, a MAP can be available
std::map<std::string, std::string> YAML::getPrefixedValues(const std::string& searchPrefix) const {
			std::map<std::string, std::string> result;
	for(auto const &entry : storage) {
		if (entry.first.rfind(searchPrefix, 0) == 0) {
			result[entry.first] = entry.second;
		}
	}
	return result;
}

bool YAML::isNumber(const std::string& s) const
{
	return !s.empty() && std::find_if(s.begin(),
									  s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

HV_CONFIGURATION_CLOSE_NAMESPACE
