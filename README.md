#FujiNet Config Tools
=========

A set of programs to aid in configuring the FujiNet device from the Atari from DOS

|Command  |Description   |Usage|
| :--------------- | :----------- |:----|
|**Fujinet Commands**|||
|FCD|Set Fujinet Host and Path|FCD \<hs#\> \[directory\]|
|FCONFIG|Shows Adapter Configuration|FCONFIG|
|FEJECT|Eject disk image in specified slot|FEJECT \<ds#\>|
|FHOST|Edit or clear a host slot.|FHOST \<hs#\> [hostname]|
|FINFO|Show disk geometry for a disk slot.|FINFO \<ds#\>|
|FLD|Display Device Slots|FLD|
|FLH|Display host slots|FLH|
|FLS|List files on a TNFS server.|FLS \<hs#\>|
|FMALL|Mounts all Device Slots|FMALL|
|FMOUNT|Mount a disk image.|FMOUNT \<ds#\> \<hs#\> \<R\|W\> \<filename\>|
|FNET|Connect to SSID with Password|FNET \[ssid\] \[password\]|
|FNEW|Create a new Disk Image|FNEW \<ds#\> \<hs#\> \<ns\> \<ss\> \<fname\>|
|FRESET|Reset FujiNet and do a Cold start|FRESET|
|FSCAN|Scan for Wireless Networks|FSCAN|
|**Fujinet N: Commands**|||
|NCD|Change Nx: Working path.|NCD [Nx:]\<path\>|
|NCOPY|Copy files to and from network.|NCOPY \<source\>,\<dest\>|
|NDEL|Delete File on Network.|NDEL \<devicespec\>|
|NDIR|Display Network Directory.|NDIR [devicespec]|
|NMKDIR|Make Directory on Network.|NMKDIR \<directory\>|
|NPWD|Show Nx: working path.|NPWD [Nx:]|
|NREN|Rename File on Network.|NREN \<devicespec:oldname\>,\<newname\>|
|NRMDIR|Remove Directory on Network.|NRMDIR \<directory\>|
|NTRANS|Set CR/LF Translation Mode for N:.|NTRANS \<Nx:\> \<mode\>|
|**Fujinet R: Commands**|||
|RBAUDLOK|Lock baud rate.|RBAUDLOK \<baud\|UNLOCK\>|
|RLISTEN|Listen for TCP connection for R:|RLISTEN \<port#\>|
