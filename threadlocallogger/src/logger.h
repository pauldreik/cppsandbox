/*
 * By Paul Dreik
 * http://www.pauldreik.se/
 * License: Boost 1.0
 */
#ifndef LOGGER_H_
#define LOGGER_H_

#include <sstream>

/*
 * this is a basic sketch for a logger that is meant to
 * * know the source file name and line number
 * * debug level configurable at compile time, or
 * * debug level set during program initialization but not later.
 * * select debug level depending on combination of thread and source file, to
 *   be able to for instance follow one specific thread very closely, ignoring
 *   the others.
 * * have as small overhead as possible, preferably no locking or mutexes needed
 *   after initial setup, apart from not keeping stdout garbled etc.
 * it does so by using a static thread local variable, which is (by language
 * rules) initialized before the start of the thread. the user has to promise to
 * set up the logging before starting any threads.
 *
 */

namespace logger
{
  enum class DebugLevel
  {
    TRACE, DEBUG, INFO, WARN, ERROR, FATAL, DISABLE_LOGGING
  };

  void
  log_impl (DebugLevel level, const char* file, int line,
	    const std::string& what);

  /**
   * this function is meant to be called after the logging has been fully
   * configured in the main thread, before other threads have started.
   * it does not need to be fast, because it is only called during
   * initialization. it will return it answer depending on the file name and the
   * thread id (it must be called from the thread it asks for)
   *
   * @param file file name
   * @return the level for the current thread and source file. debug messages of
   * less severity will be suppressed. to suppress messages altogether, return
   * DISABLE_LOGGING
   */
  DebugLevel
  determineDebugLevel (const char* file);
}
#define LOG_TRACE(...) LOG_IMPL(logger::DebugLevel::TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) LOG_IMPL(logger::DebugLevel::DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)  LOG_IMPL(logger::DebugLevel::INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  LOG_IMPL(logger::DebugLevel::WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) LOG_IMPL(logger::DebugLevel::ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...) LOG_IMPL(logger::DebugLevel::FATAL, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_IMPL(level,file,line,...) \
	if(level>=thread_specific_debuglevel) { \
		std::ostringstream oss; oss<<__VA_ARGS__; \
		log_impl(level,file,line,oss.str()); \
	    }

/*
 * note - this is thread safe because it is thread local.
 */
#define LOG_INIT(ignored) LOG_INIT_IMPL(__FILE__)
#define LOG_INIT_IMPL(file) \
		namespace { \
		  const thread_local logger::DebugLevel thread_specific_debuglevel = logger::determineDebugLevel ( \
		      file); \
		}
#endif /* LOGGER_H_ */
