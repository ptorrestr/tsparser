object:=parser

# CXX or GCC G++
cxx:=g++-4.3
cxxflags:=-g

## LOCAL DIRS
tmpdir:=./tmp
srcdir:=./src
incdir:=./include
test:= ./test

# rose base
rose_home:=/opt/rose-0.9.5a-gcc-4.3
rose_ldd:=-lrose
rose_include:=$(rose_home)/include
rose_lib:=$(rose_home)/lib

# boost base
boost_home:=/opt/boost_1_47_0-gcc-4.3
boost_ldd:=-lboost_filesystem -lboost_date_time -lboost_thread -lboost_program_options -lboost_regex -lboost_system -lboost_wave
boost_include:=$(boost_home)/include
boost_lib:=$(boost_home)/lib

# java base
java_home:=$(JAVA_HOME)
java_ldd:=-ljvm
java_lib:=$(java_home)/jre/lib/amd64/server

## INCLUDE
include:=-I$(incdir) -I$(rose_include) -I$(boost_include)

## LIB
lib:=-L$(rose_lib) -L$(boost_lib) -L$(java_lib)

## extra libraries
extra_libs:=$(rose_ldd) $(boost_ldd) $(java_ldd)

# Generate objects
sources := $(wildcard $(srcdir)/*.cc)
objects := $(patsubst $(srcdir)/%.cc, $(tmpdir)/%.o,$(sources))

####### Main

main: $(object)

$(object): $(objects)
	$(cxx) $(cxxflags) -o $(object) $(objects) $(include) $(lib) $(rose_traslator) $(extra_libs) $(object).cc

$(tmpdir)/%.o: $(srcdir)/%.cc 
	test -d $(tmpdir) || mkdir $(tmpdir)
	$(cxx) $(cxxflags) $(include) -c -o $(tmpdir)/$(*F).o $(srcdir)/$*.cc

####### AST print
# Used by test to generate AST.
astprint_dir := ./astprint
astprint_obj := ast_print

astprint-var: $(astprint_obj)

$(astprint_obj):
	$(cxx) $(cxxflags) -o $(astprint_obj) $(include) $(lib) $(rose_traslator) $(extra_libs) $(astprint_dir)/$(astprint_obj).cc

####### Test
testsrc_dir := $(test)/samples
testresults_dir := $(test)/results
testparsed_dir := $(tmpdir)/parsed
testparsedtmp_dir := $(testparsed_dir)/tmp
testdot_dir := $(tmpdir)/dots
test_sources := $(sort $(wildcard $(testsrc_dir)/*.cc))
test_parsed := $(patsubst $(testsrc_dir)/%.cc, $(testparsed_dir)/%.cc, $(test_sources))
test_tmp_builds := $(patsubst $(testsrc_dir)/%.cc, $(testparsedtmp_dir)/%.o, $(test_sources))
test_sample_dots := $(patsubst $(testsrc_dir)/%.cc, $(testdot_dir)/%_sample.dot, $(test_sources))
test_result_dots := $(patsubst $(testsrc_dir)/%.cc, $(testdot_dir)/%_result.dot, $(test_sources))
test_output_dots := $(patsubst $(testsrc_dir)/%.cc, $(testdot_dir)/%_output, $(test_sources))
test_text_dots := $(patsubst $(testsrc_dir)/%.cc, $(testdot_dir)/%_text, $(test_sources))

#Compare the output AST generated by the parser using samples files as input 
#against the ASTs of results files
test: main astprint-var compare

# Compare text and AST
compare: $(test_text_dots) $(test_output_dots)

# Compare text
# We use *.o files to catch up compilers errors!
$(testdot_dir)/%_text: $(testparsedtmp_dir)/%.o $(testresults_dir)/%.cc
	@diff $(testparsed_dir)/$*.cc $(testresults_dir)/$*.cc
	@RETVAL=$$?; \
	testName=$*; \
	if [ $$RETVAL -eq 0 ]; then \
			echo "====> Text TEST: $$testName OK"; \
	else \
			echo "====> Text TEST: $$testName FAILED"; \
	fi

# Compare dot files (AST)
$(testdot_dir)/%_output: $(testdot_dir)/%_sample.dot $(testdot_dir)/%_result.dot
	@diff $(testdot_dir)/$*_sample.dot $(testdot_dir)/$*_result.dot
	@RETVAL=$$?; \
	testName=$*; \
	if [ $$RETVAL -eq 0 ]; then \
			echo "====> AST TEST: $$testName OK"; \
	else \
			echo "====> AST TEST: $$testName FAILED"; \
	fi

# Generate AST for samples
test-sample-dots: $(test_tmp_builds)

# We use *.o files to catch up compilers errors!
$(testdot_dir)/%_sample.dot: $(testparsedtmp_dir)/%.o
	@test -d $(testdot_dir) || mkdir $(testdot_dir)
	@./$(astprint_obj) $(testparsed_dir)/$*.cc
	@$(simple_dot) -f ./$*.cc.dot > $(testdot_dir)/$*_sample.dot
	@rm ./$*.cc.dot


#SimpleDot script. This script change memory references of AST for generic node names.
simple_dot := $(test)/simpleDot

# Generate AST for results
test-result-dots: $(test_result_dots)

$(testdot_dir)/%_result.dot: $(testresults_dir)/%.cc
	@test -d $(testdot_dir) || mkdir $(testdot_dir)
	@./$(astprint_obj) $(testresults_dir)/$*.cc
	@$(simple_dot) -f ./$*.cc.dot > $(testdot_dir)/$*_result.dot
	@rm ./$*.cc.dot

# Execute parser.
test-parse: $(test_parsed)

$(testparsedtmp_dir)/%.o: $(testsrc_dir)/%.cc
	@test -d $(testparsedtmp_dir) || mkdir -p $(testparsedtmp_dir) 
	@./$(object) -c -o $(testparsedtmp_dir)/$(*F).o $(testsrc_dir)/$*.cc -rose:o $(testparsed_dir)/$*.cc

####### Clean
clean:
	test -d $(tmpdir) && rm -rf $(tmpdir)
	test -f $(object) && rm $(object)

clean-test:
	test -d $(testparsed_dir) && rm -rf $(testparsed_dir)
	test -d $(testdot_dir) && rm -rf $(testdot_dir)

