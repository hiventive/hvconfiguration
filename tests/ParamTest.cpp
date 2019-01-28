#include <iostream>
#include <gtest/gtest.h>
#include <systemc>
#include <HVConfiguration>

class SimpleModule : public sc_core::sc_module {
	SC_HAS_PROCESS(SimpleModule);
public:
	SimpleModule(sc_core::sc_module_name name) :
			sc_core::sc_module(name),
			t("t", 4) {
	}

	int getParamValue() {
		return t;
	}

private:
	hv::cfg::Param<int> t;
};

class ParamTest: public ::testing::Test {
protected:
    virtual void SetUp() {
    	m = new SimpleModule("SimpleModule");
    }

    virtual void TearDown() {
    }
protected:
	SimpleModule* m;
};

TEST_F(ParamTest, DefaultValue) {
    const int x = 4;
    EXPECT_EQ(m->getParamValue(), x);
}

int sc_main(int argc, char* argv[])
{
	hv::cfg::Broker hiventiveBroker("Hiventive broker");
	cci::cci_register_broker(hiventiveBroker.getCCIBroker());

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
