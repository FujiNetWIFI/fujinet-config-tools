# FujiNet Config Tools

A set of programs to aid in configuring the FujiNet device from an Atari via DOS.

## Building

Ensure you have a well configured cc65 installation.

```shell
cd atari
make clean all dist
```

## Usage

Usage from command-line based DOSes (like SpartaDOS) are shown here.

When running commands from a menu-based DOS (like Atari DOS 2.5),
those that require arguments will prompt you to enter them.
And to give you a chance to see the output before the DOS menu
reappears, you'll be prompted to press [Return] to proceed.

|Command  |Description   |Usage|
| :--------------- | :----------- |:----|
|**Fujinet Config Commands**|||
|FCD.COM|Set Fujinet Host and Path|FCD \<hs#\> \[directory\]|
|FCONFIG.COM|Shows Adapter Configuration|FCONFIG|
|FEJECT.COM|Eject disk image in specified slot|FEJECT \<ds#\>|
|FESCLK.COM|Set SIO Clock rate|FESCLK \<kHz\>|
|FHOST.COM|Edit or clear a host slot.|FHOST \<hs#\> [hostname]|
|FINFO.COM|Show disk geometry for a disk slot.|FINFO \<ds#\>|
|FLD.COM|Display Device Slots|FLD|
|FLH.COM|Display host slots|FLH|
|FLS.COM|List files on a TNFS server.|FLS \<hs#\>|
|FMALL.COM|Mounts all Device Slots|FMALL|
|FMOUNT.COM|Mount a disk image.|FMOUNT \<ds#\> \<hs#\> \<R\|W\> \<filename\>|
|FNET.COM|Connect to SSID with Password|FNET \[ssid\] \[password\]|
|FNEW.COM|Create a new Disk Image|FNEW \<ds#\> \<hs#\> \<ns\> \<ss\> \<fname\>|
|FRESET.COM|Reset FujiNet and do a Cold start|FRESET|
|FSCAN.COM|Scan for Wireless Networks|FSCAN|
|**Fujinet N: Commands**|||
|NCD.COM|Change Nx: Working path.|NCD [Nx:]\<path\>|
|NCOPY.COM|Copy files to and from network.|NCOPY \<source\>,\<dest\>|
|NDEL.COM|Delete File on Network.|NDEL \<devicespec\>|
|NDIR.COM|Display Network Directory.|NDIR [devicespec]|
|NMKDIR.COM|Make Directory on Network.|NMKDIR \<directory\>|
|NPWD.COM|Show Nx: working path.|NPWD [Nx:]|
|NREN.COM|Rename File on Network.|NREN \<devicespec:oldname\>,\<newname\>|
|NRMDIR.COM|Remove Directory on Network.|NRMDIR \<directory\>|
|NTRANS.COM|Set CR/LF Translation Mode for N:.|NTRANS \<Nx:\> \<mode\>|
|**Fujinet R: Commands**|||
|RAUTOANS.COM|Set auto-answer|RAUTOANS \<0\|1\>|
|RBAUDLOK.COM|Lock baud rate.|RBAUDLOK \<baud\|UNLOCK\>|
|RLISTEN.COM|Listen for TCP connection for R:|RLISTEN \<port#\>|
|RSNIFF.COM|Enable/disable modem sniffer|RSNIFF \<0\|1\>|

Documentation for each tool is available in the `fnc-tools-doc.atr` disk image.
