#include "../inc/database.h"

#include <bsoncxx/json.hpp>
#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/client.hpp>
#include <mongocxx/uri.hpp>
#include <string>

#include "bsoncxx/builder/basic/document.hpp"
#include "bsoncxx/builder/basic/kvp.hpp"
#include "bsoncxx/document/value-fwd.hpp"
#include "bsoncxx/stdx/string_view.hpp"
#include "bsoncxx/types-fwd.hpp"
#include "mongocxx/client-fwd.hpp"
#include "mongocxx/collection-fwd.hpp"
#include "mongocxx/database-fwd.hpp"
#include "mongocxx/exception/exception-fwd.hpp"

std::string Database::DATABASE = "cloneDetector";
std::string Database::COL_STATUS_UPDATES = "statusUpdates";
std::string Database::COL_FILES = "files";
std::string Database::COL_CHUNKS = "chunks";
std::string Database::COL_CLONES = "clones";
std::string Database::COL_CANDIDATES = "candidates";

mongocxx::client
Database::get_connection ()
{
  mongocxx::uri uri (m_uri);
  mongocxx::options::client client_options;
  mongocxx::options::server_api server_api_options (
      mongocxx::options::server_api::version::k_version_1);
  client_options.server_api_opts (server_api_options);

  return mongocxx::client (uri, client_options);
}

Database::Database (std::string &host, LogManager &lm)
    : m_uri ("mongodb://" + host + ":27017/"), m_instance (),
      m_gotten_logs (0), m_log_manager (&lm)
{
}

void
Database::ping ()
{
  using bsoncxx::builder::basic::kvp;
  using bsoncxx::builder::basic::make_document;

  mongocxx::client client = get_connection ();
  try
  {
    // Ping the server to verify that the connection works
    auto admin = client["admin"];
    auto command = make_document (kvp ("ping", 1));
    auto result = admin.run_command (command.view ());
    std::cout << bsoncxx::to_json (result) << "\n";
  }
  catch (const mongocxx::exception &e)
  {
    m_log_manager->add (e.what ());
  }
}

void
Database::print ()
{
  try
  {
    mongocxx::client client = get_connection ();
    mongocxx::collection logs = client[DATABASE][COL_FILES];
    auto result = logs.count_documents ({});
    std::cout << result << std::endl;
  }
  catch (const mongocxx::exception &e)
  {
    m_log_manager->add (e.what ());
  }
}

void
Database::get_logs ()
{
  using bsoncxx::builder::basic::kvp;
  using bsoncxx::builder::basic::make_document;

  try
  {
    mongocxx::client client = get_connection ();
    mongocxx::collection col = client[DATABASE][COL_STATUS_UPDATES];

    if (col.count_documents ({}) <= m_gotten_logs)
      return;

    mongocxx::options::find opts{};
    opts.projection (make_document (kvp ("_id", 0), kvp ("content", 1)));
    mongocxx::cursor cursor = col.find ({}, opts);

    int i = 0;

    for (auto &&res : cursor)
    {
      if (i >= m_gotten_logs)
      {
        auto msg_view = res["content"].get_string ().value;
        std::string message (msg_view.begin ());
        m_log_manager->add (message);
      }
      i++;
    }

    m_gotten_logs = i;
  }
  catch (const mongocxx::exception &e)
  {
    m_log_manager->add (e.what ());
  }
}

unsigned
Database::get_count (const std::string &collection)
{

  using bsoncxx::builder::basic::kvp;
  using bsoncxx::builder::basic::make_document;

  unsigned size = 0;
  try
  {

    mongocxx::client client = get_connection ();
    mongocxx::collection col = client[DATABASE][collection];

    size = col.estimated_document_count ({});
  }
  catch (const mongocxx::exception &e)
  {
    m_log_manager->add (e.what ());
  }

  return size;
}

unsigned
Database::chunks_per_file ()
{
  unsigned num_chunks, num_files;
  mongocxx::client client;
  mongocxx::collection col;

  num_chunks = num_files = 0;

  try
  {
    client = get_connection ();

    col = client[DATABASE][COL_FILES];
    num_files = col.count_documents ({});

    col = client[DATABASE][COL_CHUNKS];
    num_chunks = col.count_documents ({});
  }
  catch (const mongocxx::exception &e)
  {
    m_log_manager->add (e.what ());
  }

  if (num_files == 0)
    return 0;
  else
    return num_chunks / num_files;
}

unsigned
Database::clone_size ()
{

  using bsoncxx::builder::basic::kvp;
  using bsoncxx::builder::basic::make_document;

  unsigned num_in, num_clones;
  mongocxx::client client;
  mongocxx::collection col;
  mongocxx::options::find opts{};

  num_in = num_clones = 0;

  try
  {
    client = get_connection ();

    col = client[DATABASE][COL_CLONES];
    opts.projection (make_document (kvp ("_id", 0), kvp ("instances", 1)));
    mongocxx::cursor cursor = col.find ({}, opts);

    for (auto &&doc : cursor)
    {
      auto in = doc["instances"].get_array ().value;

      for (auto &&i : in)
      {
        auto start = i["startLine"].get_int64 ().value;
        auto end = i["endLine"].get_int64 ().value;
        num_in += end - start;
        num_clones++;
      }
    }
  }
  catch (const mongocxx::exception &e)
  {
    m_log_manager->add (e.what ());
  }

  if (num_clones == 0)
    return 0;
  else
    return num_in / num_clones;
}
