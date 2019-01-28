/*
 * @file param.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Hiventive parameter
 */

#ifndef HV_CONFIGURATION_PARAM_H
#define HV_CONFIGURATION_PARAM_H

#include "base/param-base.h"
#include "cci/param-cci.h"

HV_CONFIGURATION_OPEN_NAMESPACE

template<typename T, ::cci::cci_param_mutable_type TM = ::cci::CCI_MUTABLE_PARAM>
class Param : public virtual ParamBase<T> {
public:
	Param(const std::string& name,
			const T& defaultValue,
			const std::string& description = "",
			::cci::cci_name_type nameType = ::cci::CCI_RELATIVE_NAME,
			const ::cci::cci_originator& originator = ::cci::cci_originator());

	Param(const std::string& name,
			const ::cci::cci_value& defaultValue,
			const std::string& description = "",
			::cci::cci_name_type nameType = ::cci::CCI_RELATIVE_NAME,
			const ::cci::cci_originator& originator = ::cci::cci_originator());

	Param(const std::string& name,
			const T& defaultValue,
		    ::cci::cci_broker_handle privateBroker,
			const std::string& description = "",
			::cci::cci_name_type nameType = ::cci::CCI_RELATIVE_NAME,
			const ::cci::cci_originator& originator = ::cci::cci_originator());

	Param(const std::string& name,
			const ::cci::cci_value& defaultValue,
		  	::cci::cci_broker_handle privateBroker,
			const std::string& description = "",
			::cci::cci_name_type nameType = ::cci::CCI_RELATIVE_NAME,
			const ::cci::cci_originator& originator = ::cci::cci_originator());

	using ParamBase<T>::operator=;

	// FIXME
	// using ParamBase<T>::operator const T& ();

	~Param();

private:
	ParamCCI<T, TM> paramCCI;
};

HV_CONFIGURATION_CLOSE_NAMESPACE

#include "param.hpp"

#endif // HV_CONFIGURATION_PARAM_H
