#ifndef SPK_H
#define	SPK_H
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  フラグ定義(flag definition)
//-----------------------------------------------------------------------------
#define D_FLG_SPEAKER_CHARUMERA (0x0001)
#define D_FLG_SPEAKER_BACK		(0x0002)
#define D_FLG_SPEAKER_OFF		(0x0004)

#define D_FLG_SPEAKER_ALLPTN ( D_FLG_SPEAKER_CHARUMERA | D_FLG_SPEAKER_BACK | D_FLG_SPEAKER_OFF )

//-----------------------------------------------------------------------------
//  型定義(Type definition)
//-----------------------------------------------------------------------------
typedef enum {
	E_SPK_SCALE_DO	= 0,	//	ド
	E_SPK_SCALE_RE,			//	レ
	E_SPK_SCALE_MI,			//	ミ
	E_SPK_SCALE_FA,			//	ファ
	E_SPK_SCALE_SO,			//	ソ
	E_SPK_SCALE_RA,			//	ラ
	E_SPK_SCALE_SI,			//	シ
	E_SPK_SCALE_DO2,		//	ド

	//----------------
	E_SPK_SCALE_END,		//	終端判別用の番兵
} E_SPK_SCALE;

//-----------------------------------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//-----------------------------------------------------------------------------
void Spk_init(void);
int Spk_start(E_SPK_SCALE scale);
int Spk_stop(void);

//-----------------------------------------------------------------------------
#endif	// SPK_H