/* :ts=4
 *  $VER: retroPasteSprite.c $Revision$ (02-Nov-2017)
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

/****** retromode/main/retroPasteSprite ******************************************
*
*   NAME
*      retroPasteSprite -- Description
*
*   SYNOPSIS
*      struct retroSprite * retroPasteSprite(struct retroScreen * screen, 
*          struct retroSprite * sprite, int x, int y, int image);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       sprite - 
*       x - 
*       y - 
*       image - 
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

struct retroSprite * _retromode_retroPasteSprite(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       struct retroSprite * sprite,
       int x,
       int y,
       int image)
{

}

