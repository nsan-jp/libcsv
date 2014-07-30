#include <csv/csv_file_datasource.hpp>

csv_file_datasource::csv_file_datasource(const char* file_name) {
  this->file_name = string(file_name);
  this->buffer_pos = 0;
  this->csv_file = -1;
  clear_buffer();
}
csv_file_datasource::~csv_file_datasource() {
  close();
}

int csv_file_datasource::getc(bool no_inc_pos) {
  if (!open()) {
    return EOF;
  }
  if (buffer[buffer_pos] == 0) {
    clear_buffer();
    size_t size = ::read(csv_file, buffer, (BUF_SIZE - 1));
    if (size == 0) {
      return EOF;
    }
    buffer[size] = 0;
  }
  if (no_inc_pos) {
    return buffer[buffer_pos];
  }
  return buffer[buffer_pos++];
}

bool csv_file_datasource::open() {
  if (csv_file == -1) {
    csv_file = ::open(file_name.c_str(), O_RDONLY);
    if (csv_file == -1) {
      return false;
    }
  }
  return true;
}

void csv_file_datasource::close() {
  if (csv_file != -1) {
    ::close(csv_file);
  }
}

void csv_file_datasource::clear_buffer() {
  buffer[0] = 0;
  buffer_pos = 0;
}

csv_datasource* csv_file_datasource::copy() {
  return new csv_file_datasource(this->file_name.c_str());
}
