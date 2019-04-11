#include <hv/common/log-level.h>
#define HV_LOG_ACTIVE_LEVEL HV_LOG_LEVEL_TRACE

#include <systemc>
#include <hv/configuration.h>
#include <cci_configuration>

class ConfigModule : public sc_core::sc_module {
	SC_HAS_PROCESS(ConfigModule);
public:
	ConfigModule(sc_core::sc_module_name name) :
		sc_core::sc_module(name),
		intParam("intParam", 5),
		boolParam("boolParam", false) {
		SC_THREAD(example);

		// Register CCI callbacks on intParam
		preReadCb = boolParam.register_pre_read_callback(&ConfigModule::typedPreReadCallback, this);
		preWriteCb = boolParam.register_pre_write_callback(&ConfigModule::typedPreWriteCallback, this);
		postWriteCb = boolParam.register_post_write_callback(&ConfigModule::typedPostWriteCallback, this);
	}
private:
	void example() {
		intParam = 0xBEEF;
		HV_LOG_INFO("ConfigModule.intParam = 0x{:x}", intParam);
	}

	// CCI Pre Read Callback
	void typedPreReadCallback(const cci::cci_param_read_event<bool>& ev) {
		HV_LOG_INFO("typedPreReadCallback: {} = {} by {}", ev.param_handle.name(), ev.value, ev.originator.name());
	}

	// CCI Pre Write Callback
	bool typedPreWriteCallback(const cci::cci_param_write_event<bool>& ev) {
		HV_LOG_INFO("typedPreWriteCallback: {} update from {} to {} by {}", ev.param_handle.name(),
				ev.old_value, ev.new_value, ev.originator.name());
		return true;
	}

	// CCI Post Write Callback
	void typedPostWriteCallback(const cci::cci_param_write_event<bool>& ev) {
		HV_LOG_INFO("typedPostWriteCallback: {} updated from {} to {} by {}", ev.param_handle.name(),
				ev.old_value, ev.new_value, ev.originator.name());
	}

private:
	hv::cfg::Param<int> intParam;
	cci::cci_param<bool> boolParam;

	// Callback pointers
	cci::cci_callback_untyped_handle preReadCb;
	cci::cci_callback_untyped_handle preWriteCb;
	cci::cci_callback_untyped_handle postWriteCb;
};

class SimulationModule : public sc_core::sc_module {
	SC_HAS_PROCESS(SimulationModule);
public:
	SimulationModule(sc_core::sc_module_name name) :
			sc_core::sc_module(name),
			stringParam("stringParam", "test"),
			hvBroker(hv::cfg::getBroker()),
			cciBroker(cci::cci_get_broker()) {
		SC_THREAD(example);
	}
private:
	void example() {
		HV_LOG_INFO("SimulationModule.stringParam = {}\n", stringParam.getValue());

		HV_LOG_INFO("Hiventive Callback usage example");
		std::vector<hv::cfg::ParamIf*> params = hvBroker->getParams();
		for(auto const &param : params) {
			HV_LOG_TRACE("{}", param->getName());
		}

		if(!hvBroker->hasParam("ConfigModule.intParam")) {
			HV_LOG_ERROR("Unable to find ConfigModule.intParam parameter\n");
		}
		hv::cfg::Param<int>* intParam =
				dynamic_cast<hv::cfg::Param<int>*>(hvBroker->getParam("ConfigModule.intParam"));
		// ->getParamTyped<int>()

		//hv::cfg::Param<int> = intParam;
		/*cci::cci_param_typed_handle<int> int_param_typed_handle =
												 77	          cci::cci_param_typed_handle<int>(int_param_handle);*/

		if (intParam) {
			// Display current value
			int currentValue = intParam->getValue();
			HV_LOG_INFO("Current value of ConfigModule.intParam is 0x{:x}", currentValue);

			// Update the param's value
			HV_LOG_INFO("Updating ConfigModule.intParam to 0x{:x}", 0xDEAD);
			intParam->setValue(0xDEAD);

			// Display new value
			int newValue = intParam->getValue();
			HV_LOG_INFO("New value of ConfigModule.intParam is 0x{:x}\n", newValue);
		} else {
			HV_LOG_ERROR("Unable to find ConfigModule.intParam parameter\n");
		}

		HV_LOG_INFO("CCI Callback usage example");
		cci::cci_param_handle boolParamHandle = cciBroker.get_param_handle("ConfigModule.boolParam");
		if (boolParamHandle.is_valid()) {
			// Display current JSON value
			std::string currentValue = boolParamHandle.get_cci_value().to_json();
			HV_LOG_INFO("Current value of ConfigModule.boolParam is {}", currentValue);

			// Update the param's value
			HV_LOG_INFO("Updating ConfigModule.boolParam to {}", true);
			boolParamHandle.set_cci_value(cci::cci_value(true));

			// Display new value
			std::string newValue = boolParamHandle.get_cci_value().to_json();
			HV_LOG_INFO("New value of ConfigModule.boolParam is {}", newValue);
		} else {
			HV_LOG_ERROR("Unable to find ConfigModule.boolParam parameter");
		}
	}

private:
	hv::cfg::Param<std::string> stringParam;

	hv::cfg::Broker* hvBroker;
	cci::cci_broker_handle cciBroker;

};

int sc_main(int argc, char* argv[])
{
	spdlog::set_level(spdlog::level::trace);

	HV_LOG_DEBUG("Example 4");

	hv::cfg::Broker hiventiveBroker("Hiventive broker");

	// Call to Broker API
	hiventiveBroker.getCCIBroker().name(); // through CCI API
	hiventiveBroker.getName(); // through Hiventive Broker API

	ConfigModule c("ConfigModule");
	SimulationModule s("SimulationModule");

	sc_core::sc_start();

	return EXIT_SUCCESS;
}

