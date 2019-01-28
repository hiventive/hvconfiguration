#include <systemc>
#include <HVConfiguration>
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
		std::cout << myParam << std::endl;
	}

private:
	hv::cfg::Param<int> myParam;
};

int sc_main(int argc, char* argv[])
{
	hv::cfg::Broker hiventiveBroker("Hiventive broker");

	// Call to Broker API
	hiventiveBroker.getCCIBroker().name(); // through CCI API
	hiventiveBroker.getName(); // through Hiventive Broker API

	ConfigModule t("ConfigModule");

	sc_core::sc_start();

	return EXIT_SUCCESS;
}


