; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; ����Ŀ���ļ���ģʽ
[INSTRSET "i486p"]				; 486�̖��߂܂Ŏg�������Ƃ����L�q
[BITS 32]						; ����32λģʽ�õĻ�������
[FILE "naskfunc.nas"]			; Դ�ļ�����Ϣ������Ŀ���ļ�����Ϣ��

		GLOBAL	_io_hlt, _io_cli, _io_sti, _io_stihlt		;�����а����ĺ�����
		GLOBAL	_io_in8,  _io_in16,  _io_in32
		GLOBAL	_io_out8, _io_out16, _io_out32
		GLOBAL	_io_load_eflags, _io_store_eflags
		GLOBAL	_load_gdtr, _load_idtr
		GLOBAL	_load_cr0, _store_cr0
		GLOBAL	_load_tr
		GLOBAL	_asm_inthandler20, _asm_inthandler21
		GLOBAL	_asm_inthandler2c, _asm_inthandler0c
		GLOBAL	_asm_inthandler0d, _asm_end_app
		GLOBAL	_memtest_sub
		GLOBAL	_farjmp, _farcall
		GLOBAL	_asm_hrb_api, _start_app
		EXTERN	_inthandler20, _inthandler21
		EXTERN	_inthandler2c, _inthandler0d
		EXTERN	_inthandler0c
		EXTERN	_hrb_api
		
;������ʵ�ʵĺ���
[SECTION .text]		;Ŀ���ļ���д����Щ֮����д����

_io_hlt:	; void io_hlt(void);
		HLT
		RET

_io_cli:	; void io_cli(void);
		CLI		;���жϱ�־��Ϊ0��ָ��
		RET

_io_sti:	; void io_sti(void);
		STI		;���жϱ�־��Ϊ1��ָ��
		RET

_io_stihlt:	; void io_stihlt(void);
		STI
		HLT
		RET

;����8λ���ݣ��豸������Ӣ����Ϊport
_io_in8:	; int io_in8(int port);
		MOV		EDX,[ESP+4]		; port   ESPΪ32λջָ��Ĵ�����EDXΪ32λ���ݼĴ���
		MOV		EAX,0				;EAXΪ32λ�ۼӼĴ���
		IN		AL,DX				;ALΪ�ۼӼĴ�����λ��DXΪ16λ���ݼĴ���  INΪ���豸ȡ�õ����ź�
		RET

_io_in16:	; int io_in16(int port);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AX,DX
		RET

_io_in32:	; int io_in32(int port);
		MOV		EDX,[ESP+4]		; port
		IN		EAX,DX
		RET

;���8λ���ݣ�portΪ�豸����
_io_out8:	; void io_out8(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		AL,[ESP+8]		; data
		OUT		DX,AL		;OUTΪ���豸���͵��ź�
		RET

_io_out16:	; void io_out16(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,AX
		RET

_io_out32:	; void io_out32(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,EAX
		RET

_io_load_eflags:	; int io_load_eflags(void);
		PUSHFD		; ָ����PUSH EFLAGS��ָ���ǽ���־λ��ֵ��˫�ֳ�ѹ��ջ
		POP		EAX	;��������ֵ����EAX
		RET		;����ʱ��EAX�е�ֵ�ͱ������Ǻ����ķ���ֵ

_io_store_eflags:	; void io_store_eflags(int eflags);
		MOV		EAX,[ESP+4]
		PUSH	EAX
		POPFD		; ָ����POP EFLAGS������˫�ֳ�����־λ��ջ����
		RET
;�������������ָ���Ķ����ޣ�limit���͵�ַ��ֵ����ΪGDTR��48λ�Ĵ�����
;�������ַ��ֵ��ʱ��Ψһ�ķ�������ָ��һ���ڴ��ַ����ָ���ĵ�ַ��ȡ6���ֽڣ�48λ��
;�����ָ�����LGDT��GDTR�ĵ�16λ�Ƕ����ޣ�ʣ�µĸ�32λ����GDT�Ŀ�ʼ��ַ
_load_gdtr:		; void load_gdtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

_load_idtr:		; void load_idtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET

_load_cr0:		; int load_cr0(void);
		MOV		EAX,CR0
		RET

_store_cr0:		; void store_cr0(int cr0);
		MOV		EAX,[ESP+4]
		MOV		CR0,EAX
		RET

_load_tr:		; void load_tr(int tr);
		LTR		[ESP+4]			; tr
		RET

_asm_inthandler20:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler20
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD
		
;���̵��жϴ������
_asm_inthandler21:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler21
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler2c:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler2c
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD

_asm_inthandler0c:
		STI
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler0c
		CMP		EAX,0
		JNE		_asm_end_app
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		ADD		ESP,4			; ��INT 0x0c ��Ҳ��Ҫ���
		IRETD
;���쳣��֧�֣����ж��쳣
_asm_inthandler0d:
		STI
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX,ESP
		PUSH	EAX
		MOV		AX,SS
		MOV		DS,AX
		MOV		ES,AX
		CALL	_inthandler0d
		CMP		EAX,0			; ֻ�����ﲻͬ
		JNE		_asm_end_app	; ֻ�����ﲻͬ
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		ADD		ESP,4			; INT 0x0d ����Ҫ���
		IRETD

_memtest_sub:	; unsigned int memtest_sub(unsigned int start, unsigned int end)
		PUSH	EDI						; ���ڻ���ʹ��EBX, ESI, EDI
		PUSH	ESI
		PUSH	EBX
		MOV		ESI,0xaa55aa55			; pat0 = 0xaa55aa55;
		MOV		EDI,0x55aa55aa			; pat1 = 0x55aa55aa;
		MOV		EAX,[ESP+12+4]			; i = start;
mts_loop:
		MOV		EBX,EAX
		ADD		EBX,0xffc				; p = i + 0xffc;
		MOV		EDX,[EBX]				; old = *p;
		MOV		[EBX],ESI				; *p = pat0;
		XOR		DWORD [EBX],0xffffffff	; *p ^= 0xffffffff;
		CMP		EDI,[EBX]				; if (*p != pat1) goto fin;
		JNE		mts_fin
		XOR		DWORD [EBX],0xffffffff	; *p ^= 0xffffffff;
		CMP		ESI,[EBX]				; if (*p != pat0) goto fin;
		JNE		mts_fin
		MOV		[EBX],EDX				; *p = old;
		ADD		EAX,0x1000				; i += 0x1000;
		CMP		EAX,[ESP+12+8]			; if (i <= end) goto mts_loop;
		JBE		mts_loop
		POP		EBX
		POP		ESI
		POP		EDI
		RET
mts_fin:
		MOV		[EBX],EDX				; *p = old;
		POP		EBX
		POP		ESI
		POP		EDI
		RET

_farjmp:		; void farjmp(int eip, int cs);
		JMP		FAR	[ESP+4]				; eip, cs
		RET

_farcall:		; void farcall(int eip, int cs);
		CALL	FAR	[ESP+4]				; eip, cs
		RET

_asm_hrb_api:
		; Ϊ����������ӿ�ͷ�ͽ�ֹ�ж�����
		STI
		PUSH	DS
		PUSH	ES
		PUSHAD		; ���ڱ����PUSH
		PUSHAD		; ������hrb_api��ֵ��PUSH
		MOV		AX,SS
		MOV		DS,AX		; ������ϵͳ�öε�ַ����DS��ES
		MOV		ES,AX
		CALL	_hrb_api
		CMP		EAX,0		; ��EAX��Ϊ0ʱ�������
		JNE		_asm_end_app
		ADD		ESP,32
		POPAD
		POP		ES
		POP		DS
		IRETD
_asm_end_app:
;	EAXΪtss.esp0�ĵ�ַ
		MOV		ESP,[EAX]
		MOV		DWORD [EAX+4],0
		POPAD
		RET					; ����cmd_app

;��������Ӧ�ó���ĺ���
_start_app:		; void start_app(int eip, int cs, int esp, int ds, int *tss_esp0);
		PUSHAD		; ��32λ�Ĵ�����ֵȫ����������
		MOV		EAX,[ESP+36]	; Ӧ�ó�����EIP
		MOV		ECX,[ESP+40]	; Ӧ�ó�����CS
		MOV		EDX,[ESP+44]	; Ӧ�ó�����ESP
		MOV		EBX,[ESP+48]	; Ӧ�ó�����DS/SS
		MOV		EBP,[ESP+52]	; tss.esp0�ĵ�ַ
		MOV		[EBP  ],ESP		; �������ϵͳ��ESP
		MOV		[EBP+4],SS		; �������ϵͳ��SS
		MOV		ES,BX
		MOV		DS,BX
		MOV		FS,BX
		MOV		GS,BX
;	�������ջ��������RETF��ת��Ӧ�ó���
		OR		ECX,3			; ��Ӧ�ó����öκź�3����OR����
		OR		EBX,3			; ��Ӧ�ó����öκź�3����OR����
		PUSH	EBX				; Ӧ�ó����õ�SS
		PUSH	EDX				; Ӧ�ó����õ�ESP
		PUSH	ECX				; Ӧ�ó����õ�CS
		PUSH	EAX				; Ӧ�ó����õ�EIP
		RETF
;	Ӧ�ó�������󲻻�ص�����
