/**
 * @file
 * @brief Statistic manager.
 */

#ifndef STATISTICS_MANAGER_H
#define STATISTICS_MANAGER_H

#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/screen/screen.hpp>
#include <thread>
#include <vector>

#include "database.h"
#include "statistics.h"

/**
 * @class StatisticsManager
 * @brief Manages the different statistics.
 *
 */
class StatisticsManager
{
  std::vector<Statistic *> m_stats;
  std::thread m_worker;

  /**
   * @brief update loop for the thread.
   *
   * @param stats Vector of statistics.
   * @param screen screen for the frontend.
   * @param db database object.
   * @param file_path path for the output.
   */
  static void work (std::vector<Statistic *> *stats,
                    ftxui::ScreenInteractive *screen, Database *db,
                    std::string *file_path);

public:
  StatisticsManager () {}

  /**
   * @brief add Statistic.
   *
   * @param stat statistics object.
   */
  void add (Statistic &stat);
  /**
   * @brief Start the statistics thread.
   *
   * @param screen frontend screen.
   * @param db database object.
   * @param file_path output file path.
   */
  void start (ftxui::ScreenInteractive &screen, Database &db,
              std::string &file_path);
  /**
   * @brief Stop the statistics thread.
   */
  void stop ();
};

#endif // !STATISTICS_MANAGER_H
