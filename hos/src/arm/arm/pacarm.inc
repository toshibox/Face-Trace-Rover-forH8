; --------------------------------------------------------------------------- 
;  Hyper Operating System V4  ��ITRON4.0�d�l Real-Time OS                     
;    �A�Z���u�����ꋤ�ʃC���N���[�h�t�@�C��                                   
;                                                                             
;                                     Copyright (C) 1998-2002 by Project HOS  
;                                     http://sourceforge.jp/projects/hos/     
; --------------------------------------------------------------------------- 


; �t���O��`
Mode_USR		EQU 	0x10		; USR ���[�h
Mode_FIQ		EQU		0x11		; FIQ ���[�h
Mode_IRQ		EQU 	0x12		; IRQ ���[�h
Mode_SVR		EQU		0x13		; Supervisor ���[�h
Mode_ABT		EQU		0x17		; Abort ���[�h
Mode_UND		EQU		0x1b		; Undefined ���[�h
Mode_SYS		EQU 	0x1f		; System ���[�h

T_Bit			EQU		0x20		; T �r�b�g
F_Bit			EQU 	0x40		; F �r�b�g
I_Bit			EQU 	0x80		; I �r�b�g


; CPU��O�n���h���ԍ���`
EXCNO_UND		EQU		1			; Undefined��O
EXCNO_SWI		EQU		2			; SWI��O
EXCNO_PRE		EQU		3			; �v���t�F�b�`�A�{�[�g
EXCNO_DAT		EQU		4			; �f�[�^�A�{�[�g


				END


; ---------------------------------------------------------------------------
;  Copyright (C) 1998-2002 by Project HOS                                    
; ---------------------------------------------------------------------------