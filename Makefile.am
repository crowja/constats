AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4

SUBDIRS = src tests
dist_noinst_SCRIPTS = autogen.sh

maintainer-scrub:
	-@rm -f *~ core *.tgz *.tar.gz *.tar.bz2
	-@make maintainer-clean

clean-generic:
	rm -f core core.* a.out *.BAK *.bak *~
