// 30000 us
// Always displays the correct frame regardless of lag spikes
// In low framerates, will skip frames
uint8_t	stt_animate_precise(t_sheet *sheet, long dt)
{
	uint8_t	rvalue;
	long	frame_increments;

	sheet->frame_dt += dt;
	if (sheet->frame_dt < sheet->frame_time)
		return (0);
	frame_increments = (sheet->frame_dt / sheet->frame_time);			// Integer division
	rvalue = 1 + ((sheet->index + frame_increments) > sheet->count);
	sheet->index = (sheet->index + frame_increments) % sheet->count;	// Integer division
	sheet->frame_dt -= frame_increments * sheet->frame_time;
	return (rvalue);
}