/* :ts=4
 *  $VER: retroGetMaxPlains.c $Revision$ (04-Jan-2020)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2020 LiveForIt Software.
 *  MIT License..
 *
 * $Id$
 *
 * $Log$
 *
 *
 */


#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>

/****** retromode/main/retroGetMaxPlains ******************************************
*
*   NAME
*      retroGetMaxPlains -- Description
*
*   SYNOPSIS
*      int retroGetMaxPlains(int colors);
*
*   FUNCTION
*
*   INPUTS
*       colors - 
*
*   RESULT
*       The result ...
*
*   EXAMPLE
*
*   NOTES
*
*   BUGS
*
*   SEE ALSO
*
*****************************************************************************
*
*/

int _retromode_retroGetMaxPlains(struct RetroModeIFace *Self,
       int colors)
{
	int p=0;
	colors++;
	while (colors>(1<<p)) p++;
	return p;
}

