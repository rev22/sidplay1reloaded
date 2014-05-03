# Generated automatically from Makefile.in by configure.

SHELL			= /bin/sh

srcdirs 		= libsidplay console

.PHONY: all
all:
		@for subdir in $(srcdirs); do \
			(cd $$subdir && $(MAKE) all) || exit 1; \
		done

.PHONY: clean
clean:
		@for subdir in $(srcdirs); do \
			(cd $$subdir && $(MAKE) clean) || exit 1; \
		done

.PHONY: depend
depend:
		@for subdir in $(srcdirs); do \
			(cd $$subdir && $(MAKE) depend) || exit 1; \
		done

.PHONY: install
install:
		@for subdir in $(srcdirs); do \
			(cd $$subdir && $(MAKE) install) || exit 1; \
		done