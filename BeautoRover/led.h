#ifndef LED_H
#define	LED_H
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  フラグ定義(flg definition)
//-----------------------------------------------------------------------------
#define		D_LED_KIND_ORANGE		(0)			//	オレンジLED
#define		D_LED_KIND_GREEN		(1)			//	グリーンLED

#define		D_LED_LIGHT_OFF			(0)			//	消灯
#define		D_LED_LIGHT_ON			(1)			//	点灯


#define D_FLG_LED_WAIT					(0x0001)    // 待機
#define D_FLG_LED_RADICON_TMP			(0x0002)    // ラジコン仮選択
#define D_FLG_LED_RADICON_SELECTED		(0x0004)    // ラジコン
#define D_FLG_LED_AUTOTRACE_TMP			(0x0008)    // 追尾仮選択
#define D_FLG_LED_AUTOTARCE_SELECTED	(0x0010)	// 追尾

//  全イベントパターン
#define D_FLG_LED_ALLPTN ( D_FLG_LED_WAIT | D_FLG_LED_RADICON_TMP | D_FLG_LED_RADICON_SELECTED | D_FLG_LED_AUTOTRACE_TMP | D_FLG_LED_AUTOTARCE_SELECTED )
//-----------------------------------------------------------------------------
//  型定義(Type definition)
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//-----------------------------------------------------------------------------
void Led_init(void);
int Led_setLight(int kind, int onoff);

//-----------------------------------------------------------------------------
#endif	// LED_H