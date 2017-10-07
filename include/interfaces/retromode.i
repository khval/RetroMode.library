#ifndef RETROMODE_INTERFACE_DEF_H
#define RETROMODE_INTERFACE_DEF_H
/*
** This file is machine generated from idltool
** Do not edit
*/ 

#include <exec/types.i>
#include <exec/exec.i>
#include <exec/interfaces.i>

STRUCTURE retromodeIFace, InterfaceData_SIZE
	    FPTR Iretromode_Obtain
	    FPTR Iretromode_Release
	    FPTR Iretromode_Expunge
	    FPTR Iretromode_Clone
	    FPTR Iretromode_alloc_retoVideo
	    FPTR Iretromode_free_retroVideo
	    FPTR Iretromode_clear_retroVideo
	    FPTR Iretromode_draw_retroVideo
	    FPTR Iretromode_dma_retroVideo
	    FPTR Iretromode_set_retroVideoColor
	    FPTR Iretromode_applyCopper
	    FPTR Iretromode_retroOpenScreen
	    FPTR Iretromode_retroCloseScreen
	    FPTR Iretromode_retroApplyScreen
	    FPTR Iretromode_retroBAR
	    FPTR Iretromode_retroScreenColor
	LABEL retromodeIFace_SIZE

#endif
