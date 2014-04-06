#ifndef CSV_DATASOURCE_HPP_INCLUDED
#define CSV_DATASOURCE_HPP_INCLUDED

class csv_datasource {
public:
  virtual int getc(bool no_inc_pos) = 0;
  virtual bool open() = 0;
  virtual void close() = 0;
  virtual csv_datasource* copy() = 0;
};

#endif
