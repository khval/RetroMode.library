/* :ts=4
 *  $VER: retroLoadABKSprite.c $Revision$ (02-Nov-2017)
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
//include <proto/dos.h>
#include <libbase.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/****** retromode/main/retroLoadABKSprite ******************************************
*
*   NAME
*      retroLoadABKSprite -- Description
*
*   SYNOPSIS
*      struct retroSprite * retroLoadABKSprite(char * filename);
*
*   FUNCTION
*
*   INPUTS
*       filename - 
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

#define AllocVecTags libBase->IExec->AllocVecTags
#define FreeVec libBase->IExec->FreeVec

struct retroSprite *read_icon_or_sprite( 	struct RetroLibrary *libBase , BPTR fd )
{
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


	sprite = AllocVecTags(  sizeof(struct retroSprite), AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );

	if (!sprite) return NULL;

	if ( libBase->IDOS->Read( fd, &sprite->number_of_frames,sizeof(sprite->number_of_frames)) == sizeof(sprite->number_of_frames) )
	{
		sprite->frames = AllocVecTags(  
			sizeof(struct retroFrameHeader) * sprite->number_of_frames ,
			AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );
	}

	for (n=0; n<sprite->number_of_frames; n++ )
	{

		if (libBase->IDOS->Read( fd, sprite->frames + n, sizeof(struct retroFrameHeaderShort)) == sizeof(struct retroFrameHeaderShort))
		{
			sprite->frames[n].bytesPerRow = sprite->frames[n].planarXSize * 16 ;
			sizeOfPlanar = sprite->frames[n].height * (sprite->frames[n].planarXSize * 2 );
			sizeOfChunky = sprite->frames[n].bytesPerRow  * sprite->frames[n].height;
	
			sprite->frames[n].data = AllocVecTags(  sizeOfChunky, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );
		}
		else
		{
			sizeOfPlanar = 0;
			sizeOfChunky = 0;
		}

		if (sizeOfPlanar>0)
		{
			planar = AllocVecTags( sizeOfPlanar, 	AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );
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

				for (Plane = 0; Plane < sprite->frames[n].numberOfPlanes; Plane++ )	
				{
					if (libBase->IDOS->Read( fd, planar, sizeOfPlanar ) == sizeOfPlanar) 
					{
						int y;
						int source_BytesPerRow = sprite->frames[n].planarXSize*2;
						char *source = planar;
						char *source_ptr = planar;
						char *source_end = planar + source_BytesPerRow;
						long long int *dest_ptr64;	 // 8bits = 8 pixels, 8 pixels is 8 bytes = 64bit :-)

						for (y=0; y<sprite->frames[n].height;y++)
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
					FreeVec(planar);
					planar = NULL;
				}
			}
		}

		if (sizeOfPlanar == 0) break;
	} // Next


	// in doc it says 32 colors, but we are flexible.

	num = 0;
	while (libBase->IDOS->Read( fd, &ECSColor, 2 ) == 2)	
	{
		ECSColorToRGB32( ECSColor, sprite -> palette[num] );
		num++;
		if (num==256) break;
	}

	return sprite;
}

struct retroSprite * _retromode_retroLoadABKSprite(struct RetroModeIFace *Self,
       char * filename)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	struct retroSprite *sprite = NULL;
	BPTR fd;
	char file_id[5]; // 4 bytes (0 to 3) byte 5 (4)

	fd = libBase->IDOS ->Open( (char *) filename,MODE_OLDFILE);

	if (fd)
	{
		file_id[4]= 0;

		if (libBase->IDOS->Read( fd, file_id, 4 ))	// reads 4 bytes but terminates on byte 5.
		{
			if ((strcmp(file_id,"AmSp")==0) || (strcmp(file_id,"AmIc")==0))
			{
				sprite = read_icon_or_sprite( libBase, fd );
			}
			else
			{
				libBase->IDOS->Printf("[%s]\n",file_id);
			}
		}

		libBase->IDOS->Close(fd);
	}

	return sprite;
}

