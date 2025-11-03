#include "../inc/statistics_manager.h"

#include <chrono>
#include <fstream>
#include <ftxui/component/screen_interactive.hpp>
#include <thread>

#include "../inc/statistics.h"

void
StatisticsManager::work (std::vector<Statistic *> *stats,
                         ftxui::ScreenInteractive *screen, Database *db,
                         std::string *file_path)
{
  Statistic *files = (*stats)[0];
  Statistic *chunks = (*stats)[1];
  Statistic *candidates = (*stats)[2];
  Statistic *clones = (*stats)[3];
  unsigned counter = 0;

  std::ofstream f (file_path->c_str ());
  f.close ();

  while (true)
  {
    auto start = std::chrono::system_clock::now ();
    for (Statistic *s : *stats)
    {
      s->update (*db);
    }

    db->get_logs ();

    screen->RequestAnimationFrame ();

    if (counter % 2 == 1)
    {
      std::ofstream f (file_path->c_str (), std::ios_base::app);
      f << counter << ": f:[to:" << files->get_total ()
        << "] cu:[to:" << chunks->get_total ()
        << " c:" << chunks->get_last_change ()
        << " ti:" << chunks->get_time_per ()
        << " cf:" << chunks->get_chunks_per ()
        << "] ca:[to:" << candidates->get_total ()
        << " ti:" << candidates->get_time_per ()
        << "] cl:[to:" << clones->get_total ()
        << " c:" << clones->get_last_change ()
        << " ti:" << clones->get_time_per ()
        << " in:" << clones->get_clone_size () << "]\n";
      f.close ();
    }

    counter++;

    auto end = std::chrono::system_clock::now ();
    auto sleep_time = std::chrono::duration_cast<std::chrono::milliseconds> (
        std::chrono::milliseconds (1000) - (end - start));
    std::this_thread::sleep_for (sleep_time);
  }
}

void
StatisticsManager::add (Statistic &stat)
{
  m_stats.push_back (&stat);
}

void
StatisticsManager::start (ftxui::ScreenInteractive &screen, Database &db,
                          std::string &file_path)
{
  m_worker = std::thread (StatisticsManager::work, &m_stats, &screen, &db,
                          &file_path);
}

void
StatisticsManager::stop ()
{
  m_worker.detach ();
}
