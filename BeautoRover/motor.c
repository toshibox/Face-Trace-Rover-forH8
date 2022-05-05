#include <string.h>
#include "iodefine.h"
#include "motor.h"
#include <machine.h>

//ラジコンモード用
#define	D_MOTOR_KP					(0.0125)	//	比例ゲイン
#define	D_MOTOR_DUTY_BASE			(10)		//	基本走行デューティ比
#define	D_MOTOR_DUTY_WIDTH			(5)			//	最大duty調整幅
#define	D_MOTOR_TARGET_BRIGHTNESS	(435.0)		//	目標センサ値


M_UINT mtr_pwm_cyc = 0x7000;
//-----------------------------------------------------------------------------
//  概　要：初期化
//  引  数：なし
//  戻り値：なし
//-----------------------------------------------------------------------------
void Motor_init(void)
{
	TZ.TSTR.BIT.STR0	= 0;		// タイマZのch0をカウント停止

	TZ0.TCR.BIT.TPSC	= 0;		//	分周なし
	TZ0.TCR.BIT.CKEG	= 0;		//	立上りエッジカウント
	TZ0.TCR.BIT.CCLR	= 1;		//	GRAでコンペアマッチクリア

	TZ.TPMR.BIT.PWMB0	= 1;		//	右モーターPWMモード
	TZ.TPMR.BIT.PWMC0	= 1;		//	左モーターPWMモード

	//	HighからLowへ切り替わる波形
	TZ.TOCR.BIT.TOB0	= 0;		//	PWM波形はHigh
	TZ.TOCR.BIT.TOC0	= 0;		//	PWM波形はHigh
	TZ0.POCR.BIT.POLB	= 0;		//	ローアクティブ
	TZ0.POCR.BIT.POLC	= 0;		//	ローアクティブ

	TZ.TOER.BIT.EB0		= 0;		//	FTIOB0は出力
	TZ.TOER.BIT.EC0		= 0;		//	FTIOC0は出力

	TZ0.TCNT			= 0;		//	カウンタクリア
	TZ0.GRA				= 12000;	//	1ms周期のPWMとして動作させる
	TZ0.GRB				= 0;		//	仮設定
	TZ0.GRC				= 0;		//	仮設定
}

//-----------------------------------------------------------------------------
//  概　要：走行開始
//  引  数：dir		方向	E_MOTOR_FORWARD		前方
//							E_MOTOR_BACKARD		後方
//  						E_MOTOR_TURN_RIGHT	右回転
//							E_MOTOR_TURN_LEFT	左回転
//			duty	デューティ比(0 - 99)
//  戻り値：0	正常
//			-1	異常
//-----------------------------------------------------------------------------
int Motor_runStart(E_MOTOR_DIR dir, unsigned char duty)
{
	//	デューティ比100%以上は異常扱い
	if (duty >= 100)
	{
		return -1;
	}

	//	走行方向に従ったポート切り替え
	if (dir == E_MOTOR_FORWARD)
	{
		//	右モーター前進
		IO.PDR3.BIT.B0 = 1;
		IO.PDR3.BIT.B1 = 0;

		//	左モーター前進
		IO.PDR3.BIT.B2 = 1;
		IO.PDR3.BIT.B3 = 0;
	}
	else if (dir == E_MOTOR_BACKARD)
	{
		//	右モーター後退
		IO.PDR3.BIT.B0 = 0;
		IO.PDR3.BIT.B1 = 1;

		//	左モーター後退
		IO.PDR3.BIT.B2 = 0;
		IO.PDR3.BIT.B3 = 1;
	}
	else if (dir == E_MOTOR_TURN_RIGHT)
	{
		//	右モーター後退
		IO.PDR3.BIT.B0 = 0;
		IO.PDR3.BIT.B1 = 1;

		//	左モーター前進
		IO.PDR3.BIT.B2 = 1;
		IO.PDR3.BIT.B3 = 0;
	}
	else if (dir == E_MOTOR_TURN_LEFT)
	{
		//	右モーター前進
		IO.PDR3.BIT.B0 = 1;
		IO.PDR3.BIT.B1 = 0;

		//	左モーター後退
		IO.PDR3.BIT.B2 = 0;
		IO.PDR3.BIT.B3 = 1;
	}
	else
	{
		return -1;
	}

	//	GRAの周期レジスタをベースにデューティ比の割合を設定
	TZ0.GRB	= TZ0.GRA * (duty / 100.0);
	TZ0.GRC	= TZ0.GRA * (duty / 100.0);

	TZ.TSTR.BIT.STR0 = 1;		// タイマZのch0をカウント開始してPWM動作開始

	return 0;
}

//-----------------------------------------------------------------------------
//  概　要：ON-OFF制御走行開始
//  引  数：brightness		光センサ値
//			duty			デューティ比(0 - 99)
//  戻り値：0	正常
//			-1	異常
//-----------------------------------------------------------------------------
int Motor_runOnOff(unsigned short brightness, unsigned char duty)
{

	if (brightness >= 512)		//	黒なら左方向へ
	{
		//	右モーター前進
		IO.PDR3.BIT.B0 = 1;
		IO.PDR3.BIT.B1 = 0;

		//	左モーター停止
		IO.PDR3.BIT.B2 = 1;
		IO.PDR3.BIT.B3 = 1;
	}
	else						//	白なら右方向へ
	{
		//	右モーター停止
		IO.PDR3.BIT.B0 = 1;
		IO.PDR3.BIT.B1 = 1;

		//	左モーター前進
		IO.PDR3.BIT.B2 = 1;
		IO.PDR3.BIT.B3 = 0;
	}

	//	GRAの周期レジスタをベースにデューティ比の割合を設定
	TZ0.GRB	= TZ0.GRA * (duty / 100.0);
	TZ0.GRC	= TZ0.GRA * (duty / 100.0);

	TZ.TSTR.BIT.STR0 = 1;		// タイマZのch0をカウント開始してPWM動作開始

	return 0;
}

//-----------------------------------------------------------------------------
//  概　要：P制御走行開始
//  引  数：brightness		光センサ値
//  戻り値：0	正常
//			-1	異常
//-----------------------------------------------------------------------------
int Motor_runPcon(unsigned short brightness)
{
	short dutyDiff;

	//	右モーター前進
	IO.PDR3.BIT.B0 = 1;
	IO.PDR3.BIT.B1 = 0;

	//	左モーター前進
	IO.PDR3.BIT.B2 = 1;
	IO.PDR3.BIT.B3 = 0;

	//	目標に対するduty制御量の算出
	dutyDiff = (D_MOTOR_TARGET_BRIGHTNESS - brightness) * D_MOTOR_KP;

	//	最大調整duty幅内での補正処理
	if (dutyDiff > D_MOTOR_DUTY_WIDTH)
	{
		dutyDiff = D_MOTOR_DUTY_WIDTH;
	}
	else if (dutyDiff < -D_MOTOR_DUTY_WIDTH)
	{
		dutyDiff = -D_MOTOR_DUTY_WIDTH;
	}

	//	GRAの周期レジスタをベースにデューティ比の割合を設定
	TZ0.GRB	= TZ0.GRA * ((D_MOTOR_DUTY_BASE - dutyDiff) / 100.0);	//	右
	TZ0.GRC	= TZ0.GRA * ((D_MOTOR_DUTY_BASE + dutyDiff) / 100.0);	//	左

	TZ.TSTR.BIT.STR0 = 1;	// タイマZのch0をカウント開始してPWM動作開始

	return 0;
}

//-----------------------------------------------------------------------------
//  概　要：走行停止
//  引  数：なし
//  戻り値：0	正常
//			-1	異常
//-----------------------------------------------------------------------------
int Motor_runStop(void)
{
	//	右モーター停止
	IO.PDR3.BIT.B0 = 1;
	IO.PDR3.BIT.B1 = 1;

	//	左モーター停止
	IO.PDR3.BIT.B2 = 1;
	IO.PDR3.BIT.B3 = 1;

	TZ.TSTR.BIT.STR0 = 0;		// タイマZのch0をカウント停止してPWM動作停止

	return 0;
}


//追尾モード用


void InitMtrPwm(void)
{	
	TZ.TSTR.BIT.STR0=0;
	TZ.TSTR.BIT.STR1=0;
	
	TZ0.TCR.BYTE =	0x20;	//GRAでクリア、立上りエッジ、φでカウント
	TZ1.TCR.BYTE =	0x20;	//GRAでクリア、立上りエッジ、φでカウント
	
	TZ.TPMR.BYTE =	0xFF;	//PWM
		
	TZ.TOCR.BYTE =	0x00;	//TZ0：はじめ1出力	,1:1出力
		
	TZ0.POCR.BYTE = 0x00;
	TZ1.POCR.BYTE = 0x00;
	
	TZ.TOER.BYTE =	0x11;	//TZ0：出力許可
		
	TZ.TMDR.BYTE =	0x00;	//通常
	TZ.TFCR.BYTE =	0x00;	//通常
	
	TZ0.TSR.BYTE =	0x00;	//エラーフラグクリア
	TZ1.TSR.BYTE =	0x00;	//エラーフラグクリア

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
		//デューティのセット
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
		
		//TZが止まっていたらスタート
		if(TZ.TSTR.BIT.STR0!=1){
			TZ.TSTR.BIT.STR0=1;		//カウントスタート
		}
		if(TZ.TSTR.BIT.STR1!=1){
			TZ.TSTR.BIT.STR1=1;		//カウントスタート
		}
		// LL :HiZ（out：ZZ）  HL,LH:drive  HH:ブレーキ（out：LL）に変更必要 
		
		IO.PDR3.BYTE = PortOut_3;
		IO.PDR1.BYTE = (PortOut_1 & 0x05)|(IO.PDR1.BYTE & 0xFA);	//P10,12 0x05 0xFA
		IO.PDR5.BYTE = (PortOut_5 & 0x30)|(IO.PDR5.BYTE & 0xCF);	//P54,55 0x30 0xCF
}
