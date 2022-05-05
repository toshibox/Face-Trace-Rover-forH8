/* ------------------------------------------------------------------------ */
/*  HOS-V4  kernel configuration                                            */
/*    kernel object create and initialize                                   */
/*                                                                          */
/* ------------------------------------------------------------------------ */


#include "kernel.h"
#include "kernel_id.h"

#include "main.h"
#include "ostimer.h"



/* ------------------------------------------ */
/*                 idle stack                 */
/* ------------------------------------------ */

VP         mknl_idl_stkblk[((128) + sizeof(VP) - 1) / sizeof(VP)];	/* idle stack block*/
const VP   mknl_idl_stk   = (VP)(mknl_idl_stkblk);	/* idle stack */
const SIZE mknl_idl_stksz = (SIZE)sizeof(mknl_idl_stkblk);	/* idle stack size */



/* ------------------------------------------ */
/*           create ready queue               */
/* ------------------------------------------ */

T_MKNL_QUE mknl_rdq_tbl[8];
const INT  mknl_rdq_cnt = 8;



/* ------------------------------------------ */
/*               set time tic                 */
/* ------------------------------------------ */

const UW kernel_tic_deno = TIC_DENO;
const UW kernel_tic_div  = TIC_NUME / TIC_DENO;
const UW kernel_tic_mod  = TIC_NUME % TIC_DENO;



/* ------------------------------------------ */
/*          create task objects               */
/* ------------------------------------------ */

/* stack area */
static VP kernel_tsk1_stk[((128) + sizeof(VP) - 1) / sizeof(VP)];

/* task control block for rom area */
const T_KERNEL_TCB_ROM kernel_tcb_rom[1] =
	{
		{(ATR)(TA_HLNG|TA_ACT), (VP_INT)(0), (FP)(RxDataCheckTask), (PRI)(2), (SIZE)(128), (VP)kernel_tsk1_stk},
	};

/* task control block for ram area */
T_KERNEL_TCB_RAM kernel_tcb_ram[1];

/* task control block table */
T_KERNEL_TCB_RAM *kernel_tcb_ram_tbl[8] =
	{
		&kernel_tcb_ram[0],
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
	};

/* task control block count */
const INT kernel_tcb_cnt = 8;



/* ------------------------------------------ */
/*         create semaphore objects           */
/* ------------------------------------------ */

/* semaphore control block count */
const INT kernel_semcb_cnt = 0;



/* ------------------------------------------ */
/*        create event flag objects           */
/* ------------------------------------------ */

/* event flag control block for rom area */
const T_KERNEL_FLGCB_ROM kernel_flgcb_rom[5] =
	{
		{(ATR)((TA_TPRI | TA_WSGL | TA_CLR)), (FLGPTN)(0)},
		{(ATR)((TA_TPRI | TA_WSGL | TA_CLR)), (FLGPTN)(0)},
		{(ATR)((TA_TPRI | TA_WSGL | TA_CLR)), (FLGPTN)(0)},
		{(ATR)((TA_TPRI | TA_WSGL | TA_CLR)), (FLGPTN)(0)},
		{(ATR)((TA_TPRI | TA_WSGL | TA_CLR)), (FLGPTN)(0)},
	};

/* event flag control block for ram area */
T_KERNEL_FLGCB_RAM kernel_flgcb_ram[5];

/* event flag control block table */
T_KERNEL_FLGCB_RAM *kernel_flgcb_ram_tbl[5] =
	{
		&kernel_flgcb_ram[0],
		&kernel_flgcb_ram[1],
		&kernel_flgcb_ram[2],
		&kernel_flgcb_ram[3],
		&kernel_flgcb_ram[4],
	};

/* event flag control block count */
const INT kernel_flgcb_cnt = 5;



/* ------------------------------------------ */
/*        create data queue objects           */
/* ------------------------------------------ */

/* data que area */
static VP_INT kernel_dtq1_dtq[4];
static VP_INT kernel_dtq2_dtq[1];

/* data queue control block for rom area */
const T_KERNEL_DTQCB_ROM kernel_dtqcb_rom[2] =
	{
		{(ATR)(TA_TFIFO), (UINT)(4), kernel_dtq1_dtq},
		{(ATR)(TA_TFIFO), (UINT)(1), kernel_dtq2_dtq},
	};

/* data queue control block for ram area */
T_KERNEL_DTQCB_RAM kernel_dtqcb_ram[2];

/* data queue control block table */
T_KERNEL_DTQCB_RAM *kernel_dtqcb_ram_tbl[2] =
	{
		&kernel_dtqcb_ram[0],
		&kernel_dtqcb_ram[1],
	};

/* data queue control block count */
const INT kernel_dtqcb_cnt = 2;



/* ------------------------------------------ */
/*         create mail box objects            */
/* ------------------------------------------ */

/* mail box control block count */
const INT kernel_mbxcb_cnt = 0;



/* ------------------------------------------ */
/*      create message buffer objects         */
/* ------------------------------------------ */

/* mail box control block count */
const INT kernel_mbfcb_cnt = 0;



/* ------------------------------------------ */
/*   create fixed size memory-pool objects    */
/* ------------------------------------------ */

/* fixed size memory-pool control block count */
const INT kernel_mpfcb_cnt = 0;



/* ------------------------------------------ */
/*  create variable size memory-pool objects  */
/* ------------------------------------------ */

/* variable size memory-pool control block count */
const INT kernel_mplcb_cnt = 0;



/* ------------------------------------------ */
/*       create cyclic handler objects        */
/* ------------------------------------------ */

/* cyclic handler control block count */
const INT kernel_cyccb_cnt = 0;



/* ------------------------------------------ */
/*       create alarm handler objects         */
/* ------------------------------------------ */

/* alarm handler control block count */
const INT kernel_almcb_cnt = 0;



/* ------------------------------------------ */
/*        interrupt control objects           */
/* ------------------------------------------ */

/* interrupt control */
T_KERNEL_INTCB kernel_intcb_tbl[33];		/* interrupt control block table */
const INT      kernel_intcb_cnt = 33;		/* interrupt control block count */
const INTNO    kernel_min_intno = 0;		/* minimum intrrupt number */

/* interrupt service routine control */
const INT      kernel_isrcb_cnt = 0;		/* ISR control block count */



/* ------------------------------------------ */
/*    CPU exception handler control objects   */
/* ------------------------------------------ */

/* interrupt control */
T_KERNEL_EXCCB kernel_exccb_tbl[1];		/* CPU exception handler control block table */
const INT      kernel_exccb_cnt = 1;		/* CPU exception handler control block count */
const EXCNO    kernel_min_excno = 0;		/* minimum CPU exception handler number */



/* ------------------------------------------ */
/*          initialize functions              */
/* ------------------------------------------ */

/* object initialize */
void kernel_cfg_init(void)
{
	int i;
	
	
	/* initialize task control block */
	for ( i = 0; i < 1; i++ )
	{
		kernel_tcb_ram[i].tcb_rom = &kernel_tcb_rom[i];
	}
	
	
	/* initialize event flag control block */
	for ( i = 0; i < 5; i++ )
	{
		kernel_flgcb_ram[i].flgcb_rom = &kernel_flgcb_rom[i];
	}
	
	
	/* initialize data queue control block */
	for ( i = 0; i < 2; i++ )
	{
		kernel_dtqcb_ram[i].dtqcb_rom = &kernel_dtqcb_rom[i];
	}

	/* initialize interrupt table */
	kernel_intcb_tbl[29].exinf = (VP_INT)(0);
	kernel_intcb_tbl[29].isr   = (FP)(Ostimer_interruptHandler);
	kernel_intcb_tbl[23].exinf = (VP_INT)(0);
	kernel_intcb_tbl[23].isr   = (FP)(INT_SCI3_interruptHandler);
	kernel_intcb_tbl[32].exinf = (VP_INT)(0);
	kernel_intcb_tbl[32].isr   = (FP)(INT_SCI3_2_interruptHandler);
}

/* start up */
void kernel_cfg_start(void)
{
	kernel_ini_tsk();		/* initialize task */
	kernel_ini_flg();		/* initialize event flag */
	kernel_ini_dtq();		/* initialize data queue */

	/* call initialize routine*/
	((FP)(Main_init))((VP_INT)(0));
	((FP)(Ostimer_init))((VP_INT)(0));
}


/* ------------------------------------------------------------------------ */
/*  End of file                                                             */
/* ------------------------------------------------------------------------ */
