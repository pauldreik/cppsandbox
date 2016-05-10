/*
 * By Paul Dreik
 * http://www.pauldreik.se/
 * License: Boost 1.0
 */
#include "logger.h"
#include "worker.h"

LOG_INIT();


int
worker (int workerid)
{

  LOG_DEBUG("hello from worker "<<workerid);

  LOG_TRACE("test msg level TRACE from worker "<<workerid);
  LOG_DEBUG("test msg level DEBUG from worker "<<workerid);
  LOG_INFO("test msg level INFO from worker "<<workerid);
  LOG_WARN("test msg level WARN from worker "<<workerid);
  LOG_ERROR("test msg level ERROR from worker "<<workerid);
  LOG_FATAL("test msg level FATAL from worker "<<workerid);

  LOG_INFO("goodbye from worker "<<workerid);

  return 0;
}
