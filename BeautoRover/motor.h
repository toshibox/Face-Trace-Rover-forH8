#ifndef MOTOR_H
#define	MOTOR_H
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  �t���O��`(flag definition)
//-----------------------------------------------------------------------------
//���W�R�����[�h
#define D_FLG_MOTOR_FORWARD     (0x0001)
#define D_FLG_MOTOR_TURN_RIGHT	(0x0002)
#define D_FLG_MOTOR_TURN_LEFT	(0x0004)
#define D_FLG_MOTOR_BACKARD		(0x0008)
#define D_FLG_MOTOR_STOP		(0x0010)

#define D_FLG_MOTOR_ALLPTN		(D_FLG_MOTOR_FORWARD | D_FLG_MOTOR_TURN_RIGHT | D_FLG_MOTOR_TURN_LEFT | D_FLG_MOTOR_BACKARD | D_FLG_MOTOR_STOP)

//-----------------------------------------------------------------------------
//  �^��`(Type definition)
//-----------------------------------------------------------------------------
//���W�R�����[�h
typedef enum
{
	E_MOTOR_FORWARD = 0,		//	�O��
	E_MOTOR_BACKARD,			//	���
	E_MOTOR_TURN_RIGHT,			//	�E��]
	E_MOTOR_TURN_LEFT,			//	����]
} E_MOTOR_DIR;

//�ǔ����[�h
typedef signed char M_SBYTE;
typedef unsigned char M_UBYTE;
typedef unsigned char M_BYTE;
typedef signed short M_SWORD;
typedef unsigned short M_UWORD;
typedef signed int M_SINT;
typedef unsigned int M_UINT;
typedef signed long M_SDWORD;
typedef unsigned long M_UDWORD;
//-----------------------------------------------------------------------------
//  �v���g�^�C�v�錾(Prototype declaration)
//-----------------------------------------------------------------------------
//���W�R�����[�h
void Motor_init(void);
int Motor_runStart(E_MOTOR_DIR dir, unsigned char duty);
int Motor_runOnOff(unsigned short brightness, unsigned char duty);
int Motor_runPcon(unsigned short brightness);
int Motor_runStop(void);


//�ǔ����[�h
void InitMtrPwm(void);
void Mtr_Run(M_BYTE mt1,M_BYTE mt2,M_BYTE mt3,M_BYTE mt4);
void Mtr_Run_lv(int mt1,int mt2,int mt3,int mt4,int mt5,int mt6);
//-----------------------------------------------------------------------------
#endif	// MOTOR_H