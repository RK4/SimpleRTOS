.text
.globl StartOS
.globl SysTick_Handler
.extern RunPt
.weak Scheduler

.syntax unified
.cpu    cortex-m4
.thumb_func

SysTick_Handler:            @ 1) Saves R0-R3,R12,LR,PC,PSR
    CPSID   I               @ 2) Prevent interrupt during switch
    PUSH    {R4-R11}        @ 3) Save remaining regs r4-11
    LDR     R0, =RunPt      @ 4) R0=pointer to RunPt, old thread
    LDR     R1, [R0]        @          R1 = RunPt
    STR     SP, [R1]        @ 5) Save SP into TCB
    PUSH    {R0,LR}         @
    BL      Scheduler       @
    POP     {R0,LR}         @         
    LDR     R1, [R0]        @ 6) R1 = RunPt, new thread
    LDR     SP, [R1]        @ 7) new thread SP; SP = RunPt->sp;
    POP     {R4-R11}        @ 8) restore regs r4-11
    CPSIE   I               @ 9) tasks run with interrupts enabled
    BX      LR              @ 10) restore R0-R3,R12,LR,PC,PSR

StartOS:                    
	LDR     R0, =RunPt      @ currently running thread    
    LDR     R1, [R0]        @ R2 = value of RunPt
    LDR     SP, [R1]        @ new thread SP; SP = RunPt->stackPointer
    POP     {R4-R11}        @ restore regs r4-11
    POP     {R0-R3}         @ restore regs r0-3
    POP     {R12}           @       
    ADD     SP,SP,#4        @ discard LR from initial stack
    POP     {LR}            @ start location, PC of first thread.   
    ADD     SP,SP,#4        @ discard PSR
    CPSIE   I               @ Enable interrupts at processor level      
    BX      LR              @ start first thread

