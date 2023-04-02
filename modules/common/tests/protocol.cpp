#include <boost/test/unit_test.hpp>
#include "protocol.hpp"
using namespace common;

struct test_case
{
    const command cmd_;
    const transf_package::args_t args_;
    const transf_package::packet_t expectedPack_;
    test_case(command cmd, const transf_package::args_t& args, const transf_package::packet_t& expectedPack)
        : cmd_(cmd), args_(args), expectedPack_(expectedPack) {}
};

struct fixture
{
    test_case case_1{command::LOG_IN, {"USER", "12345"}, "<1><USER><12345>"};
    test_case case_2{command::SEND_MESSAGE, {"~!@#$%^&*()_+.?/\\\'\""}, "<12><~!@#$%^&*()_+.?/\\\'\">"};
    test_case case_3{command::ERROR_LOG_IN, 
                    {"", "", "1", "2", "3", "4", "5", "6", "7", "8", "9"}, "<3><1><2><3><4><5><6><7><8><9>"};
    test_case case_4{command::DETACH_ROOM, {}, "<14>"};
};

BOOST_FIXTURE_TEST_CASE(assemble_pack, fixture)
{
    transf_package::packet_t readyPack;

    readyPack = assemble_package(case_1.cmd_, case_1.args_);
    BOOST_CHECK_EQUAL(readyPack, case_1.expectedPack_);

    readyPack = assemble_package(case_2.cmd_, case_2.args_);
    BOOST_CHECK_EQUAL(readyPack, case_2.expectedPack_);

    readyPack = assemble_package(case_3.cmd_, case_3.args_);
    BOOST_CHECK_EQUAL(readyPack, case_3.expectedPack_);

    readyPack = assemble_package(case_4.cmd_, case_4.args_);
    BOOST_CHECK_EQUAL(readyPack, case_4.expectedPack_);
};

BOOST_FIXTURE_TEST_CASE(disassemble_pack, fixture)
{
    transf_package pack;
    pack.disassemble(case_1.expectedPack_);
    BOOST_CHECK(pack.get_command() == case_1.cmd_);
    for(std::size_t it = 0, end = case_1.args_.size(); it != end; ++it)
        BOOST_CHECK_EQUAL(pack.get_arg(it), case_1.args_.at(it));
    
    pack.disassemble(case_2.expectedPack_);
    BOOST_CHECK(pack.get_command() == case_2.cmd_);
    for(std::size_t it = 0, end = case_2.args_.size(); it != end; ++it)
        BOOST_CHECK_EQUAL(pack.get_arg(it), case_2.args_.at(it));

    pack.disassemble(case_3.expectedPack_);
    BOOST_CHECK(pack.get_command() == case_3.cmd_);
    for(std::size_t it = 0, end = case_3.args_.size() - 2; it != end; ++it)
        BOOST_CHECK_EQUAL(pack.get_arg(it), case_3.args_.at(it + 2));
    
    pack.disassemble(case_4.expectedPack_);
    BOOST_CHECK(pack.get_command() == case_4.cmd_);
    BOOST_CHECK_THROW(pack.get_arg(0), std::out_of_range);
};

BOOST_FIXTURE_TEST_CASE(correct_number_arguments, fixture)
{
    transf_package pack;
    pack.disassemble(case_1.expectedPack_);
    BOOST_CHECK_EQUAL(pack.isMatched(), true);

    pack.disassemble(case_2.expectedPack_);
    BOOST_CHECK_EQUAL(pack.isMatched(), true);
    
    pack.disassemble(case_3.expectedPack_);
    BOOST_CHECK_EQUAL(pack.isMatched(), false);

    pack.disassemble(case_4.expectedPack_);
    BOOST_CHECK_EQUAL(pack.isMatched(), true);
};
