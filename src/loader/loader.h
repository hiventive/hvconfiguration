/*
 * @file loader.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Loader to preset params reading StorageIf(s)
 */

#ifndef HV_CONFIGURATION_LOADER_H
#define HV_CONFIGURATION_LOADER_H

#include "../configuration/common.h"
#include "../storage/storage-if.h"

#include <vector>

HV_CONFIGURATION_OPEN_NAMESPACE

class Loader {
public:
	Loader(StorageIf* storage);

	Loader(std::vector<StorageIf*> storages);

private:
	std::vector<StorageIf*> storages;
};

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_LOADER_H
