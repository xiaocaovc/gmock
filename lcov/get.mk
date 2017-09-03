GTEST_INC_DIR = /usr/local/include
GMOCK_INC_DIR = /usr/local/include
GMOCK_LIB_DIR = /usr/local/lib
CC=g++
SOURCE=fool.cpp
CPPFLAGS = -I"$(GTEST_INC_DIR)" -I"$(GMOCK_INC_DIR)" -I"$(INC)"
CXXFLAGS = -g -Wall -Wextra -pthread 
LINK = -g -lpthread -lrt  -lgcov -L"$(GMOCK_LIB_DIR)" -lgmock 

OBJS=fool.o
TARGET=fool
TESTDIR=fool

all:$(TARGET)
	
$(TARGET) : $(OBJS)
	$(CC) $^ $(LINK)  -o    $@
	@echo
	@echo '*'
	@echo '* Generating HTML output'
	lcov --directory . --zerocounters
	./$(TARGET)
	-mkdir output
	mv $(TESTDIR).gcno $(TESTDIR).gcda output
	-rm *.gcno *.gcda 
	lcov --rc lcov_branch_coverage=1 --capture --directory . --output-file app.info --test-name test_noargs --no-external
	genhtml --rc lcov_branch_coverage=1 app.info --output-directory ./output/outhtml --title "$(TESTDIR) test" --show-details --legend
$(OBJS) : $(SOURCE)
	$(CC) -c -g3 -fprofile-arcs -ftest-coverage $^  -o $@
.PHONY : clean
clean:
	-rm -fr $(TARGET) $(OBJS) output *.info