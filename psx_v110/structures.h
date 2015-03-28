#pragma once

#include "defines.h"

/* CPU */

union t_opcode
{
	u32 full;
	struct
	{
		unsigned	  immediate : 16;
		unsigned			rt : 5;
		unsigned			rs : 5;
		unsigned			op : 6;
	};

	struct
	{
		unsigned		target : 26;
		unsigned	    	 op : 6;
	};

	struct
	{
		unsigned		 funct : 6;
		unsigned		 shamt : 5;
		unsigned			rd : 5;
		unsigned			rt : 5;
		unsigned			rs : 5;
		unsigned			op : 6;
	};
};

/* COP0 */

union t_sr
{
	u32 full;
	struct
	{
		unsigned IEc : 1;
		unsigned KUc : 1;
		unsigned IEp : 1;
		unsigned KUp : 1;
		unsigned IEo : 1;
		unsigned KUo : 1;
		unsigned     : 2;
		unsigned Im  : 8;
		unsigned Isc : 1;
		unsigned Swc : 1;
		unsigned PZ  : 1;
		unsigned CM  : 1;
		unsigned PE  : 1;
		unsigned TS  : 1;
		unsigned BEV : 1;
		unsigned     : 2;
		unsigned RE  : 1;
		unsigned     : 2;
		unsigned CU0 : 1;
		unsigned CU1 : 1;
		unsigned CU2 : 1;
		unsigned CU3 : 1;
	};
};

union t_epc
{
	u32 full;
};

union t_cause
{
	u32 full;
	struct
	{
		unsigned zr1    : 2;
		unsigned EXCODE : 5;
		unsigned zr2    : 1;
		unsigned Ip     : 8;
		unsigned zr3    : 10;
		unsigned CE		: 2;
		unsigned zr4    : 1;
		unsigned BD     : 1;
	};
};

/* INTERRUPT MANAGEMENT */

union t_intreg
{
	u32 full;
	struct
	{
		unsigned vsync : 1;
		unsigned gpu   : 1;
		unsigned cdrom : 1;
		unsigned dma   : 1;
		unsigned tmr0  : 1;
		unsigned tmr1  : 1;
		unsigned tmr2  : 1;
		unsigned joy   : 1;
		unsigned sio   : 1;
		unsigned spu   : 1;
		unsigned       : 22;
	};
};

/* DMA */

union t_bcr
{
	u32 full;
	struct
	{
		unsigned bc : 16;
		unsigned zr : 16;
	};
	struct
	{
		unsigned bs : 16;
		unsigned ba : 16;
	};

};

union t_chcr
{
	u32 full;
	struct
	{
		unsigned direction   : 1;
		unsigned memory_step : 1;
		unsigned zr1         : 7;
		unsigned mode        : 2;
		unsigned zr2         : 13;
		unsigned busy1       : 1;
		unsigned zr3         : 3;
		unsigned busy2       : 1;
		unsigned zr4         : 3;
	};
};

union t_dpcr
{
	u32 full;
	struct
	{
		unsigned dma0_priority : 3;
		unsigned dma0_enable : 1;
		unsigned dma1_priority : 3;
		unsigned dma1_enable : 1;
		unsigned dma2_priority : 3;
		unsigned dma2_enable : 1;
		unsigned dma3_priority : 3;
		unsigned dma3_enable : 1;
		unsigned dma4_priority : 3;
		unsigned dma4_enable : 1;
		unsigned dma5_priority : 3;
		unsigned dma5_enable : 1;
		unsigned dma6_priority : 3;
		unsigned dma6_enable : 1;
		unsigned : 4;
	};
};

union t_dicr
{
	u32 full;
	struct
	{
		unsigned    : 6;
		unsigned zr : 9;
		unsigned force_irq : 1;

		unsigned  enable_0 : 1;
		unsigned  enable_1 : 1;
		unsigned  enable_2 : 1;
		unsigned  enable_3 : 1;
		unsigned  enable_4 : 1;
		unsigned  enable_5 : 1;
		unsigned  enable_6 : 1;
		unsigned master_enable : 1;
		unsigned irq_0 : 1;
		unsigned irq_1 : 1;
		unsigned irq_2 : 1;
		unsigned irq_3 : 1;
		unsigned irq_4 : 1;
		unsigned irq_5 : 1;
		unsigned irq_6 : 1;
		unsigned master_irq : 1;
	};
};

/* GPU */

union t_gpustat
{
	u32 full;
	struct
	{
		unsigned			 tpagex : 4;
		unsigned	 		 tpagey : 1;
		unsigned	   transparenñy : 2;
		unsigned	    tpage_color : 2;
		unsigned				dither : 1;
		unsigned	   draw_to_display : 1;
		unsigned			apply_mask : 1;
		unsigned	 no_draw_over_mask : 1;
		unsigned                       : 3;
		unsigned				width1 : 1;
		unsigned				width0 : 2;
		unsigned				height : 1;
		unsigned			video_mode : 1;
		unsigned			 use_24bpp : 1;
		unsigned		   interlacing : 1;
		unsigned		display_enable : 1;
		unsigned			       irq : 1;
		unsigned				   dma : 1;
		unsigned		 ready_for_cmd : 1;
		unsigned		ready_for_vram : 1;
		unsigned		 ready_for_dma : 1;
		unsigned		  dma_directon : 2;
		unsigned	   draw_even_lines : 1;
	};
};

union t_tpage
{
	u16 full;
	struct
	{
		unsigned tpage_x : 4;
		unsigned tpage_y : 1;
		unsigned transp  : 2;
		unsigned tpage_c : 2;
		unsigned         : 2;
		unsigned tex_dis : 1;
		unsigned         : 4;
	};
};

union t_clut
{
	u16 full;
	struct
	{
		unsigned x_base : 6;
		unsigned y_base : 9;
		unsigned : 1;
	};
};

/* CDROM */

union t_cdindex
{
	u8 full;
	struct
	{
		unsigned port : 2;
		unsigned xa_fifo_empty : 1; // 0 = empty
		unsigned param_fifo_empty : 1; // 1 = empty
		unsigned param_fifo_full : 1;  // 0 = full
		unsigned response_fifo_empty : 1; // 0 = empty
		unsigned data_fifo_empty : 1; // 0 = empty
		unsigned busy : 1; // 1 = bisy
	};
};

union t_cdintenable
{

};

union t_cdintflag
{
	u8 full;
	struct
	{
		unsigned int_type : 2;
		unsigned : 6;
	};
};

/* GAMEPAD */

union t_joystat
{

};