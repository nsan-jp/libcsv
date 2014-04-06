#ifndef CSV_MEMORY_DATASOURCE_HPP_INCLUDED
#define CSV_MEMORY_DATASOURCE_HPP_INCLUDED

#include <csv/csv_datasource.hpp>
#include <fstream>
#include <stdlib.h>

class csv_memory_datasource : public csv_datasource {
public:
  csv_memory_datasource(const char* data);
  ~csv_memory_datasource();
  int getc(bool no_inc_pos);
  bool open();
  void close();
  csv_datasource* copy();
private:
  const char* data;
  size_t data_pos;
};

#endif
