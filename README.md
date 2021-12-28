# Amiga
Amiga Demos in 68000 ASM from Arnaud Guyon

## Cheap Disk

![CheapDisk Screenshot](CheapDisk.png "CheapDisk Screenshot")

### Description

This is a music disk made for Analog. Works fine on A1200 and is supposed to work also on A500 but there is a bitplan issue on A500.

### Launch

- In folder "CheapDisk" launch Asmone
- Choose memory allocation (for example Chip 1000K). It requires a lot as there are several musics.
- Load the code: "r", then "MusDisk2.s"
- Assemble the code: "a" (no option)
- Run the example: "jr"
- Left mouse to change the musics
- ESC to quit

### To compile MusDisk2.c

- You need to have AmigaOS4 SDK installed.
- Download libBlitter.library from os4Depot.net
- Download libCopper.library project from github
- Copy project Amose-Demo-in-68000-ASM-Cheap-Disk into test folder.
- Add tests/Amiga-Demos-in-68000-ASM-Cheap-Disk/CheapDisk/MusDisk2.c to makefile.

type this in AmigaShell

    make clean
    make make all

