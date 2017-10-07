VERSION = 1
REVISION = 2

.macro DATE
.ascii "3.10.2017"
.endm

.macro VERS
.ascii "retromode.library 1.2"
.endm

.macro VSTRING
.ascii "retromode.library 1.2 (3.10.2017)"
.byte 13,10,0
.endm

.macro VERSTAG
.byte 0
.ascii "$VER: retromode.library 1.2 (3.10.2017)"
.byte 0
.endm
