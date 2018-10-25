#include <iostream>
#include <systemc>
#include <cci_configuration>
#include <hv/configuration.h>

int sc_main(int argc, char* argv[])
{
	::hv::cfg::Broker hiventiveBroker("Hiventive broker");
	::cci::cci_register_broker(hiventiveBroker.getCCIBroker());

	hv::cfg::Param<int> myParam("myParam",
	                            10,
	                            "Parameter description",
	                            ::cci::CCI_RELATIVE_NAME,
	                            ::cci::cci_originator("sc_main"));

	myParam = 3;
	std::cout << myParam.getValue() << std::endl;
	return EXIT_SUCCESS;
}
