# Amiga
Amiga Demos in 68000 ASM from Arnaud Guyon

## Cheap Disk

![CheapDisk Screenshot](CheapDisk.png "CheapDisk Screenshot")

### Description

This is a music disk made for Analog. Works fine on A1200 and is supposed to work also on A500 but there is a bitplan issue on A500.

### To compile MusDisk2.s (680x0 version)

- In folder "CheapDisk" launch Asmone
- Choose memory allocation (for example Chip 1000K). It requires a lot as there are several musics.
- Load the code: "r", then "MusDisk2.s"
- Assemble the code: "a" (no option)
- Run the example: "jr"
- Left mouse to change the musics
- ESC to quit

### To compile MusDisk2.c (PPC version)

- You need to have AmigaOS4 SDK installed.
- Download libBlitter.library from os4Depot.net
- Download libCopper.library project from github
- Copy project Amose-Demo-in-68000-ASM-Cheap-Disk into test folder.
- Add tests/Amiga-Demos-in-68000-ASM-Cheap-Disk/CheapDisk/MusDisk2.c to makefile.

Type this in AmigaShell

    make clean
    make all
    
Before you run it, install the dependencies

    download ptreplay.library from os4depot.net and install it.
    download ptplay.library from os4depot.net and install it.
    
To run it type:    
    
    cd tests/Amiga-Demos-in-68000-ASM-Cheap-Disk/CheapDisk/
    MusDisk2.elf
    
