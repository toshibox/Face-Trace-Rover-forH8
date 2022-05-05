#ifndef MAIN_H
#define MAIN_H
//------------------------------------------------


//------------------------------------------------
//  �t���O��`(flag definition)
//------------------------------------------------
#define D_FLG_WAIT					(0x0001)    // �ҋ@
#define D_FLG_RADICON_SELECTED		(0x0002)    // ���W�R��
#define D_FLG_RADICON_TMP			(0x0004)    // ���W�R�����I��
#define D_FLG_AUTOTARCE_SELECTED	(0x0008)    // �ǔ�
#define D_FLG_AUTOTRACE_TMP			(0x0010)	// �ǔ����I��


#define RECEIVED_DATASIZE 8
#define RECEIVED_BUFFERSIZE 16

//------------------------------------------------
//  �^��`(Type definition)
//------------------------------------------------
//volatile BYTE sci2_rxrb[SCI2_RB_SIZE];
//volatile BYTE rb_rd_num2;
//volatile BYTE rb_wr_num2;

//------------------------------------------------
//  �v���g�^�C�v�錾(Prototype declaration)
//------------------------------------------------
//void MAIN(VP_INT exinf);
void Main_init(VP_INT exinf);
//void LEDTask(VP_INT exinf);
//void RunModeCtlTask(VP_INT exinf);
//void SpeakerTask(VP_INT exinf);
//void CYC1(VP_INT exinf);
//void ALM1(VP_INT exinf);
void INT_SCI3_interruptHandler(VP_INT exinf);
void INT_SCI3_2_interruptHandler(VP_INT exinf);
void RxDataCheckTask(VP_INT exinf);
//void MotorTask(VP_INT exinf);
//void AutoModeTsk(VP_INT exinf);
//------------------------------------------------





#endif // MAIN_H
