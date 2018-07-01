/* :ts=4
 *  $VER: retroOrFill.c $Revision$ (04-Nov-2017)
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

#include <stdlib.h>
#include <stdio.h>
#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>
#include <stdarg.h>
#include <libbase.h>

/****** retromode/main/retroOrFill ******************************************
*
*   NAME
*      retroOrFill -- Description
*
*   SYNOPSIS
*      void retroOrFill(struct retroScreen * screen, int x, int y, 
*          int and_mask, unsigned char or_color);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x - 
*       y - 
*       and_mask - 
*       or_color - 
*
*   RESULT
*       This function does not return a result
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

// nice on the stack on recursive call back

struct FillNode
{
	struct Node node;
	int x;
	int y;
};

extern BOOL insideScreenAndReplaceColor(struct retroScreen *screen,unsigned char *sc_mem, int x,int y, unsigned char replace_color);
extern void AddXY( struct RetroLibrary *libBase, struct List *list, int x, int y );

void _retromode_retroOrFill(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int x,
       int y,
       int and_mask,
       unsigned char or_color)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	char replace_color;
	struct List list;
	struct FillNode *node = NULL;
	unsigned char *sc_mem = screen -> Memory[screen -> double_buffer_draw_frame];

	libBase -> IExec-> NewList(&list);

	and_mask |= or_color;	// prevents it from go in loop forever.

	if ((x>-1)&&(x<screen->realWidth)&&(y>-1)&&(y<screen->realHeight))
	{
		replace_color = sc_mem[ screen -> realWidth * y + x ]  & and_mask;
		if (replace_color == or_color) return;
	}
	else return;
	
	do
	{
		if ( (void *) node == (void *) &list) break;

		if (node)
		{
			x = node -> x;
			y = node -> y;
			libBase -> IExec-> Remove( (struct Node *) node);
			libBase -> IExec-> FreeSysObject( ASOT_NODE, node );
			node = NULL;
		}

		sc_mem[ (screen -> realWidth * y) + x ] |= or_color;

		if (insideScreenAndReplaceColor(screen,sc_mem,x,y-1,replace_color)) AddXY( libBase, &list, x, y-1 );
		if (insideScreenAndReplaceColor(screen,sc_mem,x,y+1,replace_color)) AddXY( libBase, &list, x, y+1 );
		if (insideScreenAndReplaceColor(screen,sc_mem,x-1,y,replace_color)) AddXY( libBase, &list, x-1, y );
		if (insideScreenAndReplaceColor(screen,sc_mem,x+1,y,replace_color)) AddXY( libBase, &list, x+1, y );

	} while (node = (struct FillNode *) libBase -> IExec-> GetHead( &list));
}

