#include <hv/common/log-level.h>
#define HV_LOG_ACTIVE_LEVEL HV_LOG_LEVEL_TRACE

#include <systemc>
#include <hv/configuration.h>
#include <iostream>

int sc_main(int argc, char* argv[])
{
	hv::common::setLogLevel(hv::common::log::trace);

	hv::cfg::Broker hiventiveBroker("Hiventive broker");

	hiventiveBroker.getCCIBroker().set_preset_cci_value("myParam",
			::cci::cci_value(12), ::cci::cci_originator("sc_main"));

	hv::cfg::Param<int> myParam("myParam",
			10,
			"Parameter description",
			cci::CCI_RELATIVE_NAME,
			cci::cci_originator("sc_main"));

	HV_LOG_DEBUG("After initialization: ", myParam.getValue());

	HV_LOG_DEBUG("Set parameter value to 3:");
	myParam = 3;
	HV_LOG_DEBUG("{}", myParam.getValue());

	return EXIT_SUCCESS;
}
