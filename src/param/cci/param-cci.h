/*
 * @file param-cci.h
 * @author Guillaume Delbergue <guillaume.delbergue@hiventive.com>
 * @date June, 2018
 * @copyright Copyright (C) 2018, Hiventive.
 *
 * @brief CCI compliant parameter implementaton
 */

#ifndef HV_CONFIGURATION_PARAM_CCI_H
#define HV_CONFIGURATION_PARAM_CCI_H

#include <cci_configuration>

#include "../../configuration/common.h"

HV_CONFIGURATION_OPEN_NAMESPACE

template<typename T,
        ::cci::cci_param_mutable_type TM = ::cci::CCI_MUTABLE_PARAM>
class ParamCCI : public ::cci::cci_param_if {
public:
	/// @copydoc cci_param_if::get_name
	const char* name() const override;

	/// @copydoc cci_param_if::set_description
	void set_description(const std::string& description) override;

	/// @copydoc cci_param_if::get_description
	std::string get_description() const override;

	/// @copydoc cci_param_if::get_data_category
	::cci::cci_param_data_category get_data_category() const override;

	/// @copydoc cci_param_if::get_type_info
	const std::type_info &get_type_info() const override;

	/// @copydoc cci_param_if::get_mutable_type
	::cci::cci_param_mutable_type get_mutable_type() const override;

	/// @copydoc cci_param_if::
	using cci_param_if::set_cci_value;

	/// @copydoc cci_param_if::set_cci_value
	void set_cci_value(const ::cci::cci_value& cciValue,
			const void *password,
			const ::cci::cci_originator &originator) override;

	/// @copydoc cci_param_if::get_cci_value
	using cci_param_if::get_cci_value;

	/// @copydoc cci_param_if::get_cci_value
	::cci::cci_value get_cci_value(const ::cci::cci_originator &originator) const override;

	/// @copydoc cci_param_if::get_default_cci_value
	::cci::cci_value get_default_cci_value() const override;

	/// @copydoc cci_param_if::is_default_value
	bool is_default_value() const override;

	/// @copydoc cci_param_if::is_preset_value
	bool is_preset_value() const override;

	/// @copydoc cci_param_if::lock
	bool lock(const void* password) override;

	/// @copydoc cci_param_if::reset
	bool reset() override;

	/// @copydoc cci_param_if::get_originator
	::cci::cci_originator get_originator() const override;

	/// @copydoc cci_param_if::get_value_origin
	::cci::cci_originator get_value_origin() const override;

	/// @copydoc cci_param_if::unlock
	bool unlock(const void *password) override;

	/// @copydoc cci_param_if::is_locked
	bool is_locked() const override;

	/// @copydoc cci_param_if::get_metadata
	::cci::cci_value_map get_metadata() const override;

	/// @copydoc cci_param_if::add_metadata
	void add_metadata(const std::string& name,
					  const ::cci::cci_value& cciValue,
					  const std::string& description) override;

	/// @copydoc cci_param_callback_if::register_pre_write_callback
	::cci::cci_callback_untyped_handle register_pre_write_callback(
			const ::cci::cci_callback_untyped_handle& callback,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_param_callback_if::unregister_pre_write_callback
	bool unregister_pre_write_callback(
			const ::cci::cci_callback_untyped_handle& callback,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_param_callback_if::register_post_write_callback
	::cci::cci_callback_untyped_handle register_post_write_callback(
			const ::cci::cci_callback_untyped_handle& callback,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_param_callback_if::unregister_post_write_callback
	bool unregister_post_write_callback(
			const ::cci::cci_callback_untyped_handle& callback,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_param_callback_if::register_pre_read_callback
	::cci::cci_callback_untyped_handle register_pre_read_callback(
			const ::cci::cci_callback_untyped_handle& callback,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_param_callback_if::unregister_pre_read_callback
	bool unregister_pre_read_callback(
			const ::cci::cci_callback_untyped_handle& callback,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_param_callback_if::register_post_read_callback
	::cci::cci_callback_untyped_handle register_post_read_callback(
			const ::cci::cci_callback_untyped_handle& callback,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_param_callback_if::unregister_post_read_callback
	bool unregister_post_read_callback(
			const ::cci::cci_callback_untyped_handle& callback,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_param_callback_if::has_callbacks
	bool has_callbacks() const override;

	/// @copydoc cci_param_callback_if::unregister_all_callbacks
	bool unregister_all_callbacks(const ::cci::cci_originator& orig) override;

public:
	ParamCCI(ParamBase<T>& paramBase,
			::cci::cci_name_type nameType = ::cci::CCI_RELATIVE_NAME,
			const ::cci::cci_originator& originator = ::cci::cci_originator());

	ParamCCI(ParamBase<T>& paramBase,
			::cci::cci_broker_handle privateBroker,
			::cci::cci_name_type nameType = ::cci::CCI_RELATIVE_NAME,
			const ::cci::cci_originator& originator = ::cci::cci_originator());

	~ParamCCI() override;

private:
	/// @copydoc cci_param_if::preset_cci_value
	void preset_cci_value(const ::cci::cci_value&,
			const ::cci::cci_originator&) override;

	/// @copydoc cci_param_if::set_raw_value
	void set_raw_value(const void* value,
			const void* password,
			const ::cci::cci_originator& originator) override;

	/// @copydoc cci_param_if::get_raw_value
	const void* get_raw_value(const ::cci::cci_originator& originator) const override;

	/// @copydoc cci_param_if::get_raw_default_value
	const void* get_raw_default_value() const override;

	/// @copydoc cci_param_if::add_param_handle
	void add_param_handle(::cci::cci_param_untyped_handle* paramHandle) override;

	/// @copydoc cci_param_if::remove_param_handle
	void remove_param_handle(::cci::cci_param_untyped_handle* paramHandle) override;

	/// @copydoc cci_param_if::invalidate_all_param_handles
	void invalidate_all_param_handles() override;

private:
	/// Parameter base
	ParamBase<T>& paramBase;

	/// Parameter originator
	const ::cci::cci_originator originator;

	/// Parameter handles vector
	std::vector<::cci::cci_param_untyped_handle*> paramHandles;

	/// Metadata
	::cci::cci_value_map metadata;

	/// Lock password
	const void* lockPassword;

	/// Broker handle
	::cci::cci_broker_handle brokerHandle;

	/// Latest writer originator
	mutable ::cci::cci_originator valueOriginator;

	/// Pre write callbacks
	std::vector<CCICallbackObject<::cci::cci_callback_untyped_handle::type> > preWriteCallbacks;

	/// Post write callbacks
	std::vector<CCICallbackObject<::cci::cci_callback_untyped_handle::type> > postWriteCallbacks;

	/// Pre read callbacks
	std::vector<CCICallbackObject<::cci::cci_callback_untyped_handle::type> > preReadCallbacks;

	/// Post read callbacks
	std::vector<CCICallbackObject<::cci::cci_callback_untyped_handle::type> > postReadCallbacks;
};

HV_CONFIGURATION_CLOSE_NAMESPACE

#include "param-cci.hpp"

#endif // HV_CONFIGURATION_PARAM_CCI_H
