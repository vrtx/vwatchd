///////////////////////////////////////////////////////
//
// vwatchd Unit Test
//
// Exit with exit(EXIT_SUCCESS) or exit(EXIT_FAILURE) to 
// indicate failure.
//
///////////////////////////////////////////////////////

#include "test_setup.hpp"
#include "subprocess.hpp"

// setup the test
void setup()
{
    cout << "Running subprocess with STDIO piped to parent." << endl;
}

// run the test
void runtest()
{
    string stdout; 
    string stderr;
    subprocess *test_proc = new subprocess;
    cout << "Subproc At: " << test_proc << endl;
    subprocess *test_proc1 = subprocess::spawn("/bin/cat", "test.txt", 
                                                stdout, stderr, 0, 0);
    cout << "Subproc At: " << test_proc1 << endl;
}

void teardown()
{
    
}


VWATCHD_RUN_TEST