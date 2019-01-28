/*
 * @file environment.cpp
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Environment storage
 */

#include <cstdlib>
#include <cstdio>
#include <sstream>

#include "environment.h"

HV_CONFIGURATION_OPEN_NAMESPACE

extern char **environ;

Environment::Environment() : storage() {
	std::map<std::string, std::string> storage;
	int i = 1;
	char *s = *environ;

	for (; s; i++) {
		std::string envVar = std::string(s);
		std::string::size_type index = envVar.find("=");
		if (index == std::string::npos) {
			// TODO: error, unable to read env var
		} else {
			std::string envVarName = envVar.substr(0, index);
			std::string envVarValue = envVar.substr(index + 1);
			storage[envVarName] = envVarValue;
		}
		s = *(environ+i);
	}
}

void Environment::setValue(const std::string& key, const std::string& value) {
#ifdef _WIN32
	_putenv_s(getPrefixedKey(key).c_str(), value.c_str());
#else
	setenv(getPrefixedKey(key).c_str(), value.c_str(), true);
#endif
}

std::string Environment::getValue(const std::string& key) const {
	if (hasValue(getPrefixedKey(key))) {
		return storage.at(getPrefixedKey(key));
	} else {
		return std::string();
	}
}

std::map<std::string, std::string> Environment::getValues(const std::string& keyPrefix) const {
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

bool Environment::hasValue(const std::string& key) const {
	return storage.find(getPrefixedKey(key)) != storage.end();
}

void Environment::deleteValue(const std::string& key) {
#ifdef _WIN32
		_putenv_s(getPrefixedKey(key).c_str(), "");
#else
		unsetenv(getPrefixedKey(key).c_str());
#endif
		storage.erase(getPrefixedKey(key));
}

bool Environment::reset() {
	for(auto const &entry : storage) {
		deleteValue(entry.first);
	}
	storage.clear();
	return true;
}

std::string Environment::getPrefixedKey(const std::string& key) const {
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
