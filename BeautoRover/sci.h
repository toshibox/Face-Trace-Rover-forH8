#ifndef SCI_H
#define	SCI_H
//-----------------------------------------------------------------------------
//受信データ
//-----------------------------------------------------------------------------
//キャラクタ切替用
#define RADICON_CHARA_COMMAND		(0x6f)//o
#define AUTOCHAICE_CHARA_COMMAND	(0x6d)//m

//
#define FORWARD_MODE_COMMAND		(0x72)//r
#define TURN_RIGHT_MODE_COMMAND		(0x66)//f
#define TURN_LEFT_MODE_COMMAND		(0x64)//d
#define BACKARD_MODE_COMMAND		(0x63)//c
#define STOP_MODE_COMMAND			(0x73)//s
//-----------------------------------------------------------------------------
//  フラグ定義(flag definition)
//-----------------------------------------------------------------------------
#define D_FLG_RxDATA_DONE (0x0001)//受信
#define D_FLG_RxDATA_NONE (0x0000)//受信してない使用しない

//-----------------------------------------------------------------------------
//  型定義(Type definition)
//-----------------------------------------------------------------------------
typedef signed char SBYTE;
typedef unsigned char UBYTE;
typedef unsigned char BYTE;
typedef signed short SWORD;
typedef unsigned short UWORD;
typedef signed int SINT;
typedef unsigned int UUINT;
typedef signed long SDWORD;
typedef unsigned long UDWORD;

//-----------------------------------------------------------------------------
//  プロトタイプ宣言(Prototype declaration)
//-----------------------------------------------------------------------------
//シリアル通信********************************************************************/
#define SCI_RB_SIZE 60	//受信バッファ
#define SCI_TX_SIZE 128	//送信バッファ

//送信に割り込みを使用しない場合、コメントアウトすること。
//わからない場合、そのままでもOKです。
//#define SCI_TX_INTERRUPT

//ボーレート vs-wrc003lv 12MHz
enum brd{
CBR_38400 = 9,
CBR_31250 = 11,
CBR_19200 = 19,
CBR_14400 = 25,
CBR_9600 = 38,
CBR_4800 = 77,
CBR_2400 = 155,
};

/*パリティ*/
enum Parity{
even,
odd,
non
};

/*
初期化
シリアル通信を使用する前に初期化が必要。
引数：　baudrate　：　ボーレートの指定（上記列挙子から指定）
		parity　：　パリティの設定（上記列挙子から指定）
		stop　：　ストップビット長の設定（0～2）
戻り値：無し
*/
void InitSci3(BYTE baudrate,BYTE parity ,BYTE stop);

/*
1バイト受信
シリアル通信で1バイト受信する関数。
（実際にはバッファから1バイトのみ持ってくる）
引数：　受信データを入れる変数のポインタ
戻り値：	0：バッファが空
			1：バッファにデータ有り		
*/
BYTE SciByteRx(BYTE *data);

/*
1バイト送信
シリアル通信で1バイト送信する関数。
引数：　送信データ
戻り値：	0：バッファががいっぱい、送信エラー
			1：送信完了	
*/
BYTE SciByteTx(BYTE data);

/*
文字列送信
文字列orバイトデータの配列を送信する関数。
引数：　*str：送信する文字列のポインタ
		dataleng：送信する文字数
戻り値：	0：バッファががいっぱい、送信エラー
			1：送信完了	
*/
BYTE SciStrTx(BYTE *str,UUINT dataleng);



//シリアル通信 SCI_2********************************************************************/
#define SCI2_RB_SIZE 20	//受信バッファ

//初期化
void InitSci3_2(BYTE baudrate,BYTE parity ,BYTE stop);
//割り込み用関数
BYTE intsci3_2();
//1バイト受信
BYTE Sci_2ByteRx(BYTE *data);
//1バイト送信
void Sci_2ByteTx(BYTE data);
//文字列送信
void Sci_2StrTx(BYTE *str,UUINT dataleng);


//ユーザ使用不可
void intencoder();
BYTE intsci3();
BYTE intsci3_2();

//-----------------------------------------------------------------------------
#endif	// SCI_H