/*
 * @file memory.cpp
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief In-Memory storage
 */

#include <sstream>

#include "memory.h"

HV_CONFIGURATION_OPEN_NAMESPACE

Memory::Memory() : storage() {
}

void Memory::setValue(const std::string& key, const std::string& value) {
	storage[getPrefixedKey(key)] = value;
}

std::string Memory::getValue(const std::string& key) const {
	if (hasValue(getPrefixedKey(key))) {
		return storage.at(getPrefixedKey(key));
	} else {
		return std::string();
	}
}

std::map<std::string, std::string> Memory::getValues(const std::string& keyPrefix) const {
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

bool Memory::hasValue(const std::string& key) const {
	return storage.find(getPrefixedKey(key)) != storage.end();
}

void Memory::deleteValue(const std::string& key) {
	if (hasValue(getPrefixedKey(key))) {
		storage.erase(getPrefixedKey(key));
	}
}

bool Memory::reset() {
	storage.clear();
	return true;
}

std::string Memory::getPrefixedKey(const std::string& key) const {
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

HV_CONFIGURATION_CLOSE_NAMESPACE
