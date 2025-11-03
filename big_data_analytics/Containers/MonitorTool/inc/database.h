/**
 * @file
 * @brief Database class.
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <mongocxx/client-fwd.hpp>
#include <mongocxx/collection.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/instance-fwd.hpp>
#include <mongocxx/instance.hpp>
#include <string>

#include "log_manager.h"

/**
 * @class Database
 * @brief Handles database requests.
 *
 */
class Database
{
public:
  static std::string DATABASE;
  static std::string COL_STATUS_UPDATES;
  static std::string COL_FILES;
  static std::string COL_CHUNKS;
  static std::string COL_CLONES;
  static std::string COL_CANDIDATES;

private:
  std::string m_uri;
  mongocxx::instance m_instance;
  unsigned m_gotten_logs;
  LogManager *m_log_manager;

  /**
   * @brief Connect to the database.
   */
  mongocxx::client get_connection ();

public:
  Database (std::string &host, LogManager &lm);

  /**
   * @brief Pings the database.
   */
  void ping ();
  /**
   * @brief debug prints log count.
   */
  void print ();
  /**
   * @brief Checks if new logs has been added, and if so adds them to the log
   * manager.
   */
  void get_logs ();
  /**
   * @brief Gets the amount of documents in a given collection.
   *
   * @param collection The collection in question.
   */
  unsigned get_count (const std::string &collection);
  /**
   * @brief Get the average chunk count per file.
   */
  unsigned chunks_per_file ();
  /**
   * @brief Get the average clone size.
   */
  unsigned clone_size ();
};

#endif // !DATABASE_H
