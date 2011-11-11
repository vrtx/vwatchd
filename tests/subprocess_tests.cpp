// vwatchd unit test

#include "subprocess.hpp"
#include <boost/test/unit_test.hpp>

// test status message (trivial first test)
BOOST_AUTO_TEST_CASE(subprocess_get_status)
{

    vwatch::subprocess a;
    BOOST_REQUIRE_MESSAGE(a.get_status().length() > 0, "subprocess::get_status():  Returned an empty string.");
}