#include "../inc/frontend.h"

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <functional>
#include <string>
#include <vector>

#include "../inc/database.h"
#include "../inc/statistics.h"
#include "../inc/statistics_manager.h"

void
Frontend::run (Database &db, LogManager &lm, std::string &file_path)
{
  using namespace ftxui;

  StatisticsManager manager;

  Statistic file_stats (Database::COL_FILES, &db, &lm);
  Statistic chunk_stats (Database::COL_CHUNKS, &db, &lm);
  Statistic candidates_stats (Database::COL_CANDIDATES, &db, &lm);
  Statistic clones_stats (Database::COL_CLONES, &db, &lm);

  ScreenInteractive screen = ScreenInteractive::Fullscreen ();

  int bottom_size = 5;
  int right_size = 20;

  manager.add (file_stats);
  manager.add (chunk_stats);
  manager.add (candidates_stats);
  manager.add (clones_stats);

  manager.start (screen, db, file_path);

  Component graph_view = graphs_renderer (file_stats, chunk_stats,
                                          candidates_stats, clones_stats);
  Component log_view = log_renderer (lm);
  Component info_view = render_text_info (file_stats, chunk_stats,
                                          candidates_stats, clones_stats);

  auto container = graph_view;
  container = ResizableSplitBottom (log_view, container, &bottom_size);
  container = ResizableSplitRight (info_view, container, &right_size);

  Component renderer
      = Renderer (container, [&] { return container->Render () | border; });

  Component event_component
      = CatchEvent (renderer,
                    [&] (Event event)
                    {
                      if (event == Event::Character ('q'))
                      {
                        screen.ExitLoopClosure () ();
                        return true;
                      }
                      return false;
                    });

  screen.Loop (event_component);
  manager.stop ();
}

ftxui::Component
Frontend::graphs_renderer (Statistic &file_stats, Statistic &chunk_stats,
                           Statistic &candidates_stats,
                           Statistic &clones_stats)
{
  using namespace ftxui;
  return Renderer (
      [&] () -> Element
      {
        return vbox ({ text ("Files") | bold,
                       graph (file_stats) | color (Color::Green), separator (),
                       text ("Chunks") | bold,
                       graph (chunk_stats) | color (Color::Red), separator (),
                       text ("Candidates") | bold,
                       graph (candidates_stats) | color (Color::Yellow),
                       separator (), text ("Clones") | bold,
                       graph (clones_stats) | color (Color::Blue) });
      });
}

ftxui::Component
Frontend::log_renderer (LogManager &lm)
{
  using namespace ftxui;

  return Renderer (
      [&] () -> Element
      {
        std::vector<std::vector<std::string>> content;
        auto logs = lm.get ();
        unsigned size = logs.size ();
        int i;
        i = (size >= 20) ? size - 20 : 0;

        for (; i < size; i++)
        {
          Log log = logs[i];
          content.push_back ({ LogManager::to_string (log), log.msg });
        }

        return Table (content).Render ();
      });
}

ftxui::Component
Frontend::render_text_info (Statistic &file_stats, Statistic &chunk_stats,
                            Statistic &candidates_stats,
                            Statistic &clones_stats)
{
  using namespace ftxui;
  //clang-format off
  return Renderer (
      [&] () -> Element
      {
        return vbox ({
            text ("Files") | bold,
            text ("Count: " + std::to_string (file_stats.get_total ())),
            text ("Change: " + std::to_string (file_stats.get_last_change ())),
            text ("Latency: " + std::to_string (file_stats.get_duration ())
                  + "ms"),
            text ("Time per: " + std::to_string (file_stats.get_time_per ())
                  + "us"),
            separator (),
            text ("Chunks") | bold,
            text ("Count: " + std::to_string (chunk_stats.get_total ())),
            text ("Change: "
                  + std::to_string (chunk_stats.get_last_change ())),

            text ("Chunks per file: "
                  + std::to_string (chunk_stats.get_chunks_per ())),
            text ("Latency: " + std::to_string (chunk_stats.get_duration ())
                  + "ms"),
            text ("Time per: " + std::to_string (chunk_stats.get_time_per ())
                  + "us"),
            separator (),
            text ("Candidates") | bold,
            text ("Count: " + std::to_string (candidates_stats.get_total ())),
            text ("Latency: "
                  + std::to_string (candidates_stats.get_duration ()) + "ms"),
            text ("Time per: "
                  + std::to_string (candidates_stats.get_time_per ()) + "us"),
            separator (),
            text ("Clones") | bold,

            text ("Count: " + std::to_string (clones_stats.get_total ())),
            text ("Avg instance: "
                  + std::to_string (clones_stats.get_clone_size ())),
            text ("Change: "
                  + std::to_string (clones_stats.get_last_change ())),
            text ("Latency: " + std::to_string (clones_stats.get_duration ())
                  + "ms"),
            text ("Time per: " + std::to_string (clones_stats.get_time_per ())
                  + "us"),
            separator (),
        });
      });
  //clang-format on
}
