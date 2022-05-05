/* ------------------------------------------------------------------------ */
/*  HOS-V4  kernel configuration                                            */
/*    kernel object ID definition                                           */
/*                                                                          */
/* ------------------------------------------------------------------------ */


#ifndef __HOS_V4__kernel_cfg_h__
#define __HOS_V4__kernel_cfg_h__



/* time tic */
#define TIC_NUME		1
#define TIC_DENO		1


/* task ID definetion */
#define TSKID_RxDataCheck		1

#define TMAX_TSKID		8


/* event flag ID definetion */
#define FLGID_RunModeCtl		1
#define FLGID_LED		2
#define FLGID_Speaker		3
#define FLGID_RxDataCheck		4
#define FLGID_Motor		5

#define TMAX_FLGID		5


/* data queue ID definetion */
#define DTQID_RADICON		1
#define DTQID_AT		2

#define TMAX_DTQID		2



#endif	/* __HOS_V4__kernel_cfg_h__ */


/* ------------------------------------------------------------------------ */
/*  End of file                                                             */
/* ------------------------------------------------------------------------ */
