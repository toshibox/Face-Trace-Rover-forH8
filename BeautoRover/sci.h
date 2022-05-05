#ifndef SCI_H
#define	SCI_H
//-----------------------------------------------------------------------------
//��M�f�[�^
//-----------------------------------------------------------------------------
//�L�����N�^�ؑ֗p
#define RADICON_CHARA_COMMAND		(0x6f)//o
#define AUTOCHAICE_CHARA_COMMAND	(0x6d)//m

//
#define FORWARD_MODE_COMMAND		(0x72)//r
#define TURN_RIGHT_MODE_COMMAND		(0x66)//f
#define TURN_LEFT_MODE_COMMAND		(0x64)//d
#define BACKARD_MODE_COMMAND		(0x63)//c
#define STOP_MODE_COMMAND			(0x73)//s
//-----------------------------------------------------------------------------
//  �t���O��`(flag definition)
//-----------------------------------------------------------------------------
#define D_FLG_RxDATA_DONE (0x0001)//��M
#define D_FLG_RxDATA_NONE (0x0000)//��M���ĂȂ��g�p���Ȃ�

//-----------------------------------------------------------------------------
//  �^��`(Type definition)
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
//  �v���g�^�C�v�錾(Prototype declaration)
//-----------------------------------------------------------------------------
//�V���A���ʐM********************************************************************/
#define SCI_RB_SIZE 60	//��M�o�b�t�@
#define SCI_TX_SIZE 128	//���M�o�b�t�@

//���M�Ɋ��荞�݂��g�p���Ȃ��ꍇ�A�R�����g�A�E�g���邱�ƁB
//�킩��Ȃ��ꍇ�A���̂܂܂ł�OK�ł��B
//#define SCI_TX_INTERRUPT

//�{�[���[�g vs-wrc003lv 12MHz
enum brd{
CBR_38400 = 9,
CBR_31250 = 11,
CBR_19200 = 19,
CBR_14400 = 25,
CBR_9600 = 38,
CBR_4800 = 77,
CBR_2400 = 155,
};

/*�p���e�B*/
enum Parity{
even,
odd,
non
};

/*
������
�V���A���ʐM���g�p����O�ɏ��������K�v�B
�����F�@baudrate�@�F�@�{�[���[�g�̎w��i��L�񋓎q����w��j
		parity�@�F�@�p���e�B�̐ݒ�i��L�񋓎q����w��j
		stop�@�F�@�X�g�b�v�r�b�g���̐ݒ�i0�`2�j
�߂�l�F����
*/
void InitSci3(BYTE baudrate,BYTE parity ,BYTE stop);

/*
1�o�C�g��M
�V���A���ʐM��1�o�C�g��M����֐��B
�i���ۂɂ̓o�b�t�@����1�o�C�g�̂ݎ����Ă���j
�����F�@��M�f�[�^������ϐ��̃|�C���^
�߂�l�F	0�F�o�b�t�@����
			1�F�o�b�t�@�Ƀf�[�^�L��		
*/
BYTE SciByteRx(BYTE *data);

/*
1�o�C�g���M
�V���A���ʐM��1�o�C�g���M����֐��B
�����F�@���M�f�[�^
�߂�l�F	0�F�o�b�t�@���������ς��A���M�G���[
			1�F���M����	
*/
BYTE SciByteTx(BYTE data);

/*
�����񑗐M
������or�o�C�g�f�[�^�̔z��𑗐M����֐��B
�����F�@*str�F���M���镶����̃|�C���^
		dataleng�F���M���镶����
�߂�l�F	0�F�o�b�t�@���������ς��A���M�G���[
			1�F���M����	
*/
BYTE SciStrTx(BYTE *str,UUINT dataleng);



//�V���A���ʐM SCI_2********************************************************************/
#define SCI2_RB_SIZE 20	//��M�o�b�t�@

//������
void InitSci3_2(BYTE baudrate,BYTE parity ,BYTE stop);
//���荞�ݗp�֐�
BYTE intsci3_2();
//1�o�C�g��M
BYTE Sci_2ByteRx(BYTE *data);
//1�o�C�g���M
void Sci_2ByteTx(BYTE data);
//�����񑗐M
void Sci_2StrTx(BYTE *str,UUINT dataleng);


//���[�U�g�p�s��
void intencoder();
BYTE intsci3();
BYTE intsci3_2();

//-----------------------------------------------------------------------------
#endif	// SCI_H