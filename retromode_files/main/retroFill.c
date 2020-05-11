/* :ts=4
 *  $VER: retroFill.c $Revision$ (04-Nov-2017)
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

/****** retromode/main/retroFill ******************************************
*
*   NAME
*      retroFill -- Description
*
*   SYNOPSIS
*      void retroFill(struct retroScreen * screen, int x, int y, 
*          unsigned char color);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       x - 
*       y - 
*       color - 
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

BOOL insideScreenAndReplaceColor(struct retroScreen *screen,unsigned char *sc_mem, int x,int y, unsigned char replace_color)
{
	if ((x>-1)&&(x<screen->realWidth)&&(y>-1)&&(y<screen->realHeight))
	{
		if (sc_mem[ screen -> realWidth * y + x ] == replace_color) return TRUE;
	}
	return FALSE;
}

void AddXY( struct List *list, int x, int y )
{
	struct FillNode *newNode = NULL;
	newNode = (struct FillNode *) IExec-> AllocSysObjectTags( ASOT_NODE, 
			ASO_NoTrack, TRUE, 
			ASONODE_Size, sizeof(struct FillNode), TAG_END);

	newNode -> x = x;
	newNode -> y = y;

	IExec-> AddHead( list, (struct Node *) newNode );
}

void _retromode_retroFill(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       int x,
       int y,
       unsigned char color)
{
	char replace_color;
	struct List list;
	struct FillNode *node = NULL;
	unsigned char *sc_mem = screen -> Memory[screen -> double_buffer_draw_frame];

	IExec-> NewList(&list);

	if ((x>-1)&&(x<screen->realWidth)&&(y>-1)&&(y<screen->realHeight))
	{
		replace_color = sc_mem[ screen -> realWidth * y + x ];
		if (replace_color == color) return;
	}
	else return;
	
	do
	{
		if ( (void *) node == (void *) &list) break;

		if (node)
		{
			x = node -> x;
			y = node -> y;
			IExec-> Remove( (struct Node *) node);
			IExec-> FreeSysObject( ASOT_NODE, node );
			node = NULL;
		}

		sc_mem[ (screen -> realWidth * y) + x ] = color;

		if (insideScreenAndReplaceColor(screen,sc_mem,x,y-1,replace_color)) AddXY( &list, x, y-1 );
		if (insideScreenAndReplaceColor(screen,sc_mem,x,y+1,replace_color)) AddXY( &list, x, y+1 );
		if (insideScreenAndReplaceColor(screen,sc_mem,x-1,y,replace_color)) AddXY( &list, x-1, y );
		if (insideScreenAndReplaceColor(screen,sc_mem,x+1,y,replace_color)) AddXY( &list, x+1, y );

	} while (node = (struct FillNode *) IExec-> GetHead( &list));
}

