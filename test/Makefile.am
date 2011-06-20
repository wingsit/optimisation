ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
AM_CXXFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include -I$(top_srcdir)/garch -I$(top_srcdir)/lib -I./ @AM_CXXFLAGS@
AM_LDFLAGS = -lboost_unit_test_framework @AM_LDFLAGS@
bin_PROGRAMS = unit_test
unit_test_SOURCES = \
	test_function.cpp \
	testmain.cpp \
	testgarch.cpp \
	test_timeseries.cpp \
	test_distribution.cpp \
	test_filter.cpp

unit_test_LDADD=../garch/libgarch.la