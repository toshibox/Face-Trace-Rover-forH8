#include <stdio.h>

#include "kernel.h"
#include "iodefine.h"
#include "kernel_id.h"
#include "main.h"

#include "led.h"
#include "lsen.h"
#include "sci.h"
#include "spk.h"
#include "motor.h"
#include "runmode_control.h"

//  �e�C�x���g��`
//------------------------------------------------
//  �t���O��`(flag definition)
//------------------------------------------------
#define D_FLG_WAIT					(0x0001)    // �ҋ@
#define D_FLG_RADICON_SELECTED		(0x0002)    // ���W�R��
#define D_FLG_RADICON_TMP			(0x0004)    // ���W�R�����I��
#define D_FLG_AUTOTARCE_SELECTED	(0x0008)    // �ǔ�
#define D_FLG_AUTOTRACE_TMP			(0x0010)	// �ǔ����I��


//------------------------------------------------
//  �����v���g�^�C�v�錾
//------------------------------------------------
static FLGPTN g_led_flg = 0;
static int g_cycle = 0;
static VP_INT INT_data;


//------------------------------------------------
//  �V�X�e���G���g���[�|�C���g
//------------------------------------------------
int main(void)
{
    // HOS�N��
    sta_hos();

    return 0;
}


//------------------------------------------------
//  �T  �v�F  Sci2���荞�݃^�X�N
//------------------------------------------------
void INT_SCI3_interruptHandler(VP_INT exinf)
{
	intsci3();
}

//------------------------------------------------
//  �T  �v�F  Sci2���荞�݃^�X�N
//------------------------------------------------
void INT_SCI3_2_interruptHandler(VP_INT exinf)
{
	
	intsci3_2();
	
	
	/*
	intsci3_2();
	iset_flg(FLGID_RxDataCheck, D_FLG_RxDATA_DONE);*/
}

volatile BYTE rcv_buff[RECEIVED_BUFFERSIZE];

//------------------------------------------------
//  �T  �v�F  ��M�f�[�^�m�F�^�X�N
//------------------------------------------------
void RxDataCheckTask(VP_INT exinf)
{
	VP_INT recv;
	ER	errorCode ;
	int counter = 0;
	int GLED_flg = 0;
	int OLED_flg = 0;
	BYTE right_motor;
	BYTE left_motor;
	BYTE cs;

	//1�O�Ɠ����M���Ȃ�΃��[�h�`�F���W���Ȃ�
	while(1)
	{
		errorCode = rcv_dtq( DTQID_RADICON, &recv);

		if(OLED_flg == 0)
		{
			
			Led_setLight(D_LED_KIND_ORANGE, D_LED_LIGHT_OFF);
			//dly_tsk(1000);
			OLED_flg = 1;
		}
		else
		{
			Led_setLight(D_LED_KIND_ORANGE, D_LED_LIGHT_ON);
			//dly_tsk(1000);
			OLED_flg = 0;
		}

		rcv_buff[counter*RECEIVED_DATASIZE] = recv & 0xff;
		
		if(recv == 0xff)
		{
			
			Led_setLight(D_LED_KIND_GREEN, D_LED_LIGHT_OFF);
			

			right_motor = rcv_buff[0 * RECEIVED_DATASIZE] ;
			left_motor = rcv_buff[1 * RECEIVED_DATASIZE];
			cs = rcv_buff[2 * RECEIVED_DATASIZE];

			
			
			
			//Sci_2ByteTx((right_motor + left_motor) & 0xff);
			
			if(!((cs + right_motor + left_motor) & 0xff))
			{

				Mtr_Run((char)right_motor,(char)left_motor,0,0);
				//Sci_2ByteTx((right_motor) & 0xff);
				//Sci_2ByteTx((left_motor) & 0xff);
				Sci_2ByteTx(1 & 0xff);
				Sci_2ByteTx(right_motor & 0xff);
				Sci_2ByteTx(left_motor & 0xff);
				Sci_2ByteTx(cs & 0xff);

			}
			else
			{
				//dly_tsk(1000);
				//Sci_2ByteTx((counter) & 0xff);
				Led_setLight(D_LED_KIND_GREEN, D_LED_LIGHT_ON);//�����ڂŕ�����悤��
				Sci_2ByteTx(0 & 0xff);
				Sci_2ByteTx(right_motor & 0xff);
				Sci_2ByteTx(left_motor & 0xff);
				Sci_2ByteTx(cs & 0xff);
				
									
			}
			
			counter = 0;		
			continue;	
		}
		
		counter +=1;
				
		if(counter == (RECEIVED_BUFFERSIZE))
		{
			/*clear*/
			counter = 0;
			
		}
				
	}
	
}
			
			
	

//------------------------------------------------
//  �T  �v�F�V�X�e��������
//------------------------------------------------
void Main_init(VP_INT exinf)
{
	
	
    //---------------------------------------------------
    //  �E�H�b�`�h�b�O�^�C�}�̒�~(�����Ȃ�����)
    //---------------------------------------------------
    WDT.TCSRWD.BYTE = 0x92;
    WDT.TCSRWD.BYTE = 0x92;


    //---------------------------------------------------
    //  PCR�̐ݒ�(PCR�͏������ݐ�p���W�X�^)
    //---------------------------------------------------
    IO.PCR3         = 0x0F;     //  P30,P31,P32,P33���o��(���[�^�[��]��������)
    IO.PCR6         = 0x11;     //  P60,P64���o��
    IO.PCR7         = 0x00;     //  P74�����


    //---------------------------------------------------
    //  ���W���[���̏�����(�e���W���[���̏����������{)
    //---------------------------------------------------
    Led_init();
    Lsen_init();
    InitSci3_2( CBR_19200, non, 1);
    Spk_init();
    Motor_init();
	
	
}

//------------------------------------------------