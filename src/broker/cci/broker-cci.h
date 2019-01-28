/*
 * @file broker-cci.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief CCI compliant broker implementaton
 */

#ifndef HV_CONFIGURATION_BROKER_CCI_H
#define HV_CONFIGURATION_BROKER_CCI_H

#include "../../configuration/common.h"
#include "../../configuration/common-cplusplus.h"
#include "../../storage/storage-if.h"
#include "../base/broker-base.h"
#include "broker-cci-if-helper.h"
#include "../../configuration/common-cci.h"

HV_CONFIGURATION_OPEN_NAMESPACE

class BrokerCCI : public BrokerCCIIfHelper {
	friend class Broker;

public:
	// Broker properties
	/// @copydoc cci_broker_if::name
	const char* CCIName() const override;

	/// @copydoc cci_broker_if::is_global_broker
	bool is_global_broker() const override;

	// Parameter access
	/// @copydoc cci_broker_if::get_param_handle
	::cci::cci_param_untyped_handle get_param_handle(const std::string& paramName,
			const ::cci::cci_originator& originator) const override;

	/// @copydoc cci_broker_if::get_value_origin
	::cci::cci_originator get_value_origin(const std::string& paramName) const override;

	/// @copydoc cci_broker_if::get_cci_value
	::cci::cci_value get_cci_value(const std::string& paramName,
			const ::cci::cci_originator& originator) const override;

	// Bulk parameter access
	/// @copydoc cci_broker_if::get_param_handles
	std::vector<::cci::cci_param_untyped_handle> get_param_handles(
			const ::cci::cci_originator& originator) const override;

	/// @copydoc cci_broker_if::get_param_handles
	::cci::cci_param_range get_param_handles(::cci::cci_param_predicate& pred,
			const ::cci::cci_originator& originator) const override;

	// Parameter initialization
	/// @copydoc cci_broker_if::has_preset_value
	bool has_preset_value(const std::string& paramName) const override;

	/// @copydoc cci_broker_if::set_preset_cci_value
	void set_preset_cci_value(
			const std::string& paramName,
			const ::cci::cci_value& cciValue,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_broker_if::get_preset_cci_value
	::cci::cci_value get_preset_cci_value(const std::string& paramName) const override;

	/// @copydoc cci_broker_if::get_preset_value_origin
	::cci::cci_originator get_preset_value_origin(const std::string& paramName) const override;

	/// @copydoc cci_broker_if::lock_preset_value
	void lock_preset_value(const std::string& paramName) override;

	/// @copydoc cci_broker_if::get_unconsumed_preset_values
	std::vector<::cci::cci_name_value_pair> get_unconsumed_preset_values() const override;

	/// @copydoc cci_broker_if::get_unconsumed_preset_values
	::cci::cci_preset_value_range get_unconsumed_preset_values(
			const ::cci::cci_preset_value_predicate& pred) const override;

	/// @copydoc cci_broker_if::ignore_unconsumed_preset_values
	void ignore_unconsumed_preset_values(
			const ::cci::cci_preset_value_predicate& pred) override;

	// Handle creation
	/// @copydoc cci_broker_if::create_broker_handle
	// FIXME: add override with a clean cci_broker_if
	/* ::cci::cci_broker_handle create_broker_handle(
			const ::cci::cci_originator& originator) const; */

	// Callbacks
	/// @copydoc cci_broker_if::cci_param_create_callback_handle
	::cci::cci_param_create_callback_handle register_create_callback(
			const ::cci::cci_param_create_callback& callback,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_broker_if::unregister_create_callback
	bool unregister_create_callback(const ::cci::cci_param_create_callback_handle& callback,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_broker_if::register_destroy_callback
	::cci::cci_param_destroy_callback_handle register_destroy_callback(
			const ::cci::cci_param_destroy_callback& callback,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_broker_if::unregister_destroy_callback
	bool unregister_destroy_callback(
			const ::cci::cci_param_destroy_callback_handle& callback,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_broker_if::unregister_all_callbacks
	bool unregister_all_callbacks(const ::cci::cci_originator& originator) override;

	/// @copydoc cci_broker_if::has_callbacks
	bool has_callbacks() const override;

	// Parameter un/registration
	/// @copydoc cci_broker_if::add_param
	void add_param(::cci::cci_param_if* param) override;

	/// @copydoc cci_broker_if::remove_param
	void remove_param(::cci::cci_param_if* param) override;

	/// @copydoc cci_broker_if::create_broker_handle
	::cci::cci_broker_handle create_broker_handle(const ::cci::cci_originator& originator) const override;

protected:
	~BrokerCCI();

	// Disabled
	BrokerCCI(BrokerCCI&&) HV_CPLUSPLUS_MEMBER_FUNCTION_DELETE;

	BrokerCCI(const BrokerCCI&) HV_CPLUSPLUS_MEMBER_FUNCTION_DELETE;

	BrokerCCI& operator=(BrokerCCI&&) HV_CPLUSPLUS_MEMBER_FUNCTION_DELETE;

	BrokerCCI& operator=(const BrokerCCI&) HV_CPLUSPLUS_MEMBER_FUNCTION_DELETE;

protected:
	BrokerCCI(BrokerBase& brokerBase, StorageIf* storage = nullptr, bool registerCCI = true);

private:
	::cci::cci_param_if* getCCIParam(const std::string& paramName) const;

	std::map<std::string, ::cci::cci_param_if*> getCCIParams() const;

	void setCCIParam(::cci::cci_param_if* param);

	bool hasCCIParam(const std::string& paramName) const;

	void removeCCIParam(::cci::cci_param_if* param);

	::cci::cci_value getCCIPreset(const std::string& paramName) const;

	void setCCIPreset(const std::string& paramName, const ::cci::cci_value& value);

	bool hasCCIPreset(const std::string& paramName) const;

	void setCCIPresetUsed(const std::string& paramName, bool used) const;

	bool isCCIPresetUsed(const std::string& paramName) const;

	void setCCIPresetLocked(const std::string& paramName) const;

	bool isCCIPresetLocked(const std::string& paramName) const;

	std::vector<std::string> getCCIPresetUsed(bool used) const;

	::cci::cci_originator getCCIPresetOriginator(const std::string& paramName) const;

	void setCCIPresetOriginator(const std::string& paramName, const ::cci::cci_originator& originator);

	bool hasCCIPresetOriginator(const std::string& paramName) const;

private:
	/// Broker base
	BrokerBase& brokerBase;

	/// Params
	std::map<std::string, ::cci::cci_param_if*> params;

	/// Presets
	StorageIf* presets;

	/// Wether presets should be removed
	bool deleteStorage;

	/// Preset originators
	std::map<std::string, ::cci::cci_originator> presetOriginators;

	/// Create callbacks
	std::vector<CCICallbackObject<::cci::cci_param_create_callback_handle::type> > createCallbacks;

	/// Destroy callbacks
	std::vector<CCICallbackObject<::cci::cci_param_destroy_callback_handle::type> > destroyCallbacks;

	/// IgnoredUnconsumedPredicates
	std::vector<::cci::cci_preset_value_predicate> ignoredUnconsumedPredicates;
};

HV_CONFIGURATION_CLOSE_NAMESPACE

#endif // HV_CONFIGURATION_BROKER_CCI_H
