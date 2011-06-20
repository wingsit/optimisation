ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}
AM_CXXFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include -I$(top_srcdir)/garch -I$(top_srcdir)/lib @AM_CXXFLAGS@
AM_LDFLAGS = -lboost_unit_test_framework @AM_LDFLAGS@

lib_LTLIBRARIES = libgarch.la
libgarch_ladir = .
libgarch_la_HEADERS = \
	timeseriesmodel.hpp \
	typedef.hpp \
	timeseries.hpp
libgarch_la_SOURCES = \
	timeseriesmodel.cpp \
	timeseries.cpp \
	estimationengine.cpp
