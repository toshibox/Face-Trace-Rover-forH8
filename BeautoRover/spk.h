#ifndef SPK_H
#define	SPK_H
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  �t���O��`(flag definition)
//-----------------------------------------------------------------------------
#define D_FLG_SPEAKER_CHARUMERA (0x0001)
#define D_FLG_SPEAKER_BACK		(0x0002)
#define D_FLG_SPEAKER_OFF		(0x0004)

#define D_FLG_SPEAKER_ALLPTN ( D_FLG_SPEAKER_CHARUMERA | D_FLG_SPEAKER_BACK | D_FLG_SPEAKER_OFF )

//-----------------------------------------------------------------------------
//  �^��`(Type definition)
//-----------------------------------------------------------------------------
typedef enum {
	E_SPK_SCALE_DO	= 0,	//	�h
	E_SPK_SCALE_RE,			//	��
	E_SPK_SCALE_MI,			//	�~
	E_SPK_SCALE_FA,			//	�t�@
	E_SPK_SCALE_SO,			//	�\
	E_SPK_SCALE_RA,			//	��
	E_SPK_SCALE_SI,			//	�V
	E_SPK_SCALE_DO2,		//	�h

	//----------------
	E_SPK_SCALE_END,		//	�I�[���ʗp�̔ԕ�
} E_SPK_SCALE;

//-----------------------------------------------------------------------------
//  �v���g�^�C�v�錾(Prototype declaration)
//-----------------------------------------------------------------------------
void Spk_init(void);
int Spk_start(E_SPK_SCALE scale);
int Spk_stop(void);

//-----------------------------------------------------------------------------
#endif	// SPK_H