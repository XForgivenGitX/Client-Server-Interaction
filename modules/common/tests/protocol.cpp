#define BOOST_TEST_MAIN
#define BOOST_TEST_MODULE test_protocol

#include <boost/test/unit_test.hpp>
#include <protocol.hpp>

namespace testStrings
{
    
}


struct fixture
{
    common::transf_package pack;
    common::command testCmd;
    const common::transf_package::args_t testArgs_1;
    const common::transf_package::args_t testArgs_2;
    const common::transf_package::args_t testArgs_3;
    fixture() : testArgs_1({"USER", "12345"}), testArgs_2({"", "w45~~3455-=fg", "<><><><", "fffse12415t5", "3416824572", ",...,,,<><><,.,.,"}), testCmd(common::command::LOG_IN_REQ) {}
    ~fixture(){}
};

BOOST_FIXTURE_TEST_CASE(assemble_pack, fixture)
{
    using namespace common;
    transf_package::packet_t readyPack;
    readyPack = assemble_package(testCmd, testArgs_1);
    BOOST_CHECK_EQUAL(readyPack, "<1><USER><12345>");

    readyPack = assemble_package(testCmd, testArgs_2);
    BOOST_CHECK_EQUAL(readyPack, "<1><w45~~3455-=fg><<><><><>");

    readyPack = assemble_package(testCmd, testArgs_3);
    BOOST_CHECK_EQUAL(readyPack, "<1>");
}

BOOST_FIXTURE_TEST_CASE(disassemble_pack, fixture)
{
    using namespace common;
    
    pack.disassemble(assemble_package(testCmd, testArgs_1));
    BOOST_CHECK_THROW(pack.get_argument(2), std::out_of_range);
    BOOST_CHECK_THROW(pack.get_argument(-1), std::out_of_range);
    BOOST_CHECK_EQUAL(static_cast<unsigned>(pack.get_command()), static_cast<unsigned>(testCmd));
    BOOST_CHECK_EQUAL(pack.get_argument(0), testArgs_1.at(0));
    BOOST_CHECK_EQUAL(pack.get_argument(1), testArgs_1.at(1));
}
