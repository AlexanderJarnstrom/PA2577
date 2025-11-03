/**
 * @file
 * @brief Everything needed for the log system.
 */

#ifndef LOG_MANAGER_H
#define LOG_MANAGER_H

#include <chrono>
#include <string>
#include <vector>

/**
 * @class Log
 * @brief Holds needed log information.
 *
 */
struct Log
{
  std::chrono::time_point<std::chrono::system_clock> stamp;
  std::string msg;
};

/**
 * @class LogManager
 * @brief Manages the logs.
 *
 */
class LogManager
{
  std::vector<Log> m_logs;

public:
  LogManager () {}

  /**
   * @brief Get the logs.
   */
  std::vector<Log> get ();
  /**
   * @brief Add log to list.
   *
   * @param log log message.
   */
  void add (std::string log);

  /**
   * @brief Convert log times point to string.
   *
   * @param log Log object.
   */
  static std::string to_string (Log &log);
};

#endif // !LOG_MANAGER_H
