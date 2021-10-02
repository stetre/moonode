
default: build

build install uninstall where:
	@cd src;		$(MAKE) $@

clean cleanall:
	@cd src;		$(MAKE) $@
	@cd doc;		$(MAKE) $@

docs:
	@cd doc;		$(MAKE)

backup: clean
