;Keeps track of laps
    
LAP_INIT
    BCF	    STATUS, RP0		;Change to bank 0
    MOVLW   h'00'
    MOVWF   LAP_Laps
    
    BCF	    RSTAT, RUN		;Set state to stopped
    
    RETURN

LAP_UPDATE
    BCF	    STATUS, RP0		;Change to bank 0
    BTFSS   RSTAT, MODE
    GOTO    LAP_UPDATE_RACE
    ;else   
    GOTO    LAP_UPDATE_RECON
    

    RETURN			;This line should never be reached

LAP_UPDATE_RECON
    BCF	    STATUS, RP0		;Change to bank 0
    
    INCF    LAP_Laps, F		;Increment laps by one
    
    MOVF    LAP_Laps, W		;Calculate if Laps <= 2 (one run on the track)
    SUBLW   h'01'
    
    BTFSC   STATUS, 0		;If under or equal, set RUN bit in RSTAT
    BSF	    RSTAT, RUN	    
    BTFSS   STATUS, 0		;If over, clear RUN bit in RSTAT
    BCF	    RSTAT, RUN
    
    RETURN
    
LAP_UPDATE_RACE
    BCF	    STATUS, RP0		;Change to bank 0
    INCF    LAP_Laps, F		;Increment laps by one
    
    MOVF    LAP_Laps, W		;Calculate if Laps <= TotalLaps
    SUBLW   TOTALLAPS		
    
    BTFSC   STATUS, 0		;If under or equal, set RUN bit in RSTAT
    BSF	    RSTAT, RUN
    BTFSS   STATUS, 0		;If over, clear RUN bit in RSTAT
    BCF	    RSTAT, RUN
    
    RETURN