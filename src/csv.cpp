#include <csv/csv.hpp>

csv::csv() {
  init_fields();
  this->ds = NULL;
  eod = true;
  _lines = 0;
}

csv::csv(csv_datasource* ds) {
  init_fields();
  this->ds = ds;
}

csv::~csv() {
  free(data_buffer);
  if (ds) {
    ds->close();
    delete ds;
  }
}

void csv::to_csv(string& ret) {
  to_csv(put_field_buffer, ret, true);
  put_field_buffer.clear();
}
void csv::to_csv(string& ret, bool crlf) {
  to_csv(put_field_buffer, ret, crlf, false);
  put_field_buffer.clear();
}
void csv::to_csv(string& ret, bool crlf, bool empty_field_to_null) {
  to_csv(put_field_buffer, ret, crlf, empty_field_to_null);
  put_field_buffer.clear();
}
void csv::to_csv(vector<string>& row, string& ret) {
  to_csv(row, ret, true);
}
void csv::to_csv(vector<string>& row, string& ret, bool crlf) {
  to_csv(row, ret, crlf, false);
}
void csv::to_csv(vector<string>& row, string& ret, bool crlf, bool empty_field_to_null) {
  unsigned i,j;
  init_data_buffer();
  for(i = 0; i < row.size(); i++) {
    if (i > 0) {
      add_char_to_data_buffer(',');
    }
    if (empty_field_to_null && row[i].size() == 0) {
      continue;
    }
    add_char_to_data_buffer('\"');
    for(j = 0; j < row[i].size(); j++) {
      char v = row[i].at(j);
      if (v == '\"') {
        add_char_to_data_buffer(v);
      }
      add_char_to_data_buffer(v);
    }
    add_char_to_data_buffer('\"');
  }
  if (crlf) {
    add_char_to_data_buffer('\r');
  }
  add_char_to_data_buffer('\n');
  ret.append(data_buffer);
}

bool csv::has_more_row() {
  if (eod) {
    return false;
  } else {
    eod = (ds->getc(true) == EOF);
  }
  return !eod;
}

bool csv::next_row(vector<string>& row) {
  if (!ds->open()) {
    return false;
  }
  if (!has_more_row()) {
    return true;
  }
  init_data_buffer();
  int state = CONTROL;
  int column_mode = NO_QUOTED_COLUMN;
  for(;;) {
    int c = ds->getc(false);
    if (c == '\"') {
      if ((column_mode == NO_QUOTED_COLUMN && state != CONTROL) || state == QUOTED) {
        add_char_to_data_buffer(c);
        state = COLUMN;
      } else if (state == COLUMN) {
        state = QUOTED;
      } else if (state == CONTROL) {
        state = COLUMN;
        column_mode = QUOTED_COLUMN;
      }
    } else if (c == ',') {
      if (column_mode == NO_QUOTED_COLUMN || state == QUOTED) {
        flush_field(row);
        state = CONTROL;
      } else {
        add_char_to_data_buffer(c);
      }
    } else if (c == '\n' || c == EOF) {
      if (column_mode == NO_QUOTED_COLUMN || state == QUOTED) {
        if (c == '\n' && data_buffer[data_buffer_pos] == '\r') {
          data_buffer[data_buffer_pos] = 0;
        }
        flush_field(row);
        return true;
      } else if (state == COLUMN && c != EOF) {
        add_char_to_data_buffer(c);
      } else if (c == EOF) {
        eod = true;
        return true;
      }
    } else {
      if (state == CONTROL) {
        column_mode = NO_QUOTED_COLUMN;
        state = COLUMN;
      }
      add_char_to_data_buffer(c);
    }
  }
}

void csv::flush_field(vector<string>& row) {
  row.push_back(string(data_buffer));
  init_data_buffer();
}

void csv::add_char_to_data_buffer(int v) {
  data_buffer[data_buffer_pos] = v;
  data_buffer[data_buffer_pos +1] = 0;
  data_buffer_pos += 1;
  if ((data_buffer_pos + 1) >= data_buffer_size) {
    data_buffer_size *= 2;
    data_buffer = (char*)realloc(data_buffer, data_buffer_size);
  }
}

size_t csv::lines() {
  if (_lines != -1) {
    return _lines;
  }
  csv(this->ds->copy()).read_all(size_counter(&_lines));
  return _lines;
} 

void csv::init_data_buffer() {
  data_buffer_pos = 0;
  data_buffer[0] = 0;
}

void csv::init_fields() {
  _lines = -1;
  eod = false;
  data_buffer = (char*)malloc(BUF_SIZE);
  data_buffer_size = BUF_SIZE;
  init_data_buffer();
}
