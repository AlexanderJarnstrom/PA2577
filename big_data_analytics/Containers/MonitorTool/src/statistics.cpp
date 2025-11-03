#include "../inc/statistics.h"

#include <chrono>
#include <mongocxx/client.hpp>
#include <string>
#include <vector>

unsigned
Statistic::get_total ()
{
  return m_total_val;
}

unsigned
Statistic::get_duration ()
{
  return m_duration;
}

unsigned
Statistic::get_time_per ()
{
  return m_time_per_chunk;
}

unsigned
Statistic::get_last_change ()
{
  if (m_changes.size () == 0)
    return 0;

  return m_changes[m_changes.size () - 1];
}

unsigned
Statistic::get_chunks_per ()
{
  return m_chunks_per;
}

unsigned
Statistic::get_clone_size ()
{
  return m_clone_size;
}

void
Statistic::update (Database &db)
{
  using namespace std::chrono;

  int change;
  auto start = system_clock::now ();

  m_last_val = m_total_val;
  m_total_val = m_database->get_count (m_collection);

  if (m_collection == Database::COL_CANDIDATES)
  {
    if (m_changes.size () == 0)
      change = m_total_val;
    else
    {
      change = m_changes[0] - (m_changes[0] - m_total_val);
    }
  }
  else
    change = m_total_val - m_last_val;

  if (change == 0 && m_changes.size () == 0)
  {
    m_last = start;
    return;
  }

  if (m_collection == Database::COL_CLONES && !m_changes.empty ())
  {
    m_clone_size = db.clone_size ();
  }

  if (change != 0)
  {
    auto difference = duration_cast<microseconds> (start - m_last);
    m_last = start;
    m_time_per_chunk = difference.count () / change;
    m_unchanged_count = 0;
  }
  else if (m_changes.size () != 0 && m_collection == Database::COL_CHUNKS)
  {
    m_unchanged_count++;
  }
  else if (m_unchanged_count == 20 && m_chunks_per == 0)
  {
    m_chunks_per = db.chunks_per_file ();
  }

  if (change > m_greatest && m_changes.size () > 1)
    m_greatest = change;

  m_changes.push_back (change);
  auto end = system_clock::now ();
  auto milliseconds = duration_cast<std::chrono::milliseconds> (end - start);
  m_duration = milliseconds.count ();
}

std::vector<int>
Statistic::operator() (int width, int height) const
{
  std::vector<int> out (width);
  int i, temp, v_i, size, segment;
  float cent;

  size = m_changes.size ();

  segment = size / width;

  if (segment < 2)
  {
    for (i = width - 1; i >= 0; i--)
    {
      v_i = size - (width - i) - 1;
      if (v_i < 0)
      {
        out[i] = 0;
      }
      else
      {
        temp = m_changes[v_i];
        cent
            = static_cast<float> (temp) / static_cast<float> (m_greatest + 10);
        temp = static_cast<int> (height * cent);
        out[i] = temp;
      }
    }
  }
  else
  {
    v_i = size - 1;
    for (i = width - 1; i >= 0; i--)
    {
      if (v_i - segment < 0)
      {
        out[i] = 0;
      }
      else
      {
        temp = 0;
        for (int j = v_i; j >= v_i - segment; j--)
        {
          temp += m_changes[j];
        }
        v_i -= segment;
        temp = static_cast<int> (temp / segment);
        cent
            = static_cast<float> (temp) / static_cast<float> (m_greatest + 10);
        temp = static_cast<int> (height * cent);
        out[i] = temp;
      }
    }
  }

  return out;
}
