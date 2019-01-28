/*
 * @file param.hpp
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive parameter implementation
 */

#ifndef HV_CONFIGURATION_PARAM_IMPL_H
#define HV_CONFIGURATION_PARAM_IMPL_H

#include "param.h"

HV_CONFIGURATION_OPEN_NAMESPACE

/* template<typename T, cci::cci_param_mutable_type TM>
Param<T, TM>::Param(const std::string& name,
		const T& defaultValue,
		const std::string& description) :
		  ParamBase<T>(name, defaultValue, description),
		  paramCCI(*this) {
} */

template<typename T, ::cci::cci_param_mutable_type TM>
Param<T, TM>::Param(const std::string& name,
	  const T& defaultValue,
	  const std::string& description,
	  ::cci::cci_name_type nameType,
	  const ::cci::cci_originator& originator) :
		ParamBase<T>(name, defaultValue, description),
		paramCCI(*this, nameType, originator) {

}

template<typename T, ::cci::cci_param_mutable_type TM>
Param<T, TM>::Param(const std::string& name,
	  const ::cci::cci_value& defaultValue,
	  const std::string& description,
	  ::cci::cci_name_type nameType,
	  const ::cci::cci_originator& originator) :
		ParamBase<T>(name, defaultValue),
		paramCCI(*this, nameType, originator) {
	// FIXME: handle cci_value
}

template<typename T, ::cci::cci_param_mutable_type TM>
Param<T, TM>::Param(const std::string& name,
	  const T& defaultValue,
	  ::cci::cci_broker_handle privateBroker,
	  const std::string& description,
	  ::cci::cci_name_type nameType,
	  const ::cci::cci_originator& originator) :
		ParamBase<T>(name, defaultValue),
		paramCCI(*this, privateBroker, nameType, originator) {

}

template<typename T, ::cci::cci_param_mutable_type TM>
Param<T, TM>::Param(const std::string& name,
	  const ::cci::cci_value& defaultValue,
	  ::cci::cci_broker_handle privateBroker,
	  const std::string& description,
	  ::cci::cci_name_type nameType,
	  const ::cci::cci_originator& originator) :
		ParamBase<T>(name, defaultValue),
		paramCCI(*this, privateBroker, nameType, originator) {
	// FIXME: handle cci_value
}


template<typename T, ::cci::cci_param_mutable_type TM>
Param<T, TM>::~Param() {
}

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_PARAM_IMPL_H
