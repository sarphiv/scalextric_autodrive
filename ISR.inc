;Interrupt service routine module
    
ISR_INIT
    BCF	    STATUS, RP0		;Change to bank 0
    
    MOVLW   h'00'
    MOVWF   ISR_WCopy
    MOVWF   ISR_BCopy
    
    BSF	    INTCON, GIE		;Enable general interrupts (master interrupt switch)
    
    RETURN

ISR
    CALL    ISR_BACKUP		;Backup to registers to not interfere with main loop
    
    BCF	    STATUS, RP0		;Change to bank 0
    
    BTFSC   INTCON, INTF	;Check if triggered by hall effect sensor
    CALL    HallEffect_TRIG
    
    BTFSC   PIR1, C2IF		;Check if triggered by LDR change
    CALL    LDR_TRIG

    
    CALL    ISR_RESTORE

    RETFIE
    
ISR_BACKUP  
    BTFSS   STATUS, RP0		;Backup bank select bit
    GOTO    BANKBIT_CLEAR
    ;else
BANKBIT_SET
    BCF	    STATUS, RP0		;Change to bank 0
    BSF	    ISR_BCopy, BANKBIT
    
    GOTO    WORKING_COPY
    
BANKBIT_CLEAR
    BCF	    STATUS, RP0		;Change to bank 0
    BCF	    ISR_BCopy, BANKBIT

WORKING_COPY
    MOVWF   ISR_WCopy		;Backup working register
    
    RETURN
    
ISR_RESTORE
    BCF	    STATUS, RP0		;Change to bank 0
    
    MOVF    ISR_WCopy, W	;Restore copy of working register
    
    ;Restore bank select bit
    BTFSS   ISR_BCopy, BANKBIT	
    GOTO    BANKBIT_RESTORE_CLEAR
    ;else
    BSF	    STATUS, RP0
    
    RETURN
    
BANKBIT_RESTORE_CLEAR
    BCF	    STATUS, RP0
    
    RETURN