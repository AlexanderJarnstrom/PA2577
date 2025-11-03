/**
 * @file
 * @brief Statistic class.
 */

#ifndef STATISTICS_H
#define STATISTICS_H

#include <chrono>
#include <string>
#include <vector>

#include "database.h"
#include "log_manager.h"

/**
 * @class Statistic
 * @brief Holds all related statistics for a collection.
 *
 */
class Statistic
{
  unsigned m_greatest;
  unsigned m_total_val;
  unsigned m_last_val;
  unsigned m_time_per_chunk;
  unsigned m_chunks_per;
  unsigned m_clone_size;
  bool m_done;
  unsigned m_unchanged_count;

  std::chrono::system_clock::time_point m_last;
  unsigned m_duration;

  std::vector<unsigned> m_changes;
  std::string m_collection;
  Database *m_database;
  LogManager *m_log_manager;

public:
  Statistic (const std::string &collection, Database *database, LogManager *lm)
      : m_total_val (0), m_last_val (0), m_greatest (0), m_duration (0),
        m_done (false), m_chunks_per (0), m_clone_size (0),
        m_unchanged_count (0), m_time_per_chunk (0),
        m_last (std::chrono::system_clock::now ()), m_collection (collection),
        m_database (database), m_log_manager (lm)
  {
  }

  unsigned get_total ();
  unsigned get_duration ();
  unsigned get_time_per ();
  unsigned get_last_change ();
  unsigned get_chunks_per ();
  unsigned get_clone_size ();

  /**
   * @brief Runs all the statistics updates.
   *
   * @param db database object.
   */
  void update (Database &db);

  /**
   * @brief Creates and balances the graph data vector.
   *
   * @param width graph width.
   * @param height graph height.
   */
  std::vector<int> operator() (int width, int height) const;
};

#endif // !STATISTICS_H
