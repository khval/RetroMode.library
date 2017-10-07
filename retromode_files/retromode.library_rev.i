VERSION		EQU	1
REVISION	EQU	2

DATE	MACRO
		dc.b '3.10.2017'
		ENDM

VERS	MACRO
		dc.b 'retromode.library 1.2'
		ENDM

VSTRING	MACRO
		dc.b 'retromode.library 1.2 (3.10.2017)',13,10,0
		ENDM

VERSTAG	MACRO
		dc.b 0,'$VER: retromode.library 1.2 (3.10.2017)',0
		ENDM
