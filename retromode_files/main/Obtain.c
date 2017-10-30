/* :ts=4
 *  $VER: Obtain.c $Revision$ (03-Oct-2017)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2017 LiveForIt Software.
 *  MIT License.
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

#include "libbase.h"

/****** retromode/main/Obtain ******************************************
*
*   NAME
*      Obtain -- Description
*
*   SYNOPSIS
*      ULONG Obtain(void);
*
*   FUNCTION
*
*   INPUTS
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

ULONG _retromode_Obtain(struct RetroModeIFace *Self)
{
    return Self -> Data.RefCount++;
}

