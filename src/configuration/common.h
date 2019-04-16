/*
 * @file common.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Common headers
 */

#ifndef HV_CONFIGURATION_COMMON_H
#define HV_CONFIGURATION_COMMON_H

#include <hv/common/cplusplus.h>
#include <hv/common/log.h>
#include <hv/common/hvutils.h>



// HV Configuration namespace
#define HV_CONFIGURATION_OPEN_NAMESPACE namespace hv { \
	namespace cfg {

#define HV_CONFIGURATION_CLOSE_NAMESPACE } }

HV_CONFIGURATION_OPEN_NAMESPACE

class Broker;
class ParamIf;

Broker* getBroker();
std::string generateRelativeUniqueName(const std::string& name);
bool registerName(const std::string& name);

// Private
void _registerGlobalBroker(Broker* broker);
void _unregisterGlobalBroker();
void _registerParam(ParamIf* param);
void _hasPresetValue(const std::string& name);

template <typename T>
T convertStringToT(const std::string& str);

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_COMMON_H
