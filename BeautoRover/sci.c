#include <stdio.h>
#include <machine.h>
#include "iodefine.h"
#include "sci.h"
#include "led.h"
//#include "main.h"
#include "kernel_id.h"
#include "kernel.h"

//-----------------------------------------------------------------------------
//  �T�@�v�F������
//  ��  ���F�Ȃ�
//  �߂�l�F�Ȃ�
//-----------------------------------------------------------------------------
//SCI ************************************************************************//

#ifdef SCI_TX_INTERRUPT
volatile BYTE sci_txrb[SCI_TX_SIZE]; 
#endif

volatile BYTE sci_rxrb[SCI_RB_SIZE];
BYTE rb_rd_num;
BYTE rb_wr_num;
BYTE tx_rd_num;
BYTE tx_wr_num;

BYTE intsci3(){
	BYTE str[4],temp,next;

	if((SCI3.SSR.BYTE & 0x18) == 0 && SCI3.SSR.BIT.RDRF == 1)
	{          
		next=(rb_wr_num+1)%SCI_RB_SIZE;
		
		if(rb_rd_num!=next){
			SCI3.SSR.BYTE &= 0x87;
			sci_rxrb[rb_wr_num] = SCI3.RDR;			//  �f�[�^���󂯎��data�ɕۑ� 
			rb_wr_num=next;
			return 0;
		}
	}
	else{
		temp = SCI3.RDR;
		SCI3.SSR.BYTE &= 0x87;
	}
	
	
#ifdef SCI_TX_INTERRUPT
	if(SCI3.SSR.BIT.TDRE == 1){	
		if(tx_rd_num != tx_wr_num)	//��M�o�b�t�@�󂶂�Ȃ�������
		{
			SCI3.TDR = sci_txrb[tx_rd_num];
			tx_rd_num = (tx_rd_num + 1) % SCI_TX_SIZE;
			return 1;
			
		}
	}
	if(SCI3.SSR.BIT.TEND == 1){
		SCI3.SCR3.BIT.TIE = 0;
	}
	
#endif
	
    return 1;
}


BYTE SciByteRx(BYTE *data)
{
	if(rb_rd_num!=rb_wr_num)	//��M�o�b�t�@�󂶂�Ȃ�������
	{	
		*data=sci_rxrb[rb_rd_num];
		rb_rd_num=(rb_rd_num+1)%SCI_RB_SIZE;
		return 1;
	}
	else
		return 0;
}

void InitSci3(BYTE baudrate,BYTE parity ,BYTE stop)
{
	UUINT dmy;
	BYTE smr = 0;
	
	switch(parity){
		case even:
			smr = 0x20;
		break;
		case odd:
			smr = 0x30;
		break;
		case non:
		default:
			smr = 0x00;
		break;
	}
	if(stop == 2){
		smr |= 0x08;
	}
	rb_rd_num = 0;
	rb_wr_num = 0;
	
	SCI3.SCR3.BYTE = 0;       //�رٺ��۰�ڼ޽�(SCI�̑��M/��M����ݒ�)
	SCI3.SMR.BYTE = smr;       //�ر�Ӱ��ڼ޽�(SCI�̒ʐM̫�ϯĂ̐ݒ�)
	SCI3.BRR = (BYTE)baudrate;	          //1bps
	
	for(dmy = 280;dmy > 0;dmy--); //Wait 1bit transfer time
	
	SCI3.SCR3.BYTE = 0x70;    //Enable Tx & Rx ,enable Rx interrupt
	IO.PMR1.BIT.TXD=1;
	SCI3.SSR.BYTE &= 0x80;   //Clear receive error flag
	
}

#ifdef SCI_TX_INTERRUPT

BYTE SciByteTx(BYTE data)
{
	BYTE next;
	next=(tx_wr_num + 1)%SCI_TX_SIZE;
		
	if(tx_rd_num != next){	
		sci_txrb[tx_wr_num] = data;			//  �f�[�^���󂯎��data�ɕۑ� 
		tx_wr_num = next;
	}
	else
		return 0;
		
	if(SCI3.SSR.BIT.TEND == 1 && SCI3.SSR.BIT.TDRE == 1){	//���M������Ȃ�������
		SCI3.SCR3.BIT.TIE = 1;
		intsci3();	
	}
	return 1;
	
		
}
BYTE SciStrTx(BYTE *str,UUINT dataleng){
	BYTE next,i,wr_temp;
	wr_temp = tx_wr_num;
	//�o�b�t�@�̋󂫏󋵂��m�F
	for(i = 0;i < dataleng;i++){
		next = (wr_temp + 1)%SCI_TX_SIZE;		
		if(wr_temp != next){
			wr_temp = next;
		}
		else
			return 0;
		
	}
	//���M�f�[�^���o�b�t�@�ɃZ�b�g
	for(i = 0;i < dataleng;i++){
		next = (tx_wr_num + 1)%SCI_TX_SIZE;
		
		//�O�̂��ߍĊm�F
		if(tx_rd_num != next){	
			sci_txrb[tx_wr_num] = str[i];			//  �f�[�^���󂯎��data�ɕۑ� 
			tx_wr_num = next;
		}
		else
			return 0;
	}
	if(SCI3.SSR.BIT.TEND == 1 && SCI3.SSR.BIT.TDRE == 1){	//���M������Ȃ�������
		SCI3.SCR3.BIT.TIE = 1;
		intsci3();
	}
	return 1;
}

#else

BYTE SciByteTx(BYTE data)
{
	
	SCI3.SCR3.BIT.TE=1;
	while(SCI3.SSR.BIT.TDRE!=1);
	
	SCI3.TDR=data;
	
	while(SCI3.SSR.BIT.TEND!=1);
	SCI3.SCR3.BIT.TE=0;
	
	return 1;
}

BYTE SciStrTx(BYTE *str,UUINT dataleng)
{
	UUINT i;
	SCI3.SCR3.BIT.TE=1;
	
	for(i=0;i<dataleng;i++)
		SciByteTx(str[i]);
		
	return 1;
}

#endif


//SCI_2 ************************************************************************//
volatile BYTE sci2_rxrb[SCI2_RB_SIZE];
volatile BYTE rb_rd_num2;
volatile BYTE rb_wr_num2;


BYTE intsci3_2(){
	BYTE str[4],temp,next ;
	int recv;
	int result0;
	

	if((SCI3_2.SSR.BYTE & 0x18)==0)
	{
	
		if(SCI3_2.SSR.BIT.RDRF == 1)
		{          
			
				SCI3_2.SSR.BYTE &= 0x87;
				recv = SCI3_2.RDR;			//  �f�[�^���󂯎��data�ɕۑ�
				result0 = ifsnd_dtq( DTQID_RADICON, recv);
				return 0;
			
			
		}
		
	
	}
	
	temp=SCI3_2.RDR;
	SCI3_2.SSR.BYTE &= 0x87; 
    return 1;
}

BYTE Sci_2ByteRx(BYTE *data)
{
	if(rb_rd_num2!=rb_wr_num2)	//��M�o�b�t�@�󂶂�Ȃ�������
	{	
		*data = sci2_rxrb[rb_rd_num2];
		rb_rd_num2 = (rb_rd_num2+1)%SCI2_RB_SIZE;
		return 1;
	}
	else
		return 0;
}
/*20201002�C��
BYTE intsci3_2(){
	BYTE str[4],temp,next;
	

	if((SCI3_2.SSR.BYTE & 0x18)==0 && SCI3_2.SSR.BIT.RDRF == 1)
	{          
		next=(rb_wr_num2+1)%SCI2_RB_SIZE;
		
		if(rb_rd_num2!=next){
			SCI3_2.SSR.BYTE &= 0x87;
			sci2_rxrb[rb_wr_num2] = SCI3_2.RDR;			//  �f�[�^���󂯎��data�ɕۑ� 
			rb_wr_num2 = next;
			return 0;
		}
	}
	
	temp=SCI3_2.RDR;
	SCI3_2.SSR.BYTE &= 0x87;  
    return 1;
}

BYTE Sci_2ByteRx(BYTE *data)
{
	if(rb_rd_num2!=rb_wr_num2)	//��M�o�b�t�@�󂶂�Ȃ�������
	{	
		*data = sci2_rxrb[rb_rd_num2];
		rb_rd_num2 = (rb_rd_num2+1)%SCI2_RB_SIZE;
		return 1;
	}
	else
		return 0;
}
*/
void InitSci3_2(BYTE baudrate,BYTE parity ,BYTE stop)
{
	UUINT dmy;
	BYTE smr = 0;
	
	switch(parity){
		case even:
			smr = 0x20;
		break;
		case odd:
			smr = 0x30;
		break;
		case non:
		default:
			smr = 0x00;
		break;
	}
	if(stop == 2){
		smr |= 0x08;
	}
	rb_rd_num2 = 0;
	rb_wr_num2 = 0;
	
	SCI3_2.SCR3.BYTE = 0;       //�رٺ��۰�ڼ޽�(SCI�̑��M/��M����ݒ�)
	SCI3_2.SMR.BYTE = smr;       //�ر�Ӱ��ڼ޽�(SCI�̒ʐM̫�ϯĂ̐ݒ�)
	SCI3_2.BRR = (BYTE)baudrate;	          //1bps
	
	for(dmy = 280;dmy > 0;dmy--); //Wait 1bit transfer time
	SCI3_2.SCR3.BYTE = 0x70;    //Enable Tx & Rx ,enable Rx interrupt
	IO.PMR1.BIT.TXD2 = 1;
	SCI3_2.SSR.BYTE &= 0x80;   //Clear receive error flag

}

void Sci_2ByteTx(BYTE data)
{
	
	SCI3_2.SCR3.BIT.TE=1;
	while(SCI3_2.SSR.BIT.TDRE!=1);
	
	SCI3_2.TDR=data;
	
	while(SCI3_2.SSR.BIT.TEND!=1);
	SCI3_2.SCR3.BIT.TE=0;
}


void Sci_2StrTx(BYTE *str,UUINT dataleng)
{
	UUINT i;
	SCI3_2.SCR3.BIT.TE=1;
	
	for(i=0;i<dataleng;i++)
		Sci_2ByteTx(str[i]);
}