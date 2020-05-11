
#define ROW_ARGS (unsigned char *source_row_start, unsigned char *source_row_end, unsigned char *destination_row_start, int srcMask, int destMask)

extern void _left_right_solid ROW_ARGS
{
	unsigned char *source_row_ptr;
	unsigned char *destination_row_ptr = destination_row_start;

	for ( source_row_ptr = source_row_start;  source_row_ptr < source_row_end ; source_row_ptr++ )
	{
		*destination_row_ptr= *source_row_ptr;
		destination_row_ptr++;
	}
}

extern void _right_left_solid ROW_ARGS
{
	unsigned char *source_row_ptr;
	unsigned char *destination_row_ptr = destination_row_start;

	for ( source_row_ptr = source_row_end-1;   source_row_ptr >= source_row_start  ; source_row_ptr-- )
	{
		*destination_row_ptr= *source_row_ptr;
		destination_row_ptr++;
	}
}
