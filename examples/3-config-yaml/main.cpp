#include <hv/common/log-level.h>
#define HV_LOG_ACTIVE_LEVEL HV_LOG_LEVEL_DEBUG

#include <systemc>
#include <hv/configuration.h>
#include <cci_configuration>

struct CustomStruct {
	CustomStruct(): message(""), size(0) {};
	std::string message;
	int size;

	friend bool operator== (const CustomStruct&, const CustomStruct&);
};

inline bool operator== (const CustomStruct& lhs, const CustomStruct& rhs)
{
	return lhs.message == rhs.message &&
		   lhs.size == rhs.size;
}

class ConfigModule : public sc_core::sc_module {
	SC_HAS_PROCESS(ConfigModule);
public:
	ConfigModule(sc_core::sc_module_name name) :
		sc_core::sc_module(name),
		intParam("intParam", 5),
		customParam("customParam", CustomStruct()) {
		SC_THREAD(example);
	}
private:
	void example() {
		intParam = 0xBEEF;
		HV_LOG_INFO("ConfigModule.intParam = 0x{:x}", intParam);
		HV_LOG_INFO("ConfigModule.mapParam.message = {}", customParam.getValue().message);
		HV_LOG_INFO("ConfigModule.mapParam.size = {}", customParam.getValue().size);
	}

private:
	hv::cfg::Param<int> intParam;
	hv::cfg::Param<CustomStruct> customParam;
};

int sc_main(int argc, char* argv[])
{
	hv::common::setLogLevel(hv::common::log::trace);



	// Load configuration file
	hv::cfg::YAML yamlConfig("config.yaml");

	hv::cfg::Broker hiventiveBroker("Hiventive broker", &yamlConfig);

	// Call to Broker API
	hiventiveBroker.getCCIBroker().name(); // through CCI API
	hiventiveBroker.getName(); // through Hiventive Broker API

	HV_LOG_INFO("Preset value of A.B.C.D = {}", hiventiveBroker.getCCIBroker().get_preset_cci_value("A.B.C.D").get_int64());

	ConfigModule t("ConfigModule");

	sc_core::sc_start();

	return EXIT_SUCCESS;
}

namespace cci {
template<> struct cci_value_converter<CustomStruct> {
	typedef CustomStruct type;

	static bool pack(cci_value::reference dst, type const& src) {
		cci_value_map_ref mref(dst.set_map());
		mref.push_entry("message", src.message);
		mref.push_entry("size", src.size);
		return true;
	}

	static bool unpack(type & dst, cci_value::const_reference src) {
		if (!src.is_map()) {
			return false;
		}
		cci_value::const_map_reference m = src.get_map();
		if (m.has_entry("message") && m.at("message").try_get(dst.message)
		&& m.has_entry("size") && m.at("size").try_get(dst.size) ) {
			return true;
		}
		return false;
	}
};
}

