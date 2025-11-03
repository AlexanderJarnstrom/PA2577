#include "../inc/log_manager.h"
#include <chrono>
#include <ctime>

std::vector<Log>
LogManager::get ()
{
  return m_logs;
}

void
LogManager::add (std::string log)
{
  auto time = std::chrono::system_clock::now ();

  m_logs.push_back (Log{ time, log });
}

std::string
LogManager::to_string (Log &log)
{
  std::time_t t = std::chrono::system_clock::to_time_t (log.stamp);
  return std::ctime (&t);
}
