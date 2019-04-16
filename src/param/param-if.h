/*
 * @file param-if.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief Parameter interface
 */

#ifndef HV_CONFIGURATION_PARAM_IF_H
#define HV_CONFIGURATION_PARAM_IF_H

#include "../configuration/common.h"
#include "base/param-base.h"
#include "param-callback-if.h"

HV_CONFIGURATION_OPEN_NAMESPACE

template <typename T>
class ParamBase;

enum NameType {
	RELATIVE_NAME,
	ABSOLUTE_NAME
};

class ParamIf {
public:
	virtual const std::string& getName() const = 0;

	template<typename T>
	ParamBase<T>* getParamTyped() const {
		return static_cast< ParamBase<T>* >(this);
	}

/*private:
	virtual void setName(const std::string& name) = 0;*/
};

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_PARAM_IF_H
