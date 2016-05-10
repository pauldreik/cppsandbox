/*
 * By Paul Dreik
 * http://www.pauldreik.se/
 * License: Boost 1.0
 */
#include "ThreadName.h"

#include <stdexcept>

#if __linux__
#include <pthread.h>
#else
#error please implement me for this platform
#endif
void
ThreadName::setThreadName (const std::string& name)
{
#if __linux__
  if(0!=pthread_setname_np (pthread_self (), name.c_str())) {
      throw std::runtime_error("could not set thread name");
  }
#endif
}

std::string
ThreadName::getThreadName ()
{

#if __linux__
  char name[16];
  if(0!=pthread_getname_np (pthread_self (), name, sizeof(name))) {
      throw std::runtime_error("could not get thread name");
  }
  return std::string(name);
#endif

}
