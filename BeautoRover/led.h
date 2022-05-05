#ifndef LED_H
#define	LED_H
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  �t���O��`(flg definition)
//-----------------------------------------------------------------------------
#define		D_LED_KIND_ORANGE		(0)			//	�I�����WLED
#define		D_LED_KIND_GREEN		(1)			//	�O���[��LED

#define		D_LED_LIGHT_OFF			(0)			//	����
#define		D_LED_LIGHT_ON			(1)			//	�_��


#define D_FLG_LED_WAIT					(0x0001)    // �ҋ@
#define D_FLG_LED_RADICON_TMP			(0x0002)    // ���W�R�����I��
#define D_FLG_LED_RADICON_SELECTED		(0x0004)    // ���W�R��
#define D_FLG_LED_AUTOTRACE_TMP			(0x0008)    // �ǔ����I��
#define D_FLG_LED_AUTOTARCE_SELECTED	(0x0010)	// �ǔ�

//  �S�C�x���g�p�^�[��
#define D_FLG_LED_ALLPTN ( D_FLG_LED_WAIT | D_FLG_LED_RADICON_TMP | D_FLG_LED_RADICON_SELECTED | D_FLG_LED_AUTOTRACE_TMP | D_FLG_LED_AUTOTARCE_SELECTED )
//-----------------------------------------------------------------------------
//  �^��`(Type definition)
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  �v���g�^�C�v�錾(Prototype declaration)
//-----------------------------------------------------------------------------
void Led_init(void);
int Led_setLight(int kind, int onoff);

//-----------------------------------------------------------------------------
#endif	// LED_H