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
		myParam("myParam", 5) {
		SC_THREAD(example);
	}
private:
	void example() {
		myParam = 3;
		HV_LOG_DEBUG("{}", myParam);
	}

private:
	hv::cfg::Param<int> myParam;
};

int sc_main(int argc, char* argv[])
{
	hv::common::setLogLevel(hv::common::log::trace);

	hv::cfg::Broker hiventiveBroker("Hiventive broker");

	// Call to Broker API
	hiventiveBroker.getCCIBroker().name(); // through CCI API
	hiventiveBroker.getName(); // through Hiventive Broker API

	ConfigModule t("ConfigModule");

	sc_core::sc_start();

	return EXIT_SUCCESS;
}


