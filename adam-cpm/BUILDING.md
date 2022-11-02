# Building the tools

To build each tool, you will need the following tools:

* Z88DK: http://github.com/z88dk/z88dk
* fujinet-adam-cpm-lib: https://github.com/FujiNetWIFI/fujinet-adam-cpm-lib

fujinet-adam-cpm-lib should be compiled and the fujicpm.lib placed in $Z88DK/lib/clibs, with the src/fujicpm.h file placed in $Z88DK/include

# Making the disk

To make the distribution disk, you will need 

* cpmtools installed in your path, http://www.moria.de/~michael/cpmtools/files/cpmtools-2.21.tar.gz
* John Elliott's offset patch applied: 

```patch
Common subdirectories: cpmtools-2.21-orig/badfs and cpmtools-2.21-new/badfs
diff -u cpmtools-2.21-orig/cpmfs.c cpmtools-2.21-new/cpmfs.c
--- cpmtools-2.21-orig/cpmfs.c	2020-09-14 19:48:00.000000000 +0100
+++ cpmtools-2.21-new/cpmfs.c	2021-11-21 23:12:59.721828873 +0000
@@ -333,6 +333,16 @@
 }
 /*}}}*/
 
+/* bootOffset        -- find the logical sector number of the CP/M directory */
+/*{{{*/
+static int bootOffset(const struct cpmSuperBlock *d)
+{
+	assert(d);
+	if (d->bootsec >= 0) return d->bootsec;
+	return d->boottrk * d->sectrk;
+}
+/*}}}*/
+
 /* logical block I/O */
 /* readBlock          -- read a (partial) block                  */ /*{{{*/
 static int readBlock(const struct cpmSuperBlock *d, int blockno, char *buffer, int start, int end)
@@ -352,8 +362,8 @@
     return -1;
   }
   if (end<0) end=d->blksiz/d->secLength-1;
-  sect=(blockno*(d->blksiz/d->secLength)+ d->sectrk*d->boottrk)%d->sectrk;
-  track=(blockno*(d->blksiz/d->secLength)+ d->sectrk*d->boottrk)/d->sectrk;
+  sect =(blockno*(d->blksiz/d->secLength)+ bootOffset(d))%d->sectrk;
+  track=(blockno*(d->blksiz/d->secLength)+ bootOffset(d))/d->sectrk;
   for (counter=0; counter<=end; ++counter)
   {
     const char *err;
@@ -394,8 +404,8 @@
   fprintf(stderr,"writeBlock: write block %d %d-%d\n",blockno,start,end);
 #endif
   if (end < 0) end=d->blksiz/d->secLength-1;
-  sect = (blockno*(d->blksiz/d->secLength))%d->sectrk;
-  track = (blockno*(d->blksiz/d->secLength))/d->sectrk+d->boottrk;
+  sect  = (blockno*(d->blksiz/d->secLength) + bootOffset(d)) % d->sectrk;
+  track = (blockno*(d->blksiz/d->secLength) + bootOffset(d)) / d->sectrk;
   for (counter = 0; counter<=end; ++counter)
   {
     const char *err;
@@ -729,7 +739,7 @@
       if (argc==1 && strcmp(argv[0],"end")==0)
       {
         insideDef=0;
-        d->size=(d->secLength*d->sectrk*(d->tracks-d->boottrk))/d->blksiz;
+        d->size= (d->sectrk*d->tracks-bootOffset(d)) * d->secLength / d->blksiz;
         if (d->extents==0) d->extents=((d->size>=256 ? 8 : 16)*d->blksiz)/16384;
         if (d->extents==0) d->extents=1;
         if (found) break;
@@ -777,6 +787,7 @@
           }
         }
         else if (strcmp(argv[0],"boottrk")==0) d->boottrk=strtol(argv[1],(char**)0,0);
+        else if (strcmp(argv[0],"bootsec")==0) d->bootsec=strtol(argv[1],(char**)0,0);
         else if (strcmp(argv[0],"offset")==0)  
         {
           off_t val;
@@ -869,7 +880,7 @@
       d->type=CPMFS_DR22;
       d->skewtab=(int*)0;
       d->offset=0;
-      d->blksiz=d->boottrk=d->secLength=d->sectrk=d->tracks=-1;
+      d->blksiz=d->boottrk=d->bootsec=d->secLength=d->sectrk=d->tracks=-1;
       d->libdskGeometry[0] = 0;
       if (strcmp(argv[1],format)==0) found=1;
     }
@@ -881,9 +892,9 @@
     fprintf(stderr,"%s: unknown format %s\n",cmd,format);
     exit(1);
   }
-  if (d->boottrk<0)
+  if (d->boottrk<0 && d->bootsec<0)
   {
-    fprintf(stderr, "%s: boottrk parameter invalid or missing from diskdef\n",cmd);
+    fprintf(stderr, "%s: boottrk / bootsec parameter invalid or missing from diskdef\n",cmd);
     exit(1);
   }
   if (d->secLength<0)
@@ -1050,7 +1061,7 @@
   /*}}}*/
   /* initialise allocation vector bitmap */ /*{{{*/
   {
-    d->alvSize=((d->secLength*d->sectrk*(d->tracks-d->boottrk))/d->blksiz+INTBITS-1)/INTBITS;
+    d->alvSize= ((((d->sectrk * d->tracks)-bootOffset(d)) * d->secLength)/d->blksiz+INTBITS-1)/INTBITS;
     if ((d->alv=malloc(d->alvSize*sizeof(int)))==(int*)0) 
     {
       boo=strerror(errno);
diff -u cpmtools-2.21-orig/diskdefs.5 cpmtools-2.21-new/diskdefs.5
--- cpmtools-2.21-orig/diskdefs.5	2020-09-20 15:41:07.000000000 +0100
+++ cpmtools-2.21-new/diskdefs.5	2021-11-21 23:14:10.327772901 +0000
@@ -1,7 +1,7 @@
 .\" Believe it or not, reportedly there are nroffs which do not know \(en
 .if n .ds en -
 .if t .ds en \(en
-.TH DISKDEFS 5 "Sep 13, 2020" "CP/M tools" "File formats"
+.TH DISKDEFS 5 "Sep 20, 2020" "CP/M tools" "File formats"
 .SH NAME \"{{{roff}}}\"{{{
 diskdefs \- CP/M disk and file system format definitions
 .\"}}}
@@ -21,6 +21,7 @@
   \fBblocksize\fP \fIsize\fP
   \fBmaxdir\fP \fIcount\fP
   \fBboottrk\fP \fInumber\fP
+  [\fBbootsec\fP \fInumber\fP]
   [\fBskew\fP \fInumber\fP]
   [\fBskewtab\fP \fIsector\fP[\fB,\fP\fIsector\fP]...]
   [\fBos\fP \fB2.2\fP|\fB3\fP|\fBisx\fP|\fBp2dos\fP|\fBzsys\fP]
diff -u cpmtools-2.21-orig/diskdefs.5.in cpmtools-2.21-new/diskdefs.5.in
--- cpmtools-2.21-orig/diskdefs.5.in	2015-03-16 12:56:09.000000000 +0000
+++ cpmtools-2.21-new/diskdefs.5.in	2021-11-21 23:13:50.416352544 +0000
@@ -21,6 +21,7 @@
   \fBblocksize\fP \fIsize\fP
   \fBmaxdir\fP \fIcount\fP
   \fBboottrk\fP \fInumber\fP
+  [\fBbootsec\fP \fInumber\fP]
   [\fBskew\fP \fInumber\fP]
   [\fBskewtab\fP \fIsector\fP[\fB,\fP\fIsector\fP]...]
   [\fBos\fP \fB2.2\fP|\fB3\fP|\fBisx\fP|\fBp2dos\fP|\fBzsys\fP]

```

Once applied and installed, running ./make-dist.sh will create a new disk image.

