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


#include <exec/exec.h>
#include <proto/exec.h>
#include <dos/dos.h>
#include <exec/types.h>
#include <libraries/retromode.h>
#include <proto/retromode.h>

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

struct retroSprite *read_icon_or_sprite( 	struct RetroLibrary *libBase , FILE *fd )
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

	sprite = AllocVecTags(  
			sizeof(struct retroSprite) * sprite->number_of_frames ,
			AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );

	if (!sprite) return NULL;



	printf("read icon or sprite\n");

	if (fread( &sprite->number_of_frames,sizeof(sprite->number_of_frames), 1, fd ))
	{
		sprite->frames = AllocVecTags(  
			sizeof(struct retroFrameHeader) * sprite->number_of_frames ,
			AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );
	}

	printf("number of frames %d\n", sprite->number_of_frames);

	for (n=0; n<sprite->number_of_frames; n++ )
	{
		printf("read bob %d header, %08x\n",n, sprite->frames + n);

		if (fread( sprite->frames + n, sizeof(struct retroFrameHeaderShort),1, fd ) == 1)
		{
			sprite->frames[n].bytesPerRow = sprite->frames[n].PlanarXSize * 16 ;

			printf("%d,%d\n",  sprite->frames[n].PlanarXSize * 16 , sprite->frames[n].Height);

			sizeOfPlanar = sprite->frames[n].Height * (sprite->frames[n].PlanarXSize * 2 );
			sizeOfChunky = sprite->frames[n].bytesPerRow  * sprite->frames[n].Height;
	
			sprite->frames[n].data = AllocVecTags(  sizeOfChunky, AVT_Type, MEMF_SHARED, AVT_ClearWithValue, 0, TAG_END );

			printf ("sizeOfPlanar %d sizeOfChunky %d\n", sizeOfPlanar , sizeOfChunky);
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
				printf("colors %d\n", 1 << sprite->frames[n].NumberOfPlanes );

				for (Plane = 0; Plane < sprite->frames[n].NumberOfPlanes; Plane++ )	
				{
					// reset convertion table
					for (num=0;num<256;num++)
					{
						for (bit = 0; bit<8; bit++) 
						{
							convert[ (num<<3) | (7-bit) ] = (num & (1 << bit)) ? 1 : 0;
						}
					}

					if (fread( planar, sizeOfPlanar, 1, fd ) == 1) 
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

//							printf("check start addr %08x - %d %d\n",dest_ptr64, sprite.frames[n].Width, sprite.frames[n].bytesPerRow);

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
				}

				if (planar) 
				{
					FreeVec(planar);
					planar = NULL;
				}
			}
		}

		if (sizeOfPlanar == 0) break;
	}
	return sprite;
}

struct retroSprite * _retromode_retroLoadABKSprite(struct RetroModeIFace *Self,
       char * filename)
{
	struct RetroLibrary *libBase = (struct RetroLibrary *) Self -> Data.LibBase;
	FILE *fd;
	char file_id[5]; // 4 bytes (0 to 3) byte 5 (4)

	fd = fopen("menu.abk","r");

	if (fd)
	{
		file_id[4]= 0;

		if (fgets( file_id, 5, fd ))	// reads 4 bytes but terminates on byte 5.
		{
			if ((strcmp(file_id,"AmSp")==0) || (strcmp(file_id,"Amlc")==0))
			{
				read_icon_or_sprite( libBase, fd );
			}
		}

		fclose(fd);
	}

	return 0;

}

