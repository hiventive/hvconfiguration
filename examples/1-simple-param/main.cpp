#include <systemc>
#include <HVConfiguration>
#include <iostream>

int sc_main(int argc, char* argv[])
{
	hv::cfg::Broker hiventiveBroker("Hiventive broker");

	hiventiveBroker.getCCIBroker().set_preset_cci_value("myParam",
			::cci::cci_value(12), ::cci::cci_originator("sc_main"));

	hv::cfg::Param<int> myParam("myParam",
			10,
			"Parameter description",
			cci::CCI_RELATIVE_NAME,
			cci::cci_originator("sc_main"));

	std::cout << "After initialization:" << std::endl;
	std::cout << myParam.getValue() << std::endl;

	std::cout << "Set parameter value to 3:" << std::endl;
	myParam = 3;
	std::cout << myParam.getValue() << std::endl;

	return EXIT_SUCCESS;
}
