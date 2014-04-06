#include <csv/csv_memory_datasource.hpp>

csv_memory_datasource::csv_memory_datasource(const char* data) {
  this->data = data;
  this->data_pos = 0;
}
csv_memory_datasource::~csv_memory_datasource() {
}

int csv_memory_datasource::getc(bool no_inc_pos) {
  if (data[data_pos] == 0) {
    return EOF;
  }
  if (no_inc_pos) {
    return data[data_pos];
  }
  return data[data_pos++];
}

bool csv_memory_datasource::open() {
  return true;
}

void csv_memory_datasource::close() {
}

csv_datasource* csv_memory_datasource::copy() {
  return new csv_memory_datasource(this->data);
}
