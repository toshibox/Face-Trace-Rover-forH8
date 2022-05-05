#include <string.h>
#include "iodefine.h"
#include "motor.h"
#include <machine.h>

//���W�R�����[�h�p
#define	D_MOTOR_KP					(0.0125)	//	���Q�C��
#define	D_MOTOR_DUTY_BASE			(10)		//	��{���s�f���[�e�B��
#define	D_MOTOR_DUTY_WIDTH			(5)			//	�ő�duty������
#define	D_MOTOR_TARGET_BRIGHTNESS	(435.0)		//	�ڕW�Z���T�l


M_UINT mtr_pwm_cyc = 0x7000;
//-----------------------------------------------------------------------------
//  �T�@�v�F������
//  ��  ���F�Ȃ�
//  �߂�l�F�Ȃ�
//-----------------------------------------------------------------------------
void Motor_init(void)
{
	TZ.TSTR.BIT.STR0	= 0;		// �^�C�}Z��ch0���J�E���g��~

	TZ0.TCR.BIT.TPSC	= 0;		//	�����Ȃ�
	TZ0.TCR.BIT.CKEG	= 0;		//	�����G�b�W�J�E���g
	TZ0.TCR.BIT.CCLR	= 1;		//	GRA�ŃR���y�A�}�b�`�N���A

	TZ.TPMR.BIT.PWMB0	= 1;		//	�E���[�^�[PWM���[�h
	TZ.TPMR.BIT.PWMC0	= 1;		//	�����[�^�[PWM���[�h

	//	High����Low�֐؂�ւ��g�`
	TZ.TOCR.BIT.TOB0	= 0;		//	PWM�g�`��High
	TZ.TOCR.BIT.TOC0	= 0;		//	PWM�g�`��High
	TZ0.POCR.BIT.POLB	= 0;		//	���[�A�N�e�B�u
	TZ0.POCR.BIT.POLC	= 0;		//	���[�A�N�e�B�u

	TZ.TOER.BIT.EB0		= 0;		//	FTIOB0�͏o��
	TZ.TOER.BIT.EC0		= 0;		//	FTIOC0�͏o��

	TZ0.TCNT			= 0;		//	�J�E���^�N���A
	TZ0.GRA				= 12000;	//	1ms������PWM�Ƃ��ē��삳����
	TZ0.GRB				= 0;		//	���ݒ�
	TZ0.GRC				= 0;		//	���ݒ�
}

//-----------------------------------------------------------------------------
//  �T�@�v�F���s�J�n
//  ��  ���Fdir		����	E_MOTOR_FORWARD		�O��
//							E_MOTOR_BACKARD		���
//  						E_MOTOR_TURN_RIGHT	�E��]
//							E_MOTOR_TURN_LEFT	����]
//			duty	�f���[�e�B��(0 - 99)
//  �߂�l�F0	����
//			-1	�ُ�
//-----------------------------------------------------------------------------
int Motor_runStart(E_MOTOR_DIR dir, unsigned char duty)
{
	//	�f���[�e�B��100%�ȏ�ُ͈툵��
	if (duty >= 100)
	{
		return -1;
	}

	//	���s�����ɏ]�����|�[�g�؂�ւ�
	if (dir == E_MOTOR_FORWARD)
	{
		//	�E���[�^�[�O�i
		IO.PDR3.BIT.B0 = 1;
		IO.PDR3.BIT.B1 = 0;

		//	�����[�^�[�O�i
		IO.PDR3.BIT.B2 = 1;
		IO.PDR3.BIT.B3 = 0;
	}
	else if (dir == E_MOTOR_BACKARD)
	{
		//	�E���[�^�[���
		IO.PDR3.BIT.B0 = 0;
		IO.PDR3.BIT.B1 = 1;

		//	�����[�^�[���
		IO.PDR3.BIT.B2 = 0;
		IO.PDR3.BIT.B3 = 1;
	}
	else if (dir == E_MOTOR_TURN_RIGHT)
	{
		//	�E���[�^�[���
		IO.PDR3.BIT.B0 = 0;
		IO.PDR3.BIT.B1 = 1;

		//	�����[�^�[�O�i
		IO.PDR3.BIT.B2 = 1;
		IO.PDR3.BIT.B3 = 0;
	}
	else if (dir == E_MOTOR_TURN_LEFT)
	{
		//	�E���[�^�[�O�i
		IO.PDR3.BIT.B0 = 1;
		IO.PDR3.BIT.B1 = 0;

		//	�����[�^�[���
		IO.PDR3.BIT.B2 = 0;
		IO.PDR3.BIT.B3 = 1;
	}
	else
	{
		return -1;
	}

	//	GRA�̎������W�X�^���x�[�X�Ƀf���[�e�B��̊�����ݒ�
	TZ0.GRB	= TZ0.GRA * (duty / 100.0);
	TZ0.GRC	= TZ0.GRA * (duty / 100.0);

	TZ.TSTR.BIT.STR0 = 1;		// �^�C�}Z��ch0���J�E���g�J�n����PWM����J�n

	return 0;
}

//-----------------------------------------------------------------------------
//  �T�@�v�FON-OFF���䑖�s�J�n
//  ��  ���Fbrightness		���Z���T�l
//			duty			�f���[�e�B��(0 - 99)
//  �߂�l�F0	����
//			-1	�ُ�
//-----------------------------------------------------------------------------
int Motor_runOnOff(unsigned short brightness, unsigned char duty)
{

	if (brightness >= 512)		//	���Ȃ獶������
	{
		//	�E���[�^�[�O�i
		IO.PDR3.BIT.B0 = 1;
		IO.PDR3.BIT.B1 = 0;

		//	�����[�^�[��~
		IO.PDR3.BIT.B2 = 1;
		IO.PDR3.BIT.B3 = 1;
	}
	else						//	���Ȃ�E������
	{
		//	�E���[�^�[��~
		IO.PDR3.BIT.B0 = 1;
		IO.PDR3.BIT.B1 = 1;

		//	�����[�^�[�O�i
		IO.PDR3.BIT.B2 = 1;
		IO.PDR3.BIT.B3 = 0;
	}

	//	GRA�̎������W�X�^���x�[�X�Ƀf���[�e�B��̊�����ݒ�
	TZ0.GRB	= TZ0.GRA * (duty / 100.0);
	TZ0.GRC	= TZ0.GRA * (duty / 100.0);

	TZ.TSTR.BIT.STR0 = 1;		// �^�C�}Z��ch0���J�E���g�J�n����PWM����J�n

	return 0;
}

//-----------------------------------------------------------------------------
//  �T�@�v�FP���䑖�s�J�n
//  ��  ���Fbrightness		���Z���T�l
//  �߂�l�F0	����
//			-1	�ُ�
//-----------------------------------------------------------------------------
int Motor_runPcon(unsigned short brightness)
{
	short dutyDiff;

	//	�E���[�^�[�O�i
	IO.PDR3.BIT.B0 = 1;
	IO.PDR3.BIT.B1 = 0;

	//	�����[�^�[�O�i
	IO.PDR3.BIT.B2 = 1;
	IO.PDR3.BIT.B3 = 0;

	//	�ڕW�ɑ΂���duty����ʂ̎Z�o
	dutyDiff = (D_MOTOR_TARGET_BRIGHTNESS - brightness) * D_MOTOR_KP;

	//	�ő咲��duty�����ł̕␳����
	if (dutyDiff > D_MOTOR_DUTY_WIDTH)
	{
		dutyDiff = D_MOTOR_DUTY_WIDTH;
	}
	else if (dutyDiff < -D_MOTOR_DUTY_WIDTH)
	{
		dutyDiff = -D_MOTOR_DUTY_WIDTH;
	}

	//	GRA�̎������W�X�^���x�[�X�Ƀf���[�e�B��̊�����ݒ�
	TZ0.GRB	= TZ0.GRA * ((D_MOTOR_DUTY_BASE - dutyDiff) / 100.0);	//	�E
	TZ0.GRC	= TZ0.GRA * ((D_MOTOR_DUTY_BASE + dutyDiff) / 100.0);	//	��

	TZ.TSTR.BIT.STR0 = 1;	// �^�C�}Z��ch0���J�E���g�J�n����PWM����J�n

	return 0;
}

//-----------------------------------------------------------------------------
//  �T�@�v�F���s��~
//  ��  ���F�Ȃ�
//  �߂�l�F0	����
//			-1	�ُ�
//-----------------------------------------------------------------------------
int Motor_runStop(void)
{
	//	�E���[�^�[��~
	IO.PDR3.BIT.B0 = 1;
	IO.PDR3.BIT.B1 = 1;

	//	�����[�^�[��~
	IO.PDR3.BIT.B2 = 1;
	IO.PDR3.BIT.B3 = 1;

	TZ.TSTR.BIT.STR0 = 0;		// �^�C�}Z��ch0���J�E���g��~����PWM�����~

	return 0;
}


//�ǔ����[�h�p


void InitMtrPwm(void)
{	
	TZ.TSTR.BIT.STR0=0;
	TZ.TSTR.BIT.STR1=0;
	
	TZ0.TCR.BYTE =	0x20;	//GRA�ŃN���A�A�����G�b�W�A�ӂŃJ�E���g
	TZ1.TCR.BYTE =	0x20;	//GRA�ŃN���A�A�����G�b�W�A�ӂŃJ�E���g
	
	TZ.TPMR.BYTE =	0xFF;	//PWM
		
	TZ.TOCR.BYTE =	0x00;	//TZ0�F�͂���1�o��	,1:1�o��
		
	TZ0.POCR.BYTE = 0x00;
	TZ1.POCR.BYTE = 0x00;
	
	TZ.TOER.BYTE =	0x11;	//TZ0�F�o�͋���
		
	TZ.TMDR.BYTE =	0x00;	//�ʏ�
	TZ.TFCR.BYTE =	0x00;	//�ʏ�
	
	TZ0.TSR.BYTE =	0x00;	//�G���[�t���O�N���A
	TZ1.TSR.BYTE =	0x00;	//�G���[�t���O�N���A

	TZ0.TCNT=0;
	TZ1.TCNT=0;

	TZ0.GRA=mtr_pwm_cyc;
	TZ1.GRA=mtr_pwm_cyc;
	
	TZ0.GRB = 0;
	TZ0.GRC = 0;
	TZ0.GRD = 0;
	TZ1.GRB = 0;
	TZ1.GRC = 0;
	TZ1.GRD = 0;	
}

void Mtr_Run(M_BYTE mt1,M_BYTE mt2,M_BYTE mt3,M_BYTE mt4){
	if(mt1 == 0x80)
		mt1 = 0;
	if(mt2 == 0x80)
		mt2 = 0;
	if(mt3 == 0x80)
		mt3 = 0;
	if(mt4 == 0x80)
		mt4 = 0;
	Mtr_Run_lv(	(int)mt1*0x00FF,
				(int)mt2*0x00FF,
				(int)mt3*0x00FF,
				(int)mt4*0x00FF,
				0,0);
}

void Mtr_Run_lv(int mt1,int mt2,int mt3,int mt4,int mt5,int mt6)
{
	M_BYTE i,PortOut_3,PortOut_5,PortOut_1;
	M_UINT duty[6];
	PortOut_3 = 0x00;
	PortOut_5 = 0x00;
	PortOut_1 = 0x00;
		//�f���[�e�B�̃Z�b�g
		for(i=0;i<6;i++){
			int MTRtemp,MTRtempB;
			int tempSint;
			switch(i){
				case 0:
					MTRtemp = mt1;
					break;
				case 1:
					MTRtemp = mt2;
					break;
				case 2:
					MTRtemp = mt3;
					break;
				case 3:
					MTRtemp = mt4;
					break;
				case 4:
					MTRtemp = mt5;
					break;
				case 5:
					MTRtemp = mt6;
					break;
			}			
			if(MTRtemp == 0 || MTRtemp == 0x8000){
				tempSint=0;
			}
			else if(MTRtemp > 0){
				tempSint = MTRtemp;
				switch(i){
					case 0: //P31
					case 2:	//P35
						PortOut_3 |= (0x02 << i*2);			
						break;
					case 1:	//P32
					case 3:	//P36
						PortOut_3 |= (0x01 << i*2);	
						break;
					case 4://P12
						PortOut_1 = (0x01 << 2);
						break;
					case 5:	//P54
						PortOut_5 = (0x01 << 4);
						break;
				}			
			}
			else if(MTRtemp < 0){			//P31,32,35,36,12,54
				tempSint = -MTRtemp;
				switch(i){
					case 0:	//P30
					case 2:	//P34
						PortOut_3 |= (0x01 << i*2);	
						break;
					case 1:	//P33
					case 3: //P37
						PortOut_3 |= (0x02 << i*2);
						break;	
					case 4://P10
						PortOut_1 = (0x01 << 0);
						break;
					case 5:	//P55
						PortOut_5 = (0x01 << 5);
						break;
				}	
			}
			duty[i]=(M_UINT)(((unsigned long)tempSint*(unsigned long)mtr_pwm_cyc)/0x8000);
		}
		IO.PDR3.BYTE = 0x00;
		
		TZ0.GRB = duty[0];
		TZ0.GRC = duty[1];
		TZ0.GRD = duty[2];
		TZ1.GRB = duty[3];
		TZ1.GRC = duty[4];
		TZ1.GRD = duty[5];
		
		//TZ���~�܂��Ă�����X�^�[�g
		if(TZ.TSTR.BIT.STR0!=1){
			TZ.TSTR.BIT.STR0=1;		//�J�E���g�X�^�[�g
		}
		if(TZ.TSTR.BIT.STR1!=1){
			TZ.TSTR.BIT.STR1=1;		//�J�E���g�X�^�[�g
		}
		// LL :HiZ�iout�FZZ�j  HL,LH:drive  HH:�u���[�L�iout�FLL�j�ɕύX�K�v 
		
		IO.PDR3.BYTE = PortOut_3;
		IO.PDR1.BYTE = (PortOut_1 & 0x05)|(IO.PDR1.BYTE & 0xFA);	//P10,12 0x05 0xFA
		IO.PDR5.BYTE = (PortOut_5 & 0x30)|(IO.PDR5.BYTE & 0xCF);	//P54,55 0x30 0xCF
}
