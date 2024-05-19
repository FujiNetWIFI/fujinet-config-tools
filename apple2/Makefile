.PHONY: dist

dist:
	cp dist/bootable.po dist/dist.po
	./dist/add-file.sh dist/dist.po fconfig/fconfig.system FCONFIG.SYSTEM SYS
	./dist/add-file.sh dist/dist.po fconfig/fconfig FCONFIG
	./dist/add-file.sh dist/dist.po flh/flh.system FLH.SYSTEM SYS
	./dist/add-file.sh dist/dist.po flh/flh FLH
	./dist/add-file.sh dist/dist.po fld/fld.system FLD.SYSTEM SYS
	./dist/add-file.sh dist/dist.po fld/fld FLD
	./dist/add-file.sh dist/dist.po feject/feject.system FEJECT.SYSTEM SYS
	./dist/add-file.sh dist/dist.po feject/feject FEJECT

