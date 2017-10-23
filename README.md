# RetroMode-AmigaOS-GFXLIB
RetroMode graphic library simulated ECS/AGA effects.

Retro mode is for AmigaOS4.1

to compile type:

make install

make clean

make 

make install

first install, installs the header files into the AmigaOS4.1 SDK.

"make clean" 

Delete any old .o files, if RetroMode.library or tests crashes then this is a likely caused by outdated .o files.

structs in header files are often changed, if structs are allocated incorrect size to
a old sizeof(struct) then memory gets over written, result is a system crash. if things that are removed is being used by old .o files system crash and so on.

"make"

Builds the library

"make Install"

copies the header tils into the SDK, 
and copies the library into the libs: folder.

After you have installed header files and library

cd tests

type "make clean", to delete old binrary files.

type "make" to build the tests.


