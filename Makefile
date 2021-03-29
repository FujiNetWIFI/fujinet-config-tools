
##
## FujiNet tools Master Make file.
## Hack-o-matic, for sure, it will get better.
##

TOOLS:= fcd fconfig fcopy feject fesclk fhost finfo fld flh fls fmall \
	fmount fnet fnew freset fscan ncd ncopy ndel ndir nlogin \
	nmkdir npwd nren nrmdir ntrans rautoans rbaudlok rlisten rsniff

TOPTARGETS := mostlyclean clean install zip zap

.PHONY: all dist $(TOOLS) $(TOPTARGETS)

.SUFFIXES:

TOOLFILES := $(foreach tool,$(TOOLS),$(tool)/$(tool).com)

all: $(TOOLFILES)

dist: $(TOOLFILES)
	mkdir -p dist
	mkdir -p dist-doc
	cp $^ dist/
	cp doc/* dist-doc/
	dir2atr 720 fnc-tools.atr dist/
	dir2atr 720 fnc-tools-doc.atr dist-doc/

# Make clean, zip, etc. recurse into the tool directories
$(TOPTARGETS): $(TOOLS)
$(TOOLS):
	@$(MAKE) -C $@ --no-print-directory $(MAKECMDGOALS)

# Generate targets for the tools
define make-tool-target
$1/$1.com:
	@$$(MAKE) -C $$(dir $$@) --no-print-directory $$(notdir $$@)
endef
$(foreach tool,$(TOOLS),$(eval $(call make-tool-target,$(tool))))
