/*
 * By Paul Dreik
 * http://www.pauldreik.se/
 * License: Boost 1.0
 */
#ifndef LOGGER_CPP_
#define LOGGER_CPP_

#include <iostream>
#include <mutex>
#include <thread>

#include "logger.h"
#include "ThreadName.h"

namespace
{
  std::mutex logger_stdout_mutex;
}

void
logger::log_impl (DebugLevel level, const char* file, int line,
		  const std::string& what)
{
  std::unique_lock<std::mutex> lock (logger_stdout_mutex);
  const auto id = std::this_thread::get_id ();

  std::cout << "threadid=" << id << " level=" << int (level) << " file=" << file
      << " line=" << line << ": " << what << '\n';
}

/*
 * note - this is just a crude sketch showing how the debug level can be set
 * depending on the thread. it could just as well use the file, or query some
 * config file. it could be generalized to let the user set a callback somewhere
 */
logger::DebugLevel
logger::determineDebugLevel (const char* file)
{

  //get the thread name
  const std::string name = ThreadName::getThreadName ();

  logger::DebugLevel result = DebugLevel::DISABLE_LOGGING;

  if (name == "thread 1")
    {
      result = DebugLevel::TRACE;
    }
  else if (name == "thread 2")
    {
      result = DebugLevel::WARN;
    }
  else if (name == "thread 3")
    {
      result = DebugLevel::DISABLE_LOGGING;
    }

  return result;
}

#endif /* LOGGER_CPP_ */
