#include <cstdlib>
#include <iostream>

#include "../inc/database.h"
#include "../inc/frontend.h"
#include "../inc/log_manager.h"

int
main (int argc, char *argv[])
{
  std::string db_host = std::getenv ("DBHOST");
  std::string out_file = std::getenv ("OUTPATH");

  if (db_host.empty ())
  {
    std::cerr << "DBHOST is not defined.\n";
    return -1;
  }

  if (out_file.empty ())
  {
    std::cerr << "DBHOST is not defined.\n";
    return -1;
  }

  LogManager lm;
  Database db (db_host, lm);
  Frontend::run (db, lm, out_file);

  return 0;
}
