/* :ts=4
 *  $VER: retroFlash.c $Revision$ (16-Oct-2017)
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

/****** retromode/main/retroFlash ******************************************
*
*   NAME
*      retroFlash -- Description
*
*   SYNOPSIS
*      struct retroFlashTable * retroFlash(struct retroScreen * screen, 
*          unsigned char color, char * data);
*
*   FUNCTION
*
*   INPUTS
*       screen - 
*       color - 
*       data - 
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

char *next_char(char *ptr, char c)
{
	while (*ptr != 0)
	{
		if (*ptr == c) 
		{
			ptr++;
			break;
		}
		ptr++;
	}

	while (*ptr == ' ') ptr++;
	return ptr;
}

int get_int(char *ptr,char **to_ptr)
{
	int ret = 0;

	while (*ptr == ' ') ptr++;

	while ( (*ptr >='0') && (*ptr<='9') )
	{
		ret *= 10;
		ret += *ptr - '0';
		ptr++;
	}

	while (*ptr == ' ') ptr++;

	*to_ptr = ptr;

	return ret;
}


int hex_to_int( char *ptr, char **to_ptr)
{
	unsigned char c = *ptr;
	int ret =0;

	if (ret<15) ret = 0;

	if ((c>='0')&&(c<='9'))	ret = c - '0';
	if ((c>='a')&&(c<='f'))	ret = c - 'a' + 10;
	if ((c>='A')&&(c<='F'))	ret = c - 'A' + 10;

	// next char meybe
	if (*ptr != 0) ptr++;
	*to_ptr = ptr;

	return ret * 0x11;
}


intretromode_retroDeleteFlash(struct RetroModeIFace *Self,  struct retroScreen * screen, unsigned char color);

void _retromode_retroFlash(struct RetroModeIFace *Self,
       struct retroScreen * screen,
       unsigned char color,
       char * data)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	struct retroFlashTable *new_flash = NULL;

	int idx = 0;
	int count = 0;
	int idx_free = -1;

	char *c;
	char *sptr;

	_retromode_retroDeleteFlash(Self, screen, color);

	for (idx = 0; idx<256; idx++)
	{
		if (screen->allocatedFlashs[idx] == NULL)
		{
			idx_free = idx;
			break;
		}
	}


	for (c=data; *c!=0; c++) { if (*c=='(') count ++;	}	

	// no table exists 
	if (!new_flash)	new_flash = (struct retroFlashTable *) libBase -> IExec -> AllocVecTags( sizeof(struct retroFlashTable), AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0 ,TAG_END	);

	if ((new_flash)&&(idx_free>-1))
	{
		struct retroFlash *ptr = NULL;

		new_flash -> color = color;
		new_flash -> colors = count;
		new_flash -> index = 0;
		new_flash -> table = (struct retroFlash *) libBase -> IExec -> AllocVecTags( sizeof(struct retroFlash) * count, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0 ,TAG_END );

		sptr = data;
		for (ptr = new_flash -> table; ptr < new_flash -> table+count; ptr++)
		{
			sptr = next_char( sptr, '(' );
			ptr -> rgb.r =	hex_to_int(sptr, &sptr) ;
			ptr -> rgb.g =	hex_to_int(sptr, &sptr) ;
			ptr -> rgb.b =	hex_to_int(sptr, &sptr) ;

			libBase -> IDOS -> Printf("RGB %lx,%lx,%lx\n", 
					(int) ptr->rgb.r,
					(int) ptr->rgb.g,
					(int) ptr->rgb.b);

			sptr = next_char( sptr, ',' );
			ptr -> delay = get_int(sptr,&sptr);
		}

		screen->flashsAllocated++;		// flash tables
		screen->allocatedFlashs[idx_free] = new_flash;
		screen->allocatedFlashs_end = screen->allocatedFlashs + screen-> flashsAllocated;
	}
}

