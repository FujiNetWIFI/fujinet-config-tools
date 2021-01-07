
##
## FujiNet tools Master Make file.
## Hack-o-matic, for sure, it will get better.
##

.PHONY: all mostlyclean clean install zip

.SUFFIXES:

all mostlyclean clean install zip zap:
	@$(MAKE) -C fconfig   --no-print-directory $@	
	@$(MAKE) -C feject    --no-print-directory $@
	@$(MAKE) -C fesclk    --no-print-directory $@
	@$(MAKE) -C fhost     --no-print-directory $@
	@$(MAKE) -C finfo     --no-print-directory $@
	@$(MAKE) -C fcd       --no-print-directory $@
	@$(MAKE) -C fld       --no-print-directory $@
	@$(MAKE) -C flh       --no-print-directory $@
	@$(MAKE) -C fls       --no-print-directory $@
	@$(MAKE) -C fmount    --no-print-directory $@
	@$(MAKE) -C fmall     --no-print-directory $@
	@$(MAKE) -C fnet      --no-print-directory $@
	@$(MAKE) -C fnew      --no-print-directory $@
	@$(MAKE) -C freset    --no-print-directory $@
	@$(MAKE) -C fscan     --no-print-directory $@
	@$(MAKE) -C rlisten   --no-print-directory $@
	@$(MAKE) -C ncd       --no-print-directory $@
	@$(MAKE) -C npwd      --no-print-directory $@
	@$(MAKE) -C ndir      --no-print-directory $@
	@$(MAKE) -C ncopy     --no-print-directory $@
	@$(MAKE) -C ndel      --no-print-directory $@
	@$(MAKE) -C nren      --no-print-directory $@
	@$(MAKE) -C nmkdir    --no-print-directory $@
	@$(MAKE) -C nrmdir    --no-print-directory $@
	@$(MAKE) -C ntrans    --no-print-directory $@
	@$(MAKE) -C rautoans  --no-print-directory $@
	@$(MAKE) -C rbaudlok  --no-print-directory $@
	@$(MAKE) -C rsniff    --no-print-directory $@

dist: all
	mkdir -p dist
	mkdir -p dist-doc
	cp fconfig/fconfig.com dist/
	cp feject/feject.com dist/
	cp fesclk/fesclk.com dist/
	cp fhost/fhost.com dist/
	cp finfo/finfo.com dist/
	cp fcd/fcd.com dist/
	cp fld/fld.com dist/
	cp flh/flh.com dist/
	cp fls/fls.com dist/
	cp fmount/fmount.com dist/
	cp fmall/fmall.com dist/
	cp fnet/fnet.com dist/
	cp fnew/fnew.com dist/
	cp freset/freset.com dist/
	cp fscan/fscan.com dist/
	cp rlisten/rlisten.com dist/
	cp ncd/ncd.com dist/
	cp npwd/npwd.com dist/
	cp ndir/ndir.com dist/
	cp ncopy/ncopy.com dist/
	cp ndel/ndel.com dist/
	cp nmkdir/nmkdir.com dist/
	cp nrmdir/nrmdir.com dist/
	cp ntrans/ntrans.com dist/
	cp nren/nren.com dist/
	cp rautoans/rautoans.com dist/
	cp rbaudlok/rbaudlok.com dist/
	cp rsniff/rsniff.com dist/
	cp -ax doc/* dist-doc/ 
	dir2atr 720 fnc-tools.atr dist/
	dir2atr 720 fnc-tools-doc.atr dist-doc/
