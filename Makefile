CC = g++ -fPIC -fforce-addr -fomit-frame-pointer -O3
SRCS = $(shell ls ./src/*.cpp)
SRCNAMES = $(shell ls ./src/*.cpp | cut -d "/" -f 3)
OBJS = $(addprefix ./build/,$(SRCNAMES:.cpp=.o))
prefix = /usr/local
exec_prefix = ${prefix}
LIBDIR = ${exec_prefix}/lib
INCDIR = ${prefix}/include/csv/

compile: ./build/libcsv.so ./build/libcsv.a

./build/libcsv.so: $(OBJS)
	cd ./build/ && $(CC) *.o -shared -o libcsv.so

./build/libcsv.a: $(OBJS)
	cd ./build/ && ar rv libcsv.a *.o

./build/%.o: ./src/%.cpp
	cd ./build/ && $(CC) -c -I../src/include ../$<

install: ./build/libcsv.so ./build/libcsv.a
	install -d $(LIBDIR)
	install ./build/libcsv.so $(LIBDIR)
	install ./build/libcsv.a $(LIBDIR)
	install -d $(INCDIR)
	install ./src/include/csv/*.hpp $(INCDIR)

uninstall:
	rm -f $(LIBDIR)/libcsv.so
	rm -f $(LIBDIR)/libcsv.a
	rm -fr $(INCDIR)

clean: 
	rm -f ./build/*.o rm -f ./build/libcsv.so ./build/libcsv.a
