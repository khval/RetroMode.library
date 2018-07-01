/* :ts=4
 *  $VER: retroLoadSprite.c $Revision$ (24-Jun-2018)
 *
 *  This file is part of retromode.
 *
 *  Copyright (c) 2018 LiveForIt Software.
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
#include <libbase.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/****** retromode/main/retroLoadSprite ******************************************
*
*   NAME
*      retroLoadSprite -- Description
*
*   SYNOPSIS
*      struct retroSprite * retroLoadSprite(struct retrofd * fd);
*
*   FUNCTION
*
*   INPUTS
*       fd - 
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


struct retroSprite * _retromode_retroLoadSprite(struct RetroModeIFace *Self, FILE *fd, cust_fread_t cust_fread)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	int n;
	int num;
	int bit;
	int Plane;
	unsigned char *byte;
	unsigned char convert[ (255<<3)+8];
	unsigned int sizeOfPlanar,sizeOfChunky;
	char *planar;
	struct retroSprite *sprite;
	short ECSColor;


	sprite = (struct retroSprite *) libBase -> IExec -> AllocVecTags(  sizeof(struct retroSprite), AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );

	if (!sprite) return NULL;

	if ( cust_fread( &sprite->number_of_frames,sizeof(sprite->number_of_frames), 1, fd ) == 1 )
	{
		libBase -> IDOS -> Printf("Load sprite->number_of_frames %ld\n",  sprite->number_of_frames);


		sprite->frames = libBase -> IExec -> AllocVecTags(  
			sizeof(struct retroFrameHeader) * sprite->number_of_frames ,
			AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );
	}

	if (sprite->frames == NULL) sprite->number_of_frames=0;

	for (n=0; n<sprite->number_of_frames; n++ )
	{

		if (cust_fread( sprite->frames + n, sizeof(struct retroFrameHeaderShort), 1, fd ) == 1 )
		{
			sprite->frames[n].bytesPerRow = sprite->frames[n].PlanarXSize * 16 ;
			sizeOfPlanar = sprite->frames[n].Height * (sprite->frames[n].PlanarXSize * 2 );
			sizeOfChunky = sprite->frames[n].bytesPerRow  * sprite->frames[n].Height;
	
			sprite->frames[n].data = libBase -> IExec -> AllocVecTags(  sizeOfChunky, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );
		}
		else
		{
			sizeOfPlanar = 0;
			sizeOfChunky = 0;
		}

		if (sizeOfPlanar>0)
		{
			planar = libBase -> IExec -> AllocVecTags( sizeOfPlanar, 	AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );
			if (planar)
			{
				// reset convertion table
				for (num=0;num<256;num++)
				{
					for (bit = 0; bit<8; bit++) 
					{
						convert[ (num<<3) | (7-bit) ] = (num & (1 << bit)) ? 1 : 0;
					}
				}

				for (Plane = 0; Plane < sprite->frames[n].NumberOfPlanes; Plane++ )	
				{
					if (cust_fread( planar, sizeOfPlanar, 1, fd ) == 1) 
					{
						int y;
						int source_BytesPerRow = sprite->frames[n].PlanarXSize*2;
						char *source = planar;
						char *source_ptr = planar;
						char *source_end = planar + source_BytesPerRow;
						long long int *dest_ptr64;	 // 8bits = 8 pixels, 8 pixels is 8 bytes = 64bit :-)

						for (y=0; y<sprite->frames[n].Height;y++)
						{
							dest_ptr64 = (long long int *) (sprite->frames[n].data + (sprite->frames[n].bytesPerRow * y));

							// we can unroll it 16 bit alligned remeber ;-)
							for (source_ptr = source; source_ptr < source_end;  source_ptr++)
							{
								*dest_ptr64++ |= *((long long int *) ( (char *) convert + ((*source_ptr)<<3)));
							}

							source += source_BytesPerRow;
							source_end += source_BytesPerRow;
						}
					}

					// next plane
					for (byte = convert; byte < convert + ((255<<3)+8); byte++ )
					{ 
						*byte = *byte << 1;
					}
				} // Next

				if (planar) 
				{
					libBase -> IExec -> FreeVec(planar);
					planar = NULL;
				}
			}
		}

		if (sizeOfPlanar == 0) break;
	} // Next


	// in doc it says 32 colors, but we are flexible.

	num = 0;
	while (cust_fread( &ECSColor, 2, 1, fd ) == 1)	
	{
		ECSColorToRGB32( ECSColor, sprite -> palette[num] );
		num++;
		if (num==256) break;
	}

	return sprite;
}

