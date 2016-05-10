/*
 * By Paul Dreik
 * http://www.pauldreik.se/
 * License: Boost 1.0
 */
#include <iostream>
#include <thread>

#include "worker.h"

#include "ThreadName.h"

int
main ()
{

  std::cout << "hello from main!\nThree worker threads will be started, each printing\n"
      "debug messages from TRACE all the way up to FATAL.\n"
      "Each thread will have a different debug level";
    {
      std::thread t1 ([]()
	{ ThreadName::setThreadName("thread 1");
	  worker(1);});

      std::thread t2 ([]()
	{ ThreadName::setThreadName("thread 2");
	  worker(2);});

      std::thread t3 ([]()
	{ ThreadName::setThreadName("thread 3");
	  worker(3);});

      t1.join ();
      t2.join ();
      t3.join ();
    }
  std::cout << "goodbye from main\n";

  return 0;
}
