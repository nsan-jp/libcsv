#ifndef CSV_HPP_INCLUDED
#define CSV_HPP_INCLUDED

#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <fcntl.h>
#include <csv/csv_file_datasource.hpp>
#include <csv/csv_memory_datasource.hpp>

using namespace std;

/**
 * CSV read/write utility
 */
class csv {
public:
  csv();
  csv(csv_datasource* ds);
  ~csv();
  void to_csv(vector<string>& row, string& ret, bool crlf);
  bool has_more_row();
  bool next_row(vector<string>& row);
  size_t lines();
  template<typename T> bool read_all(T proc) {
    vector<string> row;
    unsigned i;
    for (i = 0; has_more_row(); i++) {
      row.clear();
      if (!next_row(row)) {
        return false;
      }
      proc(i, &row);
    }
  }
private:
  static const int BUF_SIZE = 1024 * 32;
  enum read_state {
    CONTROL,
    COLUMN,
    QUOTED
  };
  enum column_type {
    QUOTED_COLUMN,
    NO_QUOTED_COLUMN
  };
  struct size_counter {
    size_t *lines;
    size_counter(size_t *lines) {
      this->lines = lines;
      (*this->lines) = 0;
    }
    void operator()(int i, vector<string>* row) {
      (*lines) = i + 1; 
    } 
  };

  csv_datasource *ds;

  size_t _lines;

  char* data_buffer;
  size_t data_buffer_size;
  size_t data_buffer_pos;

  bool eod;


  void init_fields();
  void add_char_to_data_buffer(int v);
  void flush_field(vector<string>& row);
  void init_data_buffer();
};

#endif
