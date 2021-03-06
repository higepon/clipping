TARGET  = ./test
SOURCES = test.cpp
CXXFLAGS = -g \
 -Wall \
 -Wextra \
 -Wno-unused-parameter \
 -Wno-sign-compare \
 -Wno-missing-field-initializers \
 -Wformat=2 \
 -Wstrict-aliasing=2 \
 -Wdisabled-optimization \
 -Wfloat-equal \
 -Wpointer-arith \
 -Wcast-align \
 -Wredundant-decls \
 -Winline

OBJECTS = $(SOURCES:.cpp=.o)

all : $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -lgcov -lgtest -lpthread -o $(TARGET)

check :all
	$(TARGET)

.SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $<

depend:
	$(CXX) -MM $(INCLUDE) $(CXXFLAGS) $(SOURCES) > dependencies

check-syntax:
	LANG=C $(CXX) -o nul -fsyntax-only $(CXXFLAGS) $(INCLUDE) -S ${CHK_SOURCES} && python $(MONADIR)/tool/cpplint.py ${CHK_SOURCES}

clean :
	rm -f $(OBJECTS) $(TARGET) *.gcov *.gcda *.gcno dependencies

-include dependencies
