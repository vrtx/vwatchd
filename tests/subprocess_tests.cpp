// vwatchd unit test

#include "subprocess.hpp"
#include "signals.hpp"
#include "trace_parser.hpp"
#include <boost/test/unit_test.hpp>
using namespace vwatch;

// test status message (trivial first test)
BOOST_AUTO_TEST_CASE(subprocess_get_status)
{
    subprocess a;
    BOOST_REQUIRE_MESSAGE(a.get_status().length() > 0, "subprocess::get_status():  Returned an empty string.");
}


// spawn cat, send stdin, read stdout, validate buffers
BOOST_AUTO_TEST_CASE(subprocess_get_complete_output)
{
    string stdout, stderr;
    int retcode = 0;
    trace_parser echo_test_parser;

    // register signal handlers
    signals::register_handlers();

    // subprocess *echo = subprocess::spawn("/bin/ps", echo_test_parser, "aux");
    subprocess *echo = subprocess::spawn("ps", echo_test_parser, "");
    echo->get_complete_output(stdout, stderr, retcode);
    cout << "output: " << stdout << " / " << stderr << endl;
    BOOST_REQUIRE_MESSAGE(stdout == "small_string", "subprocess::get_complete_output():  Failed to get valid output from subprocess.");
}