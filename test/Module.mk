################ Source files ##########################################

test/srcs	:= $(wildcard test/*.cc)
test/tsrcs	:= $(wildcard test/?????.cc)
test/tests	:= $(addprefix $O,$(test/tsrcs:.cc=))
test/objs	:= $(addprefix $O,$(test/srcs:.cc=.o))
test/deps	:= ${test/objs:.o=.d}
test/outs	:= ${test/tests:=.out}

################ Compilation ###########################################

.PHONY:	test/all check test/check test/clean

test/all:	${test/tests}

# The correct output of a test is stored in testXX.std
# When the test runs, its output is compared to .std
#
check:		test/check
test/check:	${test/tests}
	@for i in ${test/tests}; do \
	    test="test/$$(basename $$i)";\
	    echo "Running $$test";\
	    $$i < $$test.cc > $$i.out 2>&1;\
	    diff $$test.std $$i.out && rm -f $$i.out;\
	done

${test/tests}: $Otest/%: $Otest/%.o $Otest/stdtest.o ${liba}
	@echo "Linking $@ ..."
	@${CC} ${ldflags} -o $@ $^ ${libs} -lm

################ Maintenance ###########################################

clean:	test/clean
test/clean:
	@if [ -d ${builddir}/test ]; then\
	    rm -f ${test/tests} ${test/objs} ${test/deps} ${test/outs} $Otest/.d;\
	    rmdir ${builddir}/test;\
	fi

${test/objs}: Makefile test/Module.mk ${confs} | $Otest/.d

-include ${test/deps}
