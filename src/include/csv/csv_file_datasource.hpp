#ifndef CSV_FILE_DATASOURCE_HPP_INCLUDED
#define CSV_FILE_DATASOURCE_HPP_INCLUDED

#include <csv/csv_datasource.hpp>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

class csv_file_datasource : public csv_datasource {
public:
  csv_file_datasource(const char* file_name);
  ~csv_file_datasource();
  int getc(bool no_inc_pos);
  bool open();
  void close();
  csv_datasource* copy();
private:
  static const int BUF_SIZE = 1024 * 32;
  char buffer[BUF_SIZE];
  size_t buffer_pos;
  string file_name;
  int csv_file;
  void clear_buffer();
};

#endif
