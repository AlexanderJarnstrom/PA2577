/**
 * @file
 * @brief All frontend functions.
 */

#ifndef FRONTEND_H
#define FRONTEND_H

#include <ftxui/component/component_base.hpp>

#include "../inc/database.h"
#include "../inc/log_manager.h"
#include "../inc/statistics.h"

namespace Frontend
{
/**
 * @brief Run the frontend loop.
 *
 * @param db database object.
 * @param lm log manager object.
 * @param file_path path to the output file.
 */
void run (Database &db, LogManager &lm, std::string &file_path);

/**
 * @brief Create the graphs for the renderer.
 *
 * @param file_stats File statistics.
 * @param chunk_stats Chunk statistics
 * @param candidates_stats Candidate statistics
 * @param clones_stats Clone statistics.
 */
ftxui::Component graphs_renderer (Statistic &file_stats,
                                  Statistic &chunk_stats,
                                  Statistic &candidates_stats,
                                  Statistic &clones_stats);

/**
 * @brief Create the log component for the renderer.
 *
 * @param lm log manager object.
 */
ftxui::Component log_renderer (LogManager &lm);

/**
 * @brief Create a list of statistics on the side for the renderer.
 *
 * @param file_stats File statistics.
 * @param chunk_stats Chunk statistics.
 * @param candidates_stats Candidate statistics.
 * @param clones_stats Clone statistics.
 */
ftxui::Component render_text_info (Statistic &file_stats,
                                   Statistic &chunk_stats,
                                   Statistic &candidates_stats,
                                   Statistic &clones_stats);
}; // namespace Frontend

#endif // !FRONTEND_H
