
	AREA |.text|, CODE, READONLY, ALIGN=2
	THUMB
	REQUIRE8
	PRESERVE8

	EXTERN  RunPt            ; currently running thread
	EXPORT  StartOS
	EXPORT  SysTick_Handler
	IMPORT  Scheduler
		
	;	RunPt		Adress_of_RunPt | Adress of SP  - Address of Next  
	;	R0 = RunPt  R0 = Address_of_RunPt
	;	R1 = [R1]	R1 = Address of SP of RunPt. Deference RunPt 
	;

SysTick_Handler                 ; 1) Saves R0-R3,R12,LR,PC,PSR
    CPSID   I                   ; 2) Prevent interrupt during switch
    PUSH    {R4-R11}            ; 3) Save remaining regs r4-11
    LDR     R0, =RunPt          ; 4) R0=pointer to RunPt, old thread
    LDR     R1, [R0]            ;    R1 = RunPt
    STR     SP, [R1]            ; 5) Save SP into TCB
    PUSH    {R0,LR}
    BL      Scheduler
    POP     {R0,LR}
    LDR     R1, [R0]            ; 6) R1 = RunPt, new thread
    LDR     SP, [R1]            ; 7) new thread SP; SP = RunPt->sp;
    POP     {R4-R11}            ; 8) restore regs r4-11
    CPSIE   I                   ; 9) tasks run with interrupts enabled
    BX      LR                  ; 10) restore R0-R3,R12,LR,PC,PSR
	
StartOS

    LDR     R0, =RunPt         ; currently running thread
    LDR     R2, [R0]           ; R2 = value of RunPt
    LDR     SP, [R2]           ; new thread SP; SP = RunPt->stackPointer;
    POP     {R4-R11}           ; restore regs r4-11
    POP     {R0-R3}            ; restore regs r0-3
    POP     {R12}
    ADD     SP,SP,#4           ; discard LR from initial stack
    POP     {LR}               ; start location
    ADD     SP,SP,#4           ; discard PSR
    CPSIE   I                  ; Enable interrupts at processor level
    BX      LR                 ; start first thread

    ALIGN
    END
