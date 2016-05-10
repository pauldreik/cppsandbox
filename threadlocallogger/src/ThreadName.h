/*
 * By Paul Dreik
 * http://www.pauldreik.se/
 * License: Boost 1.0
 */
#pragma once

#include <string>

namespace ThreadName {
  /**
   * sets the name of the current thread to name.
   * @param name
   */
  void setThreadName(const std::string& name);

  /**
   * gets the name of the current thread.
   * @return
   */
  std::string getThreadName();
}
