/*
 * @file loader.cpp
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Loader implementation
 */

#include "loader.h"

HV_CONFIGURATION_OPEN_NAMESPACE

Loader::Loader(StorageIf* storage) {
	storages.push_back(storage);
}

Loader::Loader(std::vector<StorageIf*> storages) :
	storages(storages) {
}

HV_CONFIGURATION_CLOSE_NAMESPACE
