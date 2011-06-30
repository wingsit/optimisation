ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} 
ACLOCAL_CXXFLAGS = ${ACLLOCAL_CXXFLAGS} -std=c++0x
AM_CXXFLAGS = -I$(top_srcdir)/src -I$(top_srcdir)/include -I$(top_srcdir)/garch -I$(top_srcdir)/lib -I$(top_srcdir)/optimisation -I./ @AM_CXXFLAGS@
AM_LDFLAGS = -lboost_unit_test_framework @AM_LDFLAGS@
bin_PROGRAMS = unit_test
unit_test_SOURCES = \
	test_filter.cpp \
	test_derivative.cpp \
	testmain.cpp \
	test_newton.cpp \
	test_samplemodel.cpp \
	test_statisticalfunctions.cpp \
	test_normal.cpp \
	test_parser.cpp \
	test_bfgs.cpp \
	test_garch.cpp \
	test_eigen_move_semantic.cpp \	
	spx_data.cpp

#unit_test_LDADD=../garch/libgarch.a