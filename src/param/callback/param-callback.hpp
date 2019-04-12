/**
 * @file param-callback.cpp
 * @author Guillaume Delbergue <guillaulme.delbergue@hiventive.com>
 * @date March, 2019
 * @copyright Copyright (C) 2019, Hiventive.
 *
 * @brief Parameter callback implementation
 */

#ifndef HV_CONFIGURATION_PARAM_CALLBACK_IMPL_H
#define HV_CONFIGURATION_PARAM_CALLBACK_IMPL_H

#include "param-callback.h"

HV_CONFIGURATION_OPEN_NAMESPACE

template<typename T>
ParamReadEvent<T>::ParamReadEvent(const T& value, const ParamIf& param):
		value(value), ph(param) {
}

template<typename T>
ParamWriteEvent<T>::ParamWriteEvent(const T& oldValue, const T& newValue, const ParamIf& param):
		oldValue(oldValue), newValue(newValue), ph(param) {
}

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_PARAM_CALLBACK_IMPL_H
