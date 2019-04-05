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
	try {
		HV_LOG_TRACE("[HVConfiguration][YAML] Opening {}", filepath);
		::YAML::Node configFile = ::YAML::LoadFile(filepath);
		parseNode(configFile, "");
	} catch (const ::YAML::BadFile& e) {
		HV_LOG_CRITICAL("[HVConfiguration][YAML] Unable to open the configuration file: {}", filepath);
	} catch(const ::YAML::Exception& e) {
		HV_LOG_CRITICAL("[HVConfiguration][YAML] Unable to parse the configuration file: {}", filepath);
	}
}

void YAML::parseNode(const ::YAML::Node& node, const std::string& parentKey) {
	for (::YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
		std::string currentKey = parentKey + it->first.as<std::string>();
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
				HV_LOG_DEBUG("Loaded {} = {}", currentKey,it->second.as<unsigned long>());
				break;
			}
			case ::YAML::NodeType::Null :
				HV_LOG_ERROR("[YAML]: UNSUPPORTED YAML::NodeType::Null");
				break;
			case ::YAML::NodeType::Sequence :
				HV_LOG_ERROR("[YAML]: UNSUPPORTED YAML::NodeType::Sequence");
				break;
			case ::YAML::NodeType::Undefined :
				HV_LOG_ERROR("[YAML]: UNSUPPORTED YAML::NodeType::Undefined");
				break;
		}
	}
}

void YAML::setValue(const std::string& key, const std::string& value) {
	HV_LOG_DEBUG("YAML::setValue");

	storage[getPrefixedKey(key)] = value;
	::YAML::Emitter out;
	out << ::YAML::BeginSeq;
	out << storage;
	out << ::YAML::EndSeq;

	HV_LOG_WARNING("[YAML]: UNSUPPORTED setValue");
}

std::string YAML::getValue(const std::string& key) const {
	if (hasValue(getPrefixedKey(key))) {
		return storage.at(getPrefixedKey(key));
	} else {
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

HV_CONFIGURATION_CLOSE_NAMESPACE
