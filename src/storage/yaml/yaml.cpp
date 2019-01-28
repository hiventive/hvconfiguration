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

YAML::YAML(const std::string& filename):
		storage() {
	try {
		::YAML::Node configFile = ::YAML::LoadFile(filename);
		parseNode(configFile, "");
	} catch (const ::YAML::BadFile& e) {
		// TODO: replace with HV_LOG(fatal, msg)
		std::cerr << "Unable to open the configuration file: " << filename << std::endl;
	} catch(const ::YAML::Exception& e) {
		std::cerr << "Unable to parse the configuration file: " << e.msg << std::endl;
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
				// TODO: HV_LOG(debug, msg)
				// std::cout << "Loaded " + currentKey << " = " << it->second << std::endl;
				break;
			}
			case ::YAML::NodeType::Null :
				std::cerr << "[YAML]: UNSUPPORTED YAML::NodeType::Null" << std::endl;
				break;
			case ::YAML::NodeType::Sequence :
				std::cerr << "[YAML]: UNSUPPORTED YAML::NodeType::Sequence" << std::endl;
				break;
			case ::YAML::NodeType::Undefined :
				std::cerr << "[YAML]: UNSUPPORTED YAML::NodeType::Undefined" << std::endl;
				break;
		}
	}
}

void YAML::setValue(const std::string& key, const std::string& value) {
	// TODO
	std::cerr << "[YAML]: UNSUPPORTED setValue" << std::endl;
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
