      INTEGER FUNCTION NVXINI(IFUNC,IPARMS)                             
                                                                        
C *** McIDAS Revision History ***      
C 1 NVXGVAR.DLM 5-Jul-94,13:01:32,`KENB' Initial release of GVAR navigation
C      module. (4663)
C 2 NVXGVAR.DLM 25-Aug-94,13:30:14,`USER' Released
C 3 NVXGVAR.DLM 29-Aug-94,10:56:58,`RUSSN' Corrected revision history cards
C 4 NVXGVAR.DLM 13-Sep-94,12:09:06,`USER' Released
C 5 NVXGVAR.DLM 28-Oct-94,15:34:28,`KENB' adopted fix from m/f for changes
C 6 NVXGVAR.DLM 31-Oct-94,12:12:46,`USER' Released
C *** McIDAS Revision History ***      

      IMPLICIT NONE
                                                                        
C     INCLUDE 'ELCONS.INC'
C=========================== DIELCONS =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCONS = INCLUDE                                                
                                                                        
      REAL*8 PI                                                         
           PARAMETER (PI=3.141592653589793D0)                           
      REAL*8 DEG                                                        
           PARAMETER (DEG=180.D0/PI)                                    
      REAL*8 RAD                                                        
           PARAMETER (RAD=PI/180.D0)                                    
C                    DEGREES TO RADIANS CONVERSION PI/180               
      REAL*8 NOMORB                                                     
           PARAMETER (NOMORB=42164.365D0)                               
C                    NOMINAL RADIAL DISTANCE OF SATELLITE (KM)          
      REAL*8 AE                                                         
           PARAMETER (AE=6378.137D0)                                    
C                    EARTH EQUATORIAL RADIUS (KM)                       
      REAL*8 FER                                                        
           PARAMETER (FER=1.D0-(6356.7533D0/AE))                        
C                    EARTH FLATTENING COEFFICIENT = 1-(BE/AE)           
      REAL*4 AEBE2                                                      
           PARAMETER (AEBE2=1.D0/(1.D0-FER)**2)                         
      REAL*4 AEBE3                                                      
           PARAMETER (AEBE3=AEBE2-1.)                                   
      REAL*4 AEBE4                                                      
           PARAMETER (AEBE4=(1.D0-FER)**4-1.)                           
C========================= END DIELCONS ===========================     
C     INCLUDE 'ELCOMM.INC'
C=========================== DIELCOMM =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCOMM = INCLUDE                                                
                                                                        
C     COMMON VARIABLES                                                  
C                                                                       
      REAL*8 XS(3)                                                      
C                      NORMALIZED S/C POSITION IN ECEF COORDINATES      
      REAL*8 BT(3,3)                                                    
C                      ECEF TO INSTRUMENT COORDINATES TRANSFORMATION    
      REAL*8  Q3                                                        
C                      USED IN SUBRTN LPOINT                        
      REAL*8 PITCH,ROLL,YAW                                             
C                          PITCH,ROLL,YAW ANGLES OF INSTRUMENT (RAD)    
      REAL*4 PMA,RMA                                                    
C                          PITCH,ROLL MISALIGNMENTS OF INSTRUMENT (RAD) 
         COMMON /ELCOMM/ XS,BT,Q3,PITCH,ROLL,YAW,PMA,RMA                
C========================= END DIELCOMM ===========================     
C     INCLUDE 'INSTCO.INC'
C=========================== DIINSTCO =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIINSTCO = INCLUDE                                                
                                                                        
      INTEGER*4 INCMAX(2)                                               
C                       NUMBER OF INCREMENTS PER CYCLE                  
      REAL*4 ELVMAX(2)                                                  
C                       BOUNDS IN ELEVATION (RADIANS)                   
      REAL*4 SCNMAX(2)                                                  
C                       BOUNDS IN SCAN ANGLE (RADIANS)                  
      REAL*4 ELVINC(2)                                                  
C                       CHANGE IN ELEVATION ANGLE PER INCREMENT (RAD)   
      REAL*4 SCNINC(2)                                                  
C                       CHANGE IN SCAN ANGLE PER INCREMENT (RADIANS)    
      REAL*4 ELVLN(2)                                                   
C                       ELEVATION ANGLE PER DETECTOR LINE (RADIANS)     
      REAL*4 SCNPX(2)                                                   
C                       SCAN ANGLE PER PIXEL (RADIANS)                  
                                                                        
      COMMON /INSTCO/ INCMAX,ELVMAX,SCNMAX,                             
     1   ELVINC,SCNINC,ELVLN,SCNPX                                      
C========================= END DIINSTCO ===========================     
C     INCLUDE 'GVRCOD.INC'
C=========================== DIGVRCOD =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE GVAR NAVIGATION SUBSYSTEM.       
C $                                                                     
C $   DESCRIPTION:                                                      
C $      THESE ARE CONSTANTS USED TO DESCRIBE THE GVAR CODICIL STRUCTURE
C $                                                                     
C $$  DIGVRCOD = INCLUDE, NAVIGATION, GVAR                              
                                                                        
C DEFINITIONS OF PARAMETERS                                             
C STTYPE   : POSITION OF SATELLITE TYPE                                 
C ETIME    : POSITION OF EPOCH TIME                                     
C IMCACT   : POSITION OF IMC ACTIVE FLAG                                
C IMGDAY   : POSITION OF IMAGE DAY VALUE  (YYDDD)                       
C IMGTM    : POSITION OF IMAGE TIME VALUE (HHMMSS)                      
C REFLON   : POSITION OF REFERENCE LONGITUDE                            
C REFDIS   : POSITION OF REFERENCE DISTANCE FROM NOMINAL                
C REFLAT   : POSITION OF REFERENCE LATITUDE                             
C REFYAW   : POSITION OF REFERENCE YAW                                  
C RATROL   : POSITION OF REFERENCE ATTITUDE ROLL                        
C RATPTC   : POSITION OF REFERENCE ATTITUDE PITCH                       
C RATYAW   : POSITION OF REFERENCE ATTITUDE YAW                         
C LDR1-13  : LOCATION OF LONGITUDE DELTA FROM REFERENCE PARAMETERS      
C RDDR1-11 : LOCATION OF RADIAL DISTANCE DELTA FROM REFERENCE PARAMETERS
C DGL1-9   : LOCATION OF GEOCENTRIC LATITUDE DELTA PARAMETERS           
C DOY1-9   : LOCATION OF ORBIT YAW DELTA PARAMETERS                     
C EXPTIM   : EXPONENTIAL START TIME FROM EPOCH                          
C RAAWDS   : LOCATION OF START OF ROLL ATTITUDE ANGLE WORDS             
C PAAWDS   : LOCATION OF START OF PITCH ATTITUDE ANGLE WORDS            
C YAAWDS   : LOCATION OF START OF YAW ATTITUDE ANGLE WORDS              
C RMAWDS   : LOCATION OF START OF ROLL MISALIGNMENT ANGLE WORDS         
C PMAWDS   : LOCATION OF START OF PITCH MISALIGNMENT ANGLE WORDS        
C EDTIME   : LOCATION OF DELTA FROM EPOCH TIME                          
C IMCROL   : LOCATION OF IMAGE MOTION COMPENSATION ROLL                 
C IMCPTC   : LOCATION OF IMAGE MOTION COMPENSATION PITCH                
C IMCYAW   : LOCATION OF IMAGE MOTION COMPENSATION YAW                  
C IMGSND   : LOCATION OF IMAGER/SOUNDER INSTRUMENT FLAG                 
                                                                        
      INTEGER      ETIME , IMCACT, IMGDAY, IMGTM , RATROL, RATPTC       
      INTEGER      RATYAW, LDR1  , LDR2  , LDR3  , LDR4  , LDR5         
      INTEGER      LDR6  , LDR7  , LDR8  , LDR9  , LDR10 , LDR11        
      INTEGER      LDR12 , LDR13 , RDDR1 , RDDR2 , RDDR3 , RDDR4        
      INTEGER      RDDR5 , RDDR6 , RDDR7 , RDDR8 , RDDR9 , RDDR10       
      INTEGER      RDDR11, DGL1  , DGL2  , DGL3  , DGL4  , DGL5         
      INTEGER      DGL6  , DGL7  , DGL8  , DGL9  , DOY1  , DOY2         
      INTEGER      DOY3  , DOY4  , DOY5  , DOY6  , DOY7  , DOY8         
      INTEGER      DOY9  , EXPTIM, RAAWDS, PAAWDS, YAAWDS, RMAWDS       
      INTEGER      PMAWDS, EDTIME, IMCROL, IMCPTC, IMCYAW, IMGSND       
      INTEGER      REFLON, REFDIS, REFLAT, REFYAW, STTYPE, IDNTFR
      INTEGER      IOFNC , IOFNI , IOFEC , IOFEI
                                                                        
      PARAMETER (STTYPE = 1)                                            
      PARAMETER (IDNTFR = 2)
      PARAMETER (IMCACT = 3)                                            
      PARAMETER (REFLON = 6)                                            
      PARAMETER (REFDIS = 7)                                            
      PARAMETER (REFLAT = 8)                                            
      PARAMETER (REFYAW = 9)                                            
      PARAMETER (RATROL = 10)                                           
      PARAMETER (RATPTC = 11)                                           
      PARAMETER (RATYAW = 12)                                           
      PARAMETER (ETIME  = 13)                                           
      PARAMETER (EDTIME = 15)                                           
      PARAMETER (IMCROL = 16)                                           
      PARAMETER (IMCPTC = 17)                                           
      PARAMETER (IMCYAW = 18)                                           
      PARAMETER (LDR1   = 19)                                           
      PARAMETER (LDR2   = 20)                                           
      PARAMETER (LDR3   = 21)                                           
      PARAMETER (LDR4   = 22)                                           
      PARAMETER (LDR5   = 23)                                           
      PARAMETER (LDR6   = 24)                                           
      PARAMETER (LDR7   = 25)                                           
      PARAMETER (LDR8   = 26)                                           
      PARAMETER (LDR9   = 27)                                           
      PARAMETER (LDR10  = 28)                                           
      PARAMETER (LDR11  = 29)                                           
      PARAMETER (LDR12  = 30)                                           
      PARAMETER (LDR13  = 31)                                           
      PARAMETER (RDDR1  = 32)                                           
      PARAMETER (RDDR2  = 33)                                           
      PARAMETER (RDDR3  = 34)                                           
      PARAMETER (RDDR4  = 35)                                           
      PARAMETER (RDDR5  = 36)                                           
      PARAMETER (RDDR6  = 37)                                           
      PARAMETER (RDDR7  = 38)                                           
      PARAMETER (RDDR8  = 39)                                           
      PARAMETER (RDDR9  = 40)                                           
      PARAMETER (RDDR10 = 41)                                           
      PARAMETER (RDDR11 = 42)                                           
      PARAMETER (DGL1   = 43)                                           
      PARAMETER (DGL2   = 44)                                           
      PARAMETER (DGL3   = 45)                                           
      PARAMETER (DGL4   = 46)                                           
      PARAMETER (DGL5   = 47)                                           
      PARAMETER (DGL6   = 48)                                           
      PARAMETER (DGL7   = 49)                                           
      PARAMETER (DGL8   = 50)                                           
      PARAMETER (DGL9   = 51)                                           
      PARAMETER (DOY1   = 52)                                           
      PARAMETER (DOY2   = 53)                                           
      PARAMETER (DOY3   = 54)                                           
      PARAMETER (DOY4   = 55)                                           
      PARAMETER (DOY5   = 56)                                           
      PARAMETER (DOY6   = 57)                                           
      PARAMETER (DOY7   = 58)                                           
      PARAMETER (DOY8   = 59)                                           
      PARAMETER (DOY9   = 60)                                           
      PARAMETER (EXPTIM = 62)                                           
      PARAMETER (RAAWDS = 63)                                           
      PARAMETER (PAAWDS = 130)                                          
      PARAMETER (YAAWDS = 185)                                          
      PARAMETER (RMAWDS = 258)                                          
      PARAMETER (PMAWDS = 313)                                          
                                                                        
      PARAMETER (IMGDAY = 368)                                          
      PARAMETER (IMGTM  = 369)                                          
      PARAMETER (IMGSND = 370)                                          
                                                                        
C THESE FOUR WORDS WERE ADDED 5-26-94 TO COMPLY W/ THE NEW ELUG         
C NUMBERING STARTED AT 380 BECAUSE THESE SAME PARAMETERS ARE USED       
C IN THE NAV MESSAGE SENT FROM THE INGESTOR TO EVX, AND WE HAD TO       
C START SOMEWHERE AFTER THE 378 NAV PARAMETERS                          
      PARAMETER (IOFNC = 380)                                           
      PARAMETER (IOFEC = 381)                                           
      PARAMETER (IOFNI = 382)                                           
      PARAMETER (IOFEI = 383)
                                                                        
C========================= END DIGVRCOD ===========================     
C     INCLUDE 'MXCDSZ.INC'
C=========================== DIMXCDSZ =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE NAVIGATION SUBSYSTEM             
C $                                                                     
C $   DESCRIPTION:                                                      
C $      THIS IS THE VALUE OF LARGEST NAVIGATION CODICIL SIZE ALLOWED   
C $                                                                     
C $$  DIMXCDSZ = INCLUDE, NAVIGATION                                    
                                                                        
C DEFINITIONS OF PARAMETERS                                             
C MXCDSZ   : LARGEST ALLOWED CODICIL SIZE                               
                                                                        
      INTEGER      MXCDSZ                                               
                                                                        
      PARAMETER (MXCDSZ = 5*128)                                        
                                                                        
C========================= END DIMXCDSZ ===========================     
C     INCLUDE 'OASET.INC'
C=========================== DIOASET =============================      
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE GVAR NAVIGATION SUBSYSTEM.       
C $                                                                     
C $   DESCRIPTION:                                                      
C $      THESE ARE CONSTANTS USED TO DESCRIBE THE GVAR O&A SET STRUCTURE
C $                                                                     
C $$  DIOASET = INCLUDE, NAVIGATION, GVAR                               
                                                                        
C DEFINITIONS OF PARAMETERS                                             
C OASIZE   : SIZE OF GVAR ORBIT AND ATTITUDE SET                        
C PCOEFS   : START OF PITCH COEFFICIENTS                                
C RMACFS   : START OF RMA COEFFICIENTS                                  
C CUTOF1   : FIRST DIVIDING POINT IN O&A SET (WHERE DIVIDED INTO 128    
C            WORD SETS)                                                 
C CUTOF2   : SECOND DIVIDING POINT IN O&A SET (WHERE DIVIDED INTO 128   
C            WORD SETS)                                                 
                                                                        
      INTEGER      OASIZE, PCOEFS, RMACFS, CUTOF1, CUTOF2               
                                                                        
      PARAMETER (OASIZE = 336)                                          
      PARAMETER (PCOEFS = 117)                                          
      PARAMETER (RMACFS = 227)                                          
      PARAMETER (CUTOF1 = 116)                                          
      PARAMETER (CUTOF2 = 226)                                          
                                                                        
C========================= END DIOASET ===========================      
C     INCLUDE 'B11DOC.INC'
C=========================== DIB11DOC =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE DECODER SCHEDULER SUBSYSTEM.     
C $                                                                     
C $   DESCRIPTION:                                                      
C $      THESE ARE INDICES INTO THE SOUNDER DOCUMENTATION BLOCK         
C $      (INDICES ARE 1-BASED FROM START OF SAD ID)                     
C $                                                                     
C $$  DIB11DOC = INCLUDE, DECODER                                       
                                                                        
C DEFINITIONS OF PARAMETERS                                             
C     SSCAN   SOUNDER SCAN STATUS IN TWO BYTES                          
C     SGBCT   TOTAL NUMBER OF BLOCK 11'S IN THIS SCAN (INCLUDES DOC)    
C     TSCLS   TIME OF SCAN LINE START IN 8 BYTES                        
C     NEWIMG  BIT # IN SSCAN FOR NEW IMAGE FLAG                         
C     ENDIMG  BIT # IN SSCAN FOR END OF IMAGE FLAG                      
C     WARN    BIT # IN SSCAN FOR WARNING MODE FLAG                      
C     DWELL1  BIT # IN SSCAN FOR DWELL MODE 1 FLAG                      
C     DWELL2  BIT # IN SSCAN FOR DWELL MODE 2 FLAG                      
C     DWELL4  BIT # IN SSCAN FOR DWELL MODE 4 FLAG                      
C     IMCFLG  BIT # IN SSCAN FOR IMC ACTIVE FLAG                        
C     STPLOC  BIT # IN SSCAN FOR STEP MODE FLAG                         
C     TIMSIZ  NUMBER OF BYTES THAT THE TIME OCCUPIES                    
C     SNSLN   NORTHERN MOST SCAN LINE OF CURRENT SCAN                   
C     SWFPX   WESTERNMOST PIXEL # OF CURRENT FRAME                      
C     SEFPX   EASTERNMOST PIXEL # OF CURRENT FRAME                      
C     SNFLN   NORTHERNMOST SCAN LINE # OF CURRENT FRAME                 
C     SSFLN   SOUTHERNMOST SCAN LINE # OF CURRENT FRAME                 
C     NAVLOC  START OF O&A PARAMETERS                                   
C     NAVEND  END OF O&A PARAMETERS                                     
                                                                        
      INTEGER      SSCAN , SGBCT , TSCLS , NEWIMG, ENDIMG, TIMSIZ       
      INTEGER      SWFPX , SEFPX , SNFLN , SSFLN , SDSTA , DWELL1       
      INTEGER      NAVLOC, DWELL2, DWELL4, SNSLN , WARN  , STPLOC       
      INTEGER      NAVEND, IMCFLG                                       
                                                                        
C LOCATION OF FIELDS WITHIN DOCUMENTATION BLOCK                         
      PARAMETER (SSCAN  = 31)                                           
      PARAMETER (SDSTA  = 33)                                           
      PARAMETER (SGBCT  = 45)                                           
      PARAMETER (TSCLS  = 67)                                           
      PARAMETER (NEWIMG = 15)                                           
      PARAMETER (ENDIMG = 14)                                           
      PARAMETER (WARN   = 11)                                           
      PARAMETER (STPLOC = 3)                                            
      PARAMETER (DWELL1 = 4)                                            
      PARAMETER (DWELL2 = 5)                                            
      PARAMETER (DWELL4 = 6)                                            
      PARAMETER (IMCFLG = 7)                                            
      PARAMETER (TIMSIZ = 8)                                            
      PARAMETER (SNSLN  = 185)                                          
      PARAMETER (SWFPX  = 187)                                          
      PARAMETER (SEFPX  = 189)                                          
      PARAMETER (SNFLN  = 191)                                          
      PARAMETER (SSFLN  = 193)                                          
      PARAMETER (NAVLOC = 307)                                          
      PARAMETER (NAVEND = 1718)                                         
                                                                        
C========================= END DIB11DOC ===========================     
                                                                        
      REAL*8       TE    , PHI   , PSI   , U     , SINU  , COSU         
      REAL*8       SINOI , COSOI , SLAT  , ASC   , SINASC, COSASC       
      REAL*8       SYAW  , W     , SW    , CW    , S2W   , C2W          
      REAL*8       SW1   , CW1   , SW3   , CW3   , GATT
      REAL*8       SUBLAT, SUBLON, SECS  , WA
      REAL         RPARMS(MXCDSZ)
      REAL*8       IMGTIM, EPOCH , TIMEX , TIME50, R     , B(3,3)       
      REAL*8       AEC   , TS    , DR    , LAM   , DLAT  , DYAW         
      REAL*8       AEBE2C, AEBE3C, AEBE4C, FERC
      INTEGER      IFUNC , LIT   , ITYPE , IMC   , YEAR  , DAY          
      INTEGER      IFTOK , HOUR  , MINUTS, INSTR , IPARMS(MXCDSZ)
      INTEGER      COUNT , OFFSET, LOOP  , IPARM2(MXCDSZ), TIME(2)      
      LOGICAL      BTEST                                                
      CHARACTER*4  CLIT
      CHARACTER*12 CFZ
                                                                        
      EQUIVALENCE(IPARM2,RPARMS)
                                                                        
      COMMON /GVRCOM/ ITYPE , INSTR , SUBLAT, SUBLON                    
      COMMON /RADCOM/ AEC   , FERC  , AEBE2C, AEBE3C, AEBE4C            
      COMMON /SAVCOM/ B     , DR    , PHI                               
                                                                        
C     INCLUDE 'GVRDAT.INC'
C=========================== DIGVRDAT =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE GVAR NAVIGATION SUBSYSTEM.       
C $                                                                     
C $   DESCRIPTION:                                                      
C $      THESE ARE DATA STATEMENTS USED IN THE GVAR NAVIGATION          
C $                                                                     
C $$  DIGVRDAT = INCLUDE, NAVIGATION, GVAR                              
                                                                        
C DEFINITIONS OF PARAMETERS                                             
C RELLST   : TWO DIMENSIONAL ARRAY OF LOCATION OF REALS IN O&A SET      
C INTLST   : ARRAY OF LOCATION OF INTEGERS IN O&A SET                   
                                                                        
      INTEGER      I     , J     , RELLST(40,2), INTLST(55)             
                                                                        
      DATA ((RELLST(I,J),J=1,2),I=1,31) / 5,11,14,64,66,95,99,101,104,  
     &        106,109,111,114,116,117,119,121,150,154,156,159,161,164,  
     &        166,169,171,172,174,176,205,209,211,214,216,219,221,224,  
     &        226,227,229,231,260,264,266,269,271,274,276,279,284,286,  
     &        315,319,321,324,326,329,331,334,336,-1,-1 /               
                                                                        
      DATA (INTLST(I),I=1,55) / 1,2,12,13,65,96,97,98,102,103,107,108,
     &    112,113,120,151,152,153,157,158,162,163,167,168,175,206,207,  
     &    208,212,213,217,218,222,223,230,261,262,263,267,268,272,273,  
     &    277,278,285,316,317,318,322,323,327,328,332,333,-1 /          
                                                                        
C========================= END DIGVRDAT ===========================     
                                                                        
C--------------------------- NVXGVR ------------------------------      

C INITIALIZE THE EARTH RADIUS CONSTANTS                                 
      AEC    = AE                                                       
      FERC   = FER                                                      
      AEBE2C = DBLE(AEBE2)                                              
      AEBE3C = DBLE(AEBE3)                                              
      AEBE4C = DBLE(AEBE4)                                              
                                                                        
C IFUNC = 1                                                             
      IF (IFUNC.EQ.1) THEN                                              
        IF (IPARMS(STTYPE).NE.LIT('GVAR')) THEN                         
          NVXINI = -1                                                   
          RETURN                                                        
        ENDIF                                                           
        ITYPE = 1                                                       
                                                                        
C COPY CODICIL SO WE CAN EQUIVALENCE IT TO REAL ARRAY                   
        DO 1 LOOP = 1, MXCDSZ                                           
 1        IPARM2(LOOP) = IPARMS(LOOP)                                   
                                                                        
C SCALE DOWN REAL CODICIL ELEMENTS                                      
        COUNT = 1                                                       
        RPARMS(IMGTM) = REAL(IPARM2(IMGTM))/1000.                       
 2      IF (RELLST(COUNT,1).EQ.-1) GOTO 4                               
          OFFSET = 1                                                    
          IF (RELLST(COUNT,1).GT.CUTOF1) OFFSET = 13                    
          IF (RELLST(COUNT,1).GT.CUTOF2) OFFSET = 31                    
          DO 3 LOOP=RELLST(COUNT,1),RELLST(COUNT,2)                     
            IF (LOOP.EQ.14.OR.LOOP.EQ.61.OR.(MOD(LOOP-8,55).EQ.0.AND.   
     &                                               LOOP.NE.8)) THEN   
              RPARMS(LOOP+OFFSET) = REAL(IPARM2(LOOP+OFFSET))/100.      
            ELSE                                                        
              RPARMS(LOOP+OFFSET) = REAL(IPARM2(LOOP+OFFSET))/10000000. 
            ENDIF                                                       
 3        CONTINUE                                                      
          COUNT = COUNT + 1                                             
        GOTO 2                                                          
                                                                        
C SEE IF THIS CODICIL IS FOR IMAGER OR SOUNDER                          
 4      INSTR = IPARMS(IMGSND)                                          

C INITIALIZE CONSTANTS IN COMMON INSTCO                                 
        CALL SETCON(INSTR, IPARMS(IOFNC), IPARMS(IOFNI), IPARMS(IOFEC),
     &                                                  IPARMS(IOFEI))
                                                                        
C GET CONTROL INFO FROM CODICIL                                         
        YEAR    = 1900 + IPARMS(IMGDAY) / 1000                          
        DAY     = IPARMS(IMGDAY) - IPARMS(IMGDAY) / 1000 * 1000         
        HOUR    = RPARMS(IMGTM) / 10000                                 
        MINUTS  = RPARMS(IMGTM) / 100 - HOUR * 100                      
        SECS    = RPARMS(IMGTM) - REAL(100*MINUTS) - REAL(10000*HOUR)   
        IMGTIM  = TIMEX(YEAR, DAY, HOUR, MINUTS, SECS)                  
        TIME(1) = IFTOK(CFZ(IPARMS(ETIME)))
        TIME(2) = IFTOK(CFZ(IPARMS(ETIME+1)))
        EPOCH   = TIME50(TIME)                                          
        IMC     = 1
        IF (BTEST(IPARMS(IMCACT),IMCFLG)) IMC = 0
                                                                        
C ASSIGN REFERENCE VALUES TO THE SUBSATELLITE LONGITUDE AND             
C LATITUDE, THE RADIAL DISTANCE AND THE ORBIT YAW.
        LAM = RPARMS(REFLON)                                            
        DR  = RPARMS(REFDIS)                                            
        PHI = RPARMS(REFLAT)                                            
        PSI = RPARMS(REFYAW)                                            
                                                                        
C ASSIGN REFERENCE VALUES TO THE ATTITUDES AND MISALIGNMENTS            
        ROLL  = RPARMS(RATROL)                                          
        PITCH = RPARMS(RATPTC)                                          
        YAW   = RPARMS(RATYAW)                                          
        RMA   = 0.                                                      
        PMA   = 0.                                                      
                                                                        
C IF IMC IS OFF, COMPUTE CHANGES IN THE SATELLITE ORBIT                 
        IF (IMC.NE.0) THEN                                              

C SET REFERENCE RADIAL DISTANCE, LATITUDE AND ORBIT YAW TO ZERO
          DR  = 0.
          PHI = 0.
          PSI = 0.

C COMPUTE TIME SINCE EPOCH (IN MINUTES)                                 
          TS = IMGTIM - EPOCH                                           
                                                                        
C COMPUTES ORBIT ANGLE AND THE RELATED TRIGONOMETRIC FUNKTIONS.         
C EARTH ROTATIONAL RATE=.729115E-4 (RAD/S)                              
          W   = 0.729115D - 4 * 60.0D0 * TS                             
          SW  = DSIN(W)                                                  
          CW  = DCOS(W)                                                  
          SW1 = DSIN(0.927*W)                                            
          CW1 = DCOS(0.927*W)                                            
          S2W = DSIN(2.*W)                                               
          C2W = DCOS(2.*W)                                               
          SW3 = DSIN(1.9268*W)                                           
          CW3 = DCOS(1.9268*W)                                           

C COMPUTES CHANGE IN THE IMC LONGITUDE FROM THE REFERENCE               
          LAM = LAM + RPARMS(LDR1) + (RPARMS(LDR2) + RPARMS(LDR3)*W) * W
     &        + (RPARMS(LDR10)*SW1 + RPARMS(LDR11)*CW1 + RPARMS(LDR4)*SW
     &        + RPARMS(LDR5)*CW + RPARMS(LDR6)*S2W + RPARMS(LDR7)*C2W   
     &        + RPARMS(LDR8)*SW3+RPARMS(LDR9)*CW3 + W*(RPARMS(LDR12)*SW 
     &        + RPARMS(LDR13)*CW))*2.                                   
                                                                        
C COMPUTES CHANGE IN RADIAL DISTANCE FROM THE REFERENCE (KM)            
          DR = DR + RPARMS(RDDR1) + RPARMS(RDDR2)*CW + RPARMS(RDDR3)*SW 
     &          + RPARMS(RDDR4)*C2W + RPARMS(RDDR5)*S2W + RPARMS(RDDR6) 
     &          * CW3+RPARMS(RDDR7)*SW3 + RPARMS(RDDR8)*CW1             
     &          + RPARMS(RDDR9)*SW1 + W*(RPARMS(RDDR10)*CW              
     &          + RPARMS(RDDR11)*SW)                                    
                                                                        
C COMPUTES THE SINE OF THE CHANGE IN THE GEOCENTRIC LATITUDE            
          DLAT = RPARMS(DGL1) + RPARMS(DGL2)*CW + RPARMS(DGL3)*SW       
     &        + RPARMS(DGL4)*C2W + RPARMS(DGL5)*S2W + W*(RPARMS(DGL6)*CW
     &        + RPARMS(DGL7)*SW) + RPARMS(DGL8)*CW1+RPARMS(DGL9)*SW1    
                                                                        
C COMPUTES GEOCENTRIC LATITUDE BY USING AN EXPANSION FOR ARCSINE        
          PHI = PHI + DLAT * (1. + DLAT * DLAT / 6.)                    
                                                                        
C COMPUTES SINE OF THE CHANGE IN THE ORBIT YAW                          
          DYAW = RPARMS(DOY1) + RPARMS(DOY2)*SW + RPARMS(DOY3)*CW       
     &         + RPARMS(DOY4)*S2W + RPARMS(DOY5)*C2W                    
     &         + W*(RPARMS(DOY6)*SW + RPARMS(DOY7)*CW)                  
     &         + RPARMS(DOY8)*SW1 + RPARMS(DOY9)*CW1                    
                                                                        
C COMPUTES THE ORBIT YAW BY USING AN EXPANSION FOR ARCSINE.             
          PSI = PSI + DYAW * (1. + DYAW * DYAW / 6.)                    
                                                                        
C CALCULATION OF CHANGES IN THE SATELLITE ORBIT ENDS HERE               
        ENDIF                                                           

C CONVERSION OF THE IMC LONGITUDE AND ORBIT YAW TO THE SUBSATELLITE     
C LONGITUDE AND THE ORBIT INCLINATION (REF: GOES-PCC-TM-2473, INPUTS    
C REQUIRED FOR EARTH LOCATION AND GRIDDING BY SPS, JUNE 6, 1988)
        SLAT  = DSIN(PHI)
        SYAW  = DSIN(PSI)                                                
        SINOI = SLAT**2 + SYAW**2                                       
        COSOI = DSQRT(1.-SINOI)                                          
        SINOI = DSQRT(SINOI)

        IF (SLAT .EQ. 0.0D0 .AND. SYAW .EQ. 0.0D0) THEN
          U = 0.0D0
        ELSE
          U = DATAN2(SLAT,SYAW)
        ENDIF

        SINU  = DSIN(U)                                                  
        COSU  = DCOS(U)                                                  
                                                                        
C COMPUTES LONGITUDE OF THE ASCENDING NODE                              
        ASC    = LAM-U                                                  
        SINASC = DSIN(ASC)                                               
        COSASC = DCOS(ASC)                                               
                                                                        
C COMPUTES THE SUBSATELLITE GEOGRAPHIC LATITUDE                         
        SUBLAT = DATAN(AEBE2C*DTAN(PHI))                                  
                                                                        
C COMPUTES THE SUBSATELLITE LONGITUDE                                   
        SUBLON = ASC+DATAN2(COSOI*SINU,COSU)                             
                                                                        
C COMPUTES THE SPACECRAFT TO EARTH FIXED COORDINATES TRANSFORMATION     
C MATRIX:                                                               
C     (VECTOR IN ECEF COORDINATES) = B * (VECTOR IN S/C COORDINATES)    
        B(1,2) = -SINASC*SINOI                                          
        B(2,2) =  COSASC*SINOI                                          
        B(3,2) = -COSOI                                                 
        B(1,3) = -COSASC*COSU+SINASC*SINU*COSOI                         
        B(2,3) = -SINASC*COSU-COSASC*SINU*COSOI                         
        B(3,3) = -SLAT                                                  
        B(1,1) = -COSASC*SINU-SINASC*COSU*COSOI                         
        B(2,1) = -SINASC*SINU+COSASC*COSU*COSOI                         
        B(3,1) =  COSU*SINOI                                            
                                                                        
C COMPUTES THE NORMALIZED SPACECRAFT POSITION VECTOR IN EARTH FIXED     
C COORDINATES - XS.                                                     
        R     = (NOMORB+DR)/AEC                                         
        XS(1) = -B(1,3)*R                                               
        XS(2) = -B(2,3)*R                                               
        XS(3) = -B(3,3)*R                                               
                                                                        
C PRECOMPUTES Q3 (USED IN LPOINT)                                       
        Q3 = XS(1)**2 + XS(2)**2 + AEBE2C * XS(3)**2 - 1.0              
                                                                        
C COMPUTES THE ATTITUDES AND MISALIGNMENTS IF IMC IS OFF                
        IF (IMC.NE.0) THEN                                              
                                                                        
C COMPUTES THE SOLAR ORBIT ANGLE                                        
          WA = RPARMS(61)*TS                                            
                                                                        
C COMPUTES THE DIFFERENCE BETWEEN CURRENT TIME, TS, AND THE             
C EXPONENTIAL TIME, IPARMS(62). NOTE THAT BOTH TIMES ARE SINCE EPOCH.   
          TE = TS - RPARMS(EXPTIM)

C COMPUTES ROLL + ROLL MISALIGNMENT                                     
          ROLL = ROLL + GATT(RAAWDS,RPARMS,IPARMS,WA,TE)
                                                                        
C COMPUTES PITCH + PITCH MISALIGNMENT                                   
          PITCH = PITCH + GATT(PAAWDS,RPARMS,IPARMS,WA,TE)              
                                                                        
C COMPUTES YAW                                                          
          YAW = YAW + GATT(YAAWDS,RPARMS,IPARMS,WA,TE)                  
                                                                        
C COMPUTES ROLL MISALIGNMENT                                            
          RMA = GATT(RMAWDS,RPARMS,IPARMS,WA,TE)                        
                                                                        
C COMPUTES PITCH MISALIGNMENT                                           
          PMA = GATT(PMAWDS,RPARMS,IPARMS,WA,TE)                        
                                                                        
C APPLY THE EARTH SENSOR COMPENSATION IF NEEDED                         
          ROLL   = ROLL + RPARMS(IMCROL)
          PITCH  = PITCH + RPARMS(IMCPTC)
          YAW    = YAW + RPARMS(IMCYAW)
        ENDIF                                                           
                                                                        
C COMPUTES THE INSTRUMENT TO EARTH FIXED COORDINATES TRANSFORMATION     
C MATRIX - BT                                                           
        CALL INST2E(ROLL,PITCH,YAW,B,BT)                                
                                                                        
C IFUNC = 2                                                             
      ELSEIF (IFUNC.EQ.2) THEN                                          
        IF (INDEX(CLIT(IPARMS(1)),'LL').NE.0) ITYPE=1                   
        IF (INDEX(CLIT(IPARMS(1)),'XY').NE.0) ITYPE=2                   
      ENDIF                                                             
      NVXINI=0                                                          
      RETURN                                                            
      END                                                               

                                                                
                                                                        
                                                                        
      INTEGER FUNCTION NVXSAE(XLIN,XELE,XDUM,XLAT,XLON,Z)               
                                                                        
      IMPLICIT NONE
                                                                        
      REAL         XLIN  , XELE  , XDUM  , XLAT  , XLON  , Z            
      REAL*8       RL    , RP    , E     , S
      REAL         YLAT  , YLON
      REAL*8       SUBLAT, SUBLON, EVLN  , SCPX  , TMPLAT, TMPLON
      INTEGER      INSTR , ITYPE , STAT                                 
                                                                        
C     INCLUDE 'ELCONS.INC'
C=========================== DIELCONS =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCONS = INCLUDE                                                
                                                                        
      REAL*8 PI                                                         
           PARAMETER (PI=3.141592653589793D0)                           
      REAL*8 DEG                                                        
           PARAMETER (DEG=180.D0/PI)                                    
      REAL*8 RAD                                                        
           PARAMETER (RAD=PI/180.D0)                                    
C                    DEGREES TO RADIANS CONVERSION PI/180               
      REAL*8 NOMORB                                                     
           PARAMETER (NOMORB=42164.365D0)                               
C                    NOMINAL RADIAL DISTANCE OF SATELLITE (KM)          
      REAL*8 AE                                                         
           PARAMETER (AE=6378.137D0)                                    
C                    EARTH EQUATORIAL RADIUS (KM)                       
      REAL*8 FER                                                        
           PARAMETER (FER=1.D0-(6356.7533D0/AE))                        
C                    EARTH FLATTENING COEFFICIENT = 1-(BE/AE)           
      REAL*4 AEBE2                                                      
           PARAMETER (AEBE2=1.D0/(1.D0-FER)**2)                         
      REAL*4 AEBE3                                                      
           PARAMETER (AEBE3=AEBE2-1.)                                   
      REAL*4 AEBE4                                                      
           PARAMETER (AEBE4=(1.D0-FER)**4-1.)                           
C========================= END DIELCONS ===========================     
C     INCLUDE 'ELCOMM.INC'
C=========================== DIELCOMM =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCOMM = INCLUDE                                                
                                                                        
C     COMMON VARIABLES                                                  
C                                                                       
      REAL*8 XS(3)                                                      
C                      NORMALIZED S/C POSITION IN ECEF COORDINATES      
      REAL*8 BT(3,3)                                                    
C                      ECEF TO INSTRUMENT COORDINATES TRANSFORMATION    
      REAL*8  Q3                                                        
C                      USED IN SUBRTN LPOINT                        
      REAL*8 PITCH,ROLL,YAW                                             
C                          PITCH,ROLL,YAW ANGLES OF INSTRUMENT (RAD)    
      REAL*4 PMA,RMA                                                    
C                          PITCH,ROLL MISALIGNMENTS OF INSTRUMENT (RAD) 
         COMMON /ELCOMM/ XS,BT,Q3,PITCH,ROLL,YAW,PMA,RMA                
C========================= END DIELCOMM ===========================     
C     INCLUDE 'OASET.INC'
C=========================== DIOASET =============================      
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE GVAR NAVIGATION SUBSYSTEM.       
C $                                                                     
C $   DESCRIPTION:                                                      
C $      THESE ARE CONSTANTS USED TO DESCRIBE THE GVAR O&A SET STRUCTURE
C $                                                                     
C $$  DIOASET = INCLUDE, NAVIGATION, GVAR                               
                                                                        
C DEFINITIONS OF PARAMETERS                                             
C OASIZE   : SIZE OF GVAR ORBIT AND ATTITUDE SET                        
C PCOEFS   : START OF PITCH COEFFICIENTS                                
C RMACFS   : START OF RMA COEFFICIENTS                                  
C CUTOF1   : FIRST DIVIDING POINT IN O&A SET (WHERE DIVIDED INTO 128    
C            WORD SETS)                                                 
C CUTOF2   : SECOND DIVIDING POINT IN O&A SET (WHERE DIVIDED INTO 128   
C            WORD SETS)                                                 
                                                                        
      INTEGER      OASIZE, PCOEFS, RMACFS, CUTOF1, CUTOF2               
                                                                        
      PARAMETER (OASIZE = 336)                                          
      PARAMETER (PCOEFS = 117)                                          
      PARAMETER (RMACFS = 227)                                          
      PARAMETER (CUTOF1 = 116)                                          
      PARAMETER (CUTOF2 = 226)                                          
                                                                        
C========================= END DIOASET ===========================      
C     INCLUDE 'INSTCO.INC'
C=========================== DIINSTCO =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIINSTCO = INCLUDE                                                
                                                                        
      INTEGER*4 INCMAX(2)                                               
C                       NUMBER OF INCREMENTS PER CYCLE                  
      REAL*4 ELVMAX(2)                                                  
C                       BOUNDS IN ELEVATION (RADIANS)                   
      REAL*4 SCNMAX(2)                                                  
C                       BOUNDS IN SCAN ANGLE (RADIANS)                  
      REAL*4 ELVINC(2)                                                  
C                       CHANGE IN ELEVATION ANGLE PER INCREMENT (RAD)   
      REAL*4 SCNINC(2)                                                  
C                       CHANGE IN SCAN ANGLE PER INCREMENT (RADIANS)    
      REAL*4 ELVLN(2)                                                   
C                       ELEVATION ANGLE PER DETECTOR LINE (RADIANS)     
      REAL*4 SCNPX(2)                                                   
C                       SCAN ANGLE PER PIXEL (RADIANS)                  
                                                                        
      COMMON /INSTCO/ INCMAX,ELVMAX,SCNMAX,                             
     1   ELVINC,SCNINC,ELVLN,SCNPX                                      
C========================= END DIINSTCO ===========================     
                                                                        
      COMMON /GVRCOM/ ITYPE, INSTR , SUBLAT, SUBLON                     
                                                                        
C--------------------------------------------------------------         
                                                                        
C TRANSFORM LINE/PIXEL TO GEOGRAPHIC COORDINATES:                       
C SET INPUT LINE/PIXEL NUMBERS                                          
      RL = XLIN                                                         
      RP = XELE

C IF DOING SOUNDER NAV, HAVE TO TRICK ROUTINES INTO THINKING IMAGE IS
C AT RES 1, BECAUSE NAV ROUTINES TAKE SOUNDER RES INTO ACCOUNT
      IF (INSTR.EQ.2) THEN
        RL = (RL+9.)/10.
        RP = (RP+9.)/10.
      ENDIF
                                                                        
C COMPUTE ELEVATION AND SCAN ANGLES (E,S) RELATED TO INPUT              
C LINE AND PIXEL NUMBERS                                                
      E = EVLN(INSTR,RL)                                                
      S = SCPX(INSTR,RP)                                                
                                                                        
C TRANSFORM ELEVATION AND SCAN ANGLES TO GEOGRAPHIC COORDINATES         
      CALL LPOINT(E,S,TMPLAT,TMPLON,STAT)                                   
      IF (STAT.NE.0) GOTO 900                                           
      TMPLAT = TMPLAT * DEG                                                 
      TMPLON = TMPLON * DEG                                                 
                                                                        
C PUT LONGITUDE INTO MCIDAS FORM                                        
      TMPLON = -TMPLON                                                      
                                                                        
C SEE IF WE HAVE TO CONVERT TO X Y Z COORDINATES                        
      IF (ITYPE.EQ.2) THEN                                              
        YLAT = TMPLAT                                                     
        YLON = TMPLON                                                     
        CALL LLCART(YLAT,YLON,XLAT,XLON,Z)                              
      ELSE
	XLAT = TMPLAT
	XLON = TMPLON
      ENDIF                                                             
                                                                        
      NVXSAE=0                                                          
      RETURN                                                            
                                                                        
 900  CONTINUE                                                          
      XLAT = TMPLAT
      XLON = -TMPLON
      NVXSAE=-1                                                         
      RETURN                                                            
      END                                                               
                                                                        
                                                                        
                                                                        
                                                                        
      INTEGER FUNCTION NVXEAS(ZLAT,ZLON,Z,XLIN,XELE,XDUM)               
                                                                        
      IMPLICIT NONE
                                                                        
      REAL         ZLAT  , ZLON  , Z     , XLIN  , XELE  , XDUM         
      REAL         X     , Y            
      REAL*8       TMPLIN, TMPELE, E     , S     , TMPLAT, TMPLON
      REAL*8       SUBLAT, SUBLON
      INTEGER      INSTR , ITYPE , IER                                  
                                                                        
C     INCLUDE 'ELCONS.INC'
C=========================== DIELCONS =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCONS = INCLUDE                                                
                                                                        
      REAL*8 PI                                                         
           PARAMETER (PI=3.141592653589793D0)                           
      REAL*8 DEG                                                        
           PARAMETER (DEG=180.D0/PI)                                    
      REAL*8 RAD                                                        
           PARAMETER (RAD=PI/180.D0)                                    
C                    DEGREES TO RADIANS CONVERSION PI/180               
      REAL*8 NOMORB                                                     
           PARAMETER (NOMORB=42164.365D0)                               
C                    NOMINAL RADIAL DISTANCE OF SATELLITE (KM)          
      REAL*8 AE                                                         
           PARAMETER (AE=6378.137D0)                                    
C                    EARTH EQUATORIAL RADIUS (KM)                       
      REAL*8 FER                                                        
           PARAMETER (FER=1.D0-(6356.7533D0/AE))                        
C                    EARTH FLATTENING COEFFICIENT = 1-(BE/AE)           
      REAL*4 AEBE2                                                      
           PARAMETER (AEBE2=1.D0/(1.D0-FER)**2)                         
      REAL*4 AEBE3                                                      
           PARAMETER (AEBE3=AEBE2-1.)                                   
      REAL*4 AEBE4                                                      
           PARAMETER (AEBE4=(1.D0-FER)**4-1.)                           
C========================= END DIELCONS ===========================     
C     INCLUDE 'ELCOMM.INC'
C=========================== DIELCOMM =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCOMM = INCLUDE                                                
                                                                        
C     COMMON VARIABLES                                                  
C                                                                       
      REAL*8 XS(3)                                                      
C                      NORMALIZED S/C POSITION IN ECEF COORDINATES      
      REAL*8 BT(3,3)                                                    
C                      ECEF TO INSTRUMENT COORDINATES TRANSFORMATION    
      REAL*8  Q3                                                        
C                      USED IN SUBRTN LPOINT                        
      REAL*8 PITCH,ROLL,YAW                                             
C                          PITCH,ROLL,YAW ANGLES OF INSTRUMENT (RAD)    
      REAL*4 PMA,RMA                                                    
C                          PITCH,ROLL MISALIGNMENTS OF INSTRUMENT (RAD) 
         COMMON /ELCOMM/ XS,BT,Q3,PITCH,ROLL,YAW,PMA,RMA                
C========================= END DIELCOMM ===========================     
C     INCLUDE 'OASET.INC'
C=========================== DIOASET =============================      
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE GVAR NAVIGATION SUBSYSTEM.       
C $                                                                     
C $   DESCRIPTION:                                                      
C $      THESE ARE CONSTANTS USED TO DESCRIBE THE GVAR O&A SET STRUCTURE
C $                                                                     
C $$  DIOASET = INCLUDE, NAVIGATION, GVAR                               
                                                                        
C DEFINITIONS OF PARAMETERS                                             
C OASIZE   : SIZE OF GVAR ORBIT AND ATTITUDE SET                        
C PCOEFS   : START OF PITCH COEFFICIENTS                                
C RMACFS   : START OF RMA COEFFICIENTS                                  
C CUTOF1   : FIRST DIVIDING POINT IN O&A SET (WHERE DIVIDED INTO 128    
C            WORD SETS)                                                 
C CUTOF2   : SECOND DIVIDING POINT IN O&A SET (WHERE DIVIDED INTO 128   
C            WORD SETS)                                                 
                                                                        
      INTEGER      OASIZE, PCOEFS, RMACFS, CUTOF1, CUTOF2               
                                                                        
      PARAMETER (OASIZE = 336)                                          
      PARAMETER (PCOEFS = 117)                                          
      PARAMETER (RMACFS = 227)                                          
      PARAMETER (CUTOF1 = 116)                                          
      PARAMETER (CUTOF2 = 226)                                          
                                                                        
C========================= END DIOASET ===========================      
C     INCLUDE 'INSTCO.INC'
C=========================== DIINSTCO =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIINSTCO = INCLUDE                                                
                                                                        
      INTEGER*4 INCMAX(2)                                               
C                       NUMBER OF INCREMENTS PER CYCLE                  
      REAL*4 ELVMAX(2)                                                  
C                       BOUNDS IN ELEVATION (RADIANS)                   
      REAL*4 SCNMAX(2)                                                  
C                       BOUNDS IN SCAN ANGLE (RADIANS)                  
      REAL*4 ELVINC(2)                                                  
C                       CHANGE IN ELEVATION ANGLE PER INCREMENT (RAD)   
      REAL*4 SCNINC(2)                                                  
C                       CHANGE IN SCAN ANGLE PER INCREMENT (RADIANS)    
      REAL*4 ELVLN(2)                                                   
C                       ELEVATION ANGLE PER DETECTOR LINE (RADIANS)     
      REAL*4 SCNPX(2)                                                   
C                       SCAN ANGLE PER PIXEL (RADIANS)                  
                                                                        
      COMMON /INSTCO/ INCMAX,ELVMAX,SCNMAX,                             
     1   ELVINC,SCNINC,ELVLN,SCNPX                                      
C========================= END DIINSTCO ===========================     
                                                                        
      COMMON /GVRCOM/ ITYPE, INSTR, SUBLAT, SUBLON                      
                                                                        
C--------------------------------------------------------------         
                                                                        
      NVXEAS = 0                                                        
                                                                        
C IF IN CARTESIAN COORDS, TRANSFORM TO LAT/LON                          
      IF (ITYPE.EQ.2) THEN                                              
        X = ZLAT                                                        
        Y = ZLON                                                        
        CALL CARTLL(X,Y,Z,ZLAT,ZLON)                                    
      ENDIF                                                             
                                                                        
      IF (ABS(ZLAT).GT.90.) THEN                                        
        NVXEAS = -1                                                     
        RETURN                                                          
      ENDIF                                                             

C ELUG ROUTINES USE REAL*8 VARS; USE TEMP VARIABLES 
      TMPLAT = ZLAT 
      TMPLON = ZLON 
                                                                        
C TRANSFORM LAT/LON TO ELEVATION AND SCAN ANGLES                        
      CALL GPOINT(TMPLAT*RAD,-TMPLON*RAD,E,S,IER)
                                                                        
C SEE IF THE LAT/LON GIVEN IS ABLE TO BE SEEN                           
      IF (IER.NE.0) THEN                                                
        NVXEAS = -1                                                     
        RETURN                                                          
      ENDIF                                                             
                                                                        
C CONVERT ELEVATION AND SCAN ANGLES TO LINE/PIXEL COORDINATES           
      CALL EVSC2L(INSTR,E,S,TMPLIN,TMPELE)

C CONVERT INTERNAL 8 BYTE VALUES TO 4 BYTES 
      XLIN = TMPLIN 
      XELE = TMPELE

C IF DOING SOUNDER NAV, CHANGE LIN & ELE RETURNED TO RES 10 VALUES
      IF (INSTR.EQ.2) THEN
        XLIN = XLIN*10.-9.
        XELE = XELE*10.-9.
      ENDIF
                                                                        
      RETURN                                                            
      END                                                               
                                                                        
                                                                        
                                                                        
                                                                        
      INTEGER FUNCTION NVXOPT(IFUNC,XIN,XOUT)                           
                                                                        
      IMPLICIT NONE
                                                                        
C     INCLUDE 'ELCONS.INC'
C=========================== DIELCONS =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCONS = INCLUDE                                                
                                                                        
      REAL*8 PI                                                         
           PARAMETER (PI=3.141592653589793D0)                           
      REAL*8 DEG                                                        
           PARAMETER (DEG=180.D0/PI)                                    
      REAL*8 RAD                                                        
           PARAMETER (RAD=PI/180.D0)                                    
C                    DEGREES TO RADIANS CONVERSION PI/180               
      REAL*8 NOMORB                                                     
           PARAMETER (NOMORB=42164.365D0)                               
C                    NOMINAL RADIAL DISTANCE OF SATELLITE (KM)          
      REAL*8 AE                                                         
           PARAMETER (AE=6378.137D0)                                    
C                    EARTH EQUATORIAL RADIUS (KM)                       
      REAL*8 FER                                                        
           PARAMETER (FER=1.D0-(6356.7533D0/AE))                        
C                    EARTH FLATTENING COEFFICIENT = 1-(BE/AE)           
      REAL*4 AEBE2                                                      
           PARAMETER (AEBE2=1.D0/(1.D0-FER)**2)                         
      REAL*4 AEBE3                                                      
           PARAMETER (AEBE3=AEBE2-1.)                                   
      REAL*4 AEBE4                                                      
           PARAMETER (AEBE4=(1.D0-FER)**4-1.)                           
C========================= END DIELCONS ===========================     
C     INCLUDE 'ELCOMM.INC'
C=========================== DIELCOMM =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCOMM = INCLUDE                                                
                                                                        
C     COMMON VARIABLES                                                  
C                                                                       
      REAL*8 XS(3)                                                      
C                      NORMALIZED S/C POSITION IN ECEF COORDINATES      
      REAL*8 BT(3,3)                                                    
C                      ECEF TO INSTRUMENT COORDINATES TRANSFORMATION    
      REAL*8  Q3                                                        
C                      USED IN SUBRTN LPOINT                        
      REAL*8 PITCH,ROLL,YAW                                             
C                          PITCH,ROLL,YAW ANGLES OF INSTRUMENT (RAD)    
      REAL*4 PMA,RMA                                                    
C                          PITCH,ROLL MISALIGNMENTS OF INSTRUMENT (RAD) 
         COMMON /ELCOMM/ XS,BT,Q3,PITCH,ROLL,YAW,PMA,RMA                
C========================= END DIELCOMM ===========================     
                                                                        
      INTEGER      IFUNC , LIT   , ITYPE , INSTR , JDAY  , JTIME        
      INTEGER      IROUND, ITIME , LASDAY, LASTIM
      REAL         FLAT  , FLON  , GHA   , XIN(*)       
      REAL         DEC   , XLAT  , XLON  , XOUT(*)              
      REAL*8       AEC   , FERC  , AEBE2C, AEBE3C, AEBE4C, B(3,3)       
      REAL*8       SUBLAT, SUBLON, R     , DR    , PHI
                                                                        
      COMMON /GVRCOM/ ITYPE , INSTR , SUBLAT, SUBLON                    
      COMMON /RADCOM/ AEC   , FERC  , AEBE2C, AEBE3C, AEBE4C            
      COMMON /SAVCOM/ B     , DR    , PHI                               
                                                                        
      DATA LASDAY/-1/,LASTIM/-1/                                        
                                                                        
C-----------------------------------------------------------------      
                                                                        
      NVXOPT = -1                                                       
                                                                        
      IF (IFUNC.EQ.LIT('SPOS')) THEN                                    
        XOUT(1) = SUBLAT * DEG                                          
        XOUT(2) = -SUBLON * DEG                                         
        XOUT(2) = AMOD(XOUT(2),360.)                                    
        NVXOPT = 0                                                      
                                                                        
      ELSEIF (IFUNC.EQ.LIT('HGT ')) THEN                                
        AEC    = AE + DBLE(XIN(1))                                      
        FERC   = 1.D0 - ((6356.7533D0 + DBLE(XIN(1))) / AEC)            
        AEBE2C = 1.D0 / ((1.D0 - FERC)*(1.D0 - FERC))                   
        AEBE3C = AEBE2C - 1.D0                                          
        AEBE4C = (1.D0-FERC)*(1.D0-FERC)*(1.D0-FERC)*(1.D0-FERC) - 1.D0 
                                                                        
C RECOMPUTE VALUES THAT DEPEND ON EARTH RADIUS                          
        SUBLAT = DATAN(AEBE2C*DTAN(PHI))
        R      = (NOMORB+DR)/AEC                                        
        XS(1)  = -B(1,3)*R                                              
        XS(2)  = -B(2,3)*R                                              
        XS(3)  = -B(3,3)*R                                              
        Q3     = XS(1)**2 + XS(2)**2 + AEBE2C * XS(3)**2 - 1.0          
        NVXOPT = 0                                                      
                                                                        
      ELSEIF (IFUNC.EQ.LIT('ANG ')) THEN                                
        JDAY  = IROUND(XIN(1))                                          
        JTIME = ITIME(XIN(2))                                           
        FLAT  = XIN(3)                                                  
        FLON  = XIN(4)                                                  
        IF (JDAY.NE.LASDAY.OR.JTIME.NE.LASTIM) THEN                     
          CALL SOLARP(JDAY,JTIME,GHA,DEC,XLAT,XLON)                     
          LASDAY = JDAY                                                 
          LASTIM = JTIME
        ENDIF
        CALL GVRANG(JDAY, JTIME, FLAT, FLON, GHA, DEC, XOUT(1), XOUT(2),
     &                                                          XOUT(3))
        NVXOPT = 0                                                      
      ENDIF                                                             
                                                                        
      RETURN                                                            
      END                                                               
                                                                        
                                                                        
                                                                        
                                                                        
                                                                        
C THE FOLLOWING ARE SUBSIDIARY ROUTINES                                 
                                                                        
                                                                        
      SUBROUTINE SETCON(INSTR, NADNSC, NADNSI, NADEWC, NADEWI)
                                                                        
      IMPLICIT NONE

      INTEGER INSTR, NADNSC, NADNSI, NADEWC, NADEWI
      character*12 cfg
                                                                        
C     INCLUDE 'ELCONS.INC'
C=========================== DIELCONS =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCONS = INCLUDE                                                
                                                                        
      REAL*8 PI                                                         
           PARAMETER (PI=3.141592653589793D0)                           
      REAL*8 DEG                                                        
           PARAMETER (DEG=180.D0/PI)                                    
      REAL*8 RAD                                                        
           PARAMETER (RAD=PI/180.D0)                                    
C                    DEGREES TO RADIANS CONVERSION PI/180               
      REAL*8 NOMORB                                                     
           PARAMETER (NOMORB=42164.365D0)                               
C                    NOMINAL RADIAL DISTANCE OF SATELLITE (KM)          
      REAL*8 AE                                                         
           PARAMETER (AE=6378.137D0)                                    
C                    EARTH EQUATORIAL RADIUS (KM)                       
      REAL*8 FER                                                        
           PARAMETER (FER=1.D0-(6356.7533D0/AE))                        
C                    EARTH FLATTENING COEFFICIENT = 1-(BE/AE)           
      REAL*4 AEBE2                                                      
           PARAMETER (AEBE2=1.D0/(1.D0-FER)**2)                         
      REAL*4 AEBE3                                                      
           PARAMETER (AEBE3=AEBE2-1.)                                   
      REAL*4 AEBE4                                                      
           PARAMETER (AEBE4=(1.D0-FER)**4-1.)                           
C========================= END DIELCONS ===========================     
C     INCLUDE 'INSTCO.INC'
C=========================== DIINSTCO =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIINSTCO = INCLUDE                                                
                                                                        
      INTEGER*4 INCMAX(2)                                               
C                       NUMBER OF INCREMENTS PER CYCLE                  
      REAL*4 ELVMAX(2)                                                  
C                       BOUNDS IN ELEVATION (RADIANS)                   
      REAL*4 SCNMAX(2)                                                  
C                       BOUNDS IN SCAN ANGLE (RADIANS)                  
      REAL*4 ELVINC(2)                                                  
C                       CHANGE IN ELEVATION ANGLE PER INCREMENT (RAD)   
      REAL*4 SCNINC(2)                                                  
C                       CHANGE IN SCAN ANGLE PER INCREMENT (RADIANS)    
      REAL*4 ELVLN(2)                                                   
C                       ELEVATION ANGLE PER DETECTOR LINE (RADIANS)     
      REAL*4 SCNPX(2)                                                   
C                       SCAN ANGLE PER PIXEL (RADIANS)                  
                                                                        
      COMMON /INSTCO/ INCMAX,ELVMAX,SCNMAX,                             
     1   ELVINC,SCNINC,ELVLN,SCNPX                                      
C========================= END DIINSTCO ===========================     
                                                                        
      INCMAX(1)  = 6136                                                 
      INCMAX(2)  = 2805                                                 
      ELVINC(1)  = 8.E-6                                                
      ELVINC(2)  = 17.5E-6                                              
      SCNINC(1)  = 16.E-6                                               
      SCNINC(2)  = 35.E-6                                               
      ELVLN(1)   = 28.E-6                                               
      ELVLN(2)   = 280.E-6                                              
      SCNPX(1)   = 16.E-6                                               
      SCNPX(2)   = 280.E-6                                              
      ELVMAX(1)  = 0.220896                                             
      ELVMAX(2)  = 0.22089375                                           
      SCNMAX(1)  = 0.24544                                              
      SCNMAX(2)  = 0.2454375                                            

C new code because of change to elug; nadir position is available
C in the signal, so should compute 4 values above using them 
C instead of having them hardwired
      call ddest('nadnsc: ', nadnsc)
      call ddest('nadnsi: ', nadnsi)
      call ddest('nadewc: ', nadewc)
      call ddest('nadewi: ', nadewi)
c
c--- new code from Kathy Kelly for sounder nav - 10/27
c
      if (nadnsc .ne. 0 .and. nadnsi .ne. 0 .and. nadewc .ne. 0
     &                                 .and. nadewi .ne. 0) then
	IF (INSTR .EQ. 1) THEN

           ELVMAX(INSTR) = (NADNSC*INCMAX(INSTR)+NADNSI)
     &                                             *ELVINC(INSTR)

        ELSE

           ELVMAX(INSTR) = ((9-NADNSC)*INCMAX(INSTR)-NADNSI)
     &                                             *ELVINC(INSTR)

	ENDIF

        SCNMAX(INSTR) = (NADEWC*INCMAX(INSTR)+NADEWI)
     &                                            *SCNINC(INSTR)
      endif
c
c--- end of new code from Kathy Kelly for sounder nav - 10/27
c

      call ddest('elvmax(1): '//cfg(elvmax(1)), 0)
      call ddest('elvmax(2): '//cfg(elvmax(2)), 0)
      call ddest('scnmax(1): '//cfg(scnmax(1)), 0)
      call ddest('scnmax(2): '//cfg(scnmax(2)), 0)
                                                                        
      RETURN                                                            
      END                                                               
                                                                        
                                                                        
                                                                        
      FUNCTION TIMEX(NY,ND,NH,NM,S)                                     
                                                                        
      IMPLICIT NONE
                                                                        
      INTEGER      ND    , NH    , NM    , J     , NY                   
      REAL*8       TIMEX , S                                            
                                                                        
C-----------------------------------------------------------------------
                                                                        
      J = ND + 1461 * (NY + 4799) / 4 - 3 * ((NY + 4899) / 100) / 4     
     &                                                        - 2465022 
                                                                        
C COMPUTE ACTUAL TIME IN MINUTES FROM JANUARY 1.0, 1950                 
      TIMEX = J * 1440.D0 + NH * 60.D0 + NM + S / 60.D0                 
                                                                        
      RETURN                                                            
      END                                                               
                                                                        
                                                                        
                                                                        
      FUNCTION TIME50(I)                                                
                                                                        
      IMPLICIT NONE
                                                                        
      INTEGER      NY    , ND    , NH    , NM    , J     , I(2)         
      INTEGER      IAA   , IAB   , NBC   , IAC   , DEF                  
      REAL*8       TIME50, S                                            
                                                                        
C-----------------------------------------------------------------      
                                                                        
      NY  = I(1) / 10000                                                
      IAA = I(1) - (NY*10000)                                           
                                                                        
      ND  = (I(1) - (NY*10000)) * .1                                    
      IAB = (IAA - (ND * 10)) * 10                                      
      NBC = I(2) / 10000000.                                            
      IAC = I(2) - (NBC * 10000000)                                     
      NH  = IAB + NBC                                                   
      DEF = I(2) - IAC                                                  
      NM  = IAC * .00001                                                
      S   = (I(2) - (DEF + (NM * 100000))) * .001                       
                                                                        
C HERE WE CONVERT INTEGER YEAR AND DAY OF YEAR TO NUMBER OF             
C DAYS FROM 0 HOUR UT, 1950 JAN. 1.0                                    
C THIS CONVERTION IS BASED ON AN ALGORITHM BY FLIEGEL AND VAN           
C FLANDERN, COMM. OF ACM, VOL.11, NO. 10, OCT. 1968 (P.657)             
                                                                        
      J = ND + 1461 * (NY + 4799) / 4 - 3 * ((NY + 4899) / 100) / 4     
     &                                                       - 2465022  
                                                                        
C COMPUTE TIME IN MINUTES FROM JANUARY 1.0, 1950                        
      TIME50 = J * 1440.D0 + NH * 60.D0 + NM + S / 60.D0                
                                                                        
      RETURN                                                            
      END                                                               
                                                                        
                                                                        
                                                                        
                                                                        
      REAL*8 FUNCTION GATT(K0,RPARMS,IPARMS,WA,TE)                        
                                                                        
      IMPLICIT NONE

C     INCLUDE 'MXCDSZ.INC'
C=========================== DIMXCDSZ =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE NAVIGATION SUBSYSTEM             
C $                                                                     
C $   DESCRIPTION:                                                      
C $      THIS IS THE VALUE OF LARGEST NAVIGATION CODICIL SIZE ALLOWED   
C $                                                                     
C $$  DIMXCDSZ = INCLUDE, NAVIGATION                                    
                                                                        
C DEFINITIONS OF PARAMETERS                                             
C MXCDSZ   : LARGEST ALLOWED CODICIL SIZE                               
                                                                        
      INTEGER      MXCDSZ                                               
                                                                        
      PARAMETER (MXCDSZ = 5*128)                                        
                                                                        
C========================= END DIMXCDSZ ===========================     
                                                                        
C CALLING PARAMETERS                                                    
      INTEGER K0                                                        
C STARTING POSITION OF A PARAMETER SUBSET IN THE REAL O&A SET           
      REAL*4 RPARMS(MXCDSZ)
C INPUT O&A PARAMETER SET                                               
      INTEGER IPARMS(MXCDSZ)
C INPUT O&A PARAMETER SET                                               
      REAL*8 WA                                                           
C INPUT SOLAR ORBIT ANGLE IN RADIANS                                    
      REAL*8 TE                                                           
C INPUT EXPONENTIAL TIME DELAY FROM EPOCH (MINUTES)                     
                                                                        
C LOCAL VARIABLES                                                       
      INTEGER      KKK   , I     , J     , M     , L     , LL    , K    
      REAL*8       IR    , JR    , MR    , ATT                          
                                                                        
C------------------------------------------------------------------     
                                                                        
C CONSTANT COMPONENT                                                    
      K   = K0                                                          
      ATT = RPARMS(K+2)                                                 
                                                                        
C COMPUTES THE EXPONENTIAL TERM                                         
      IF (TE.GE.0) ATT = ATT + RPARMS(K) * DEXP(-TE / RPARMS(K+1))       
                                                                        
C EXTRACTS THE NUMBER OF SINUSOIDS                                      
      IR = REAL(IPARMS(K+3))
      I  = NINT(IR)                                                     
                                                                        
C CALCULATION OF SINUSOIDS                                              
      DO 10 L=1,I                                                       
        ATT = ATT + RPARMS(K+2*L+2) * DCOS(WA*L+RPARMS(K+2*L+3))         
 10   CONTINUE                                                          
                                                                        
C POINTER TO THE NUMBER OF MONOMIAL SINUSOIDS                           
      K = K + 34                                                        

C EXTACTS NUMBER OF MONOMIAL SINUSOIDS                                  
      IR  = REAL(IPARMS(K))                                             
      KKK = IPARMS(K)                                                   
                                                                        
C COMPUTES MONOMIAL SINUSOIDS                                           
      DO 20 L=1,KKK                                                     
        LL = K + 5 * L                                                  
                                                                        
C ORDER OF SINUSOID                                                     
        JR = REAL(IPARMS(LL-4))                                         
                                                                        
C ORDER OF MONOMIAL SINUSOID                                            
        MR  = REAL(IPARMS(LL-3))                                        
        J   = NINT(JR)                                                  
        M   = NINT(MR)                                                  
        ATT = ATT + RPARMS(LL-2) * ((WA - RPARMS(LL))**MR)              
     &                                      * DCOS(JR*WA+RPARMS(LL-1))  
 20   CONTINUE                                                          
                                                                        
      GATT = ATT                                                        
      RETURN                                                            
      END                                                               
                                                                        
                                                                        
                                                                        
                                                                        
      SUBROUTINE EVSC2L(INSTR,ELEV,SCAN,RL,RP)                          
                                                                        
      IMPLICIT NONE
                                                                        
C CALLING PARAMETERS                                                    
      INTEGER INSTR                                                     
C INSTRUMENT CODE (1-IMAGER, 2-SOUNDER)                                 
      REAL*8 ELEV                                                         
C ELEVATION ANGLE IN RADIANS                                            
      REAL*8 SCAN                                                         
C SCAN ANGLE IN RADIANS                                                 
      REAL*8 RL                                                           
C LINE NUMBER                                                           
      REAL*8 RP                                                           
C PIXEL NUMBER                                                          
                                                                        
C     INCLUDE 'INSTCO.INC'
C=========================== DIINSTCO =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIINSTCO = INCLUDE                                                
                                                                        
      INTEGER*4 INCMAX(2)                                               
C                       NUMBER OF INCREMENTS PER CYCLE                  
      REAL*4 ELVMAX(2)                                                  
C                       BOUNDS IN ELEVATION (RADIANS)                   
      REAL*4 SCNMAX(2)                                                  
C                       BOUNDS IN SCAN ANGLE (RADIANS)                  
      REAL*4 ELVINC(2)                                                  
C                       CHANGE IN ELEVATION ANGLE PER INCREMENT (RAD)   
      REAL*4 SCNINC(2)                                                  
C                       CHANGE IN SCAN ANGLE PER INCREMENT (RADIANS)    
      REAL*4 ELVLN(2)                                                   
C                       ELEVATION ANGLE PER DETECTOR LINE (RADIANS)     
      REAL*4 SCNPX(2)                                                   
C                       SCAN ANGLE PER PIXEL (RADIANS)                  
                                                                        
      COMMON /INSTCO/ INCMAX,ELVMAX,SCNMAX,                             
     1   ELVINC,SCNINC,ELVLN,SCNPX                                      
C========================= END DIINSTCO ===========================     
                                                                        
C--------------------------------------------------------------         
                                                                        
C COMPUTE FRACTIONAL LINE NUMBER                                        
      RL = (ELVMAX(INSTR) - ELEV) / ELVLN(INSTR)                        
      IF (INSTR.EQ.1) THEN                                              
        RL = RL + 4.5                                                   
      ELSE                                                              
        RL = RL + 2.5                                                   
      ENDIF                                                             
                                                                        
C COMPUTE FRACTIONAL PIXEL NUMBER                                       
      RP = (SCNMAX(INSTR) + SCAN) / SCNPX(INSTR) + 1.                   
                                                                        
      RETURN                                                            
      END                                                               
                                                                        
                                                                        
                                                                        
      REAL*8 FUNCTION EVLN(INSTR,RLINE)                                   
                                                                        
      IMPLICIT NONE
                                                                        
C CALLING PARAMETERS                                                    
      INTEGER INSTR                                                     
C INSTRUMENT CODE (1-IMAGER, 2-SOUNDER)                                 
      REAL*8  RLINE                                                     
C FRACTIONAL LINE  NUMBER                                               
                                                                        
C     INCLUDE 'INSTCO.INC'
C=========================== DIINSTCO =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIINSTCO = INCLUDE                                                
                                                                        
      INTEGER*4 INCMAX(2)                                               
C                       NUMBER OF INCREMENTS PER CYCLE                  
      REAL*4 ELVMAX(2)                                                  
C                       BOUNDS IN ELEVATION (RADIANS)                   
      REAL*4 SCNMAX(2)                                                  
C                       BOUNDS IN SCAN ANGLE (RADIANS)                  
      REAL*4 ELVINC(2)                                                  
C                       CHANGE IN ELEVATION ANGLE PER INCREMENT (RAD)   
      REAL*4 SCNINC(2)                                                  
C                       CHANGE IN SCAN ANGLE PER INCREMENT (RADIANS)    
      REAL*4 ELVLN(2)                                                   
C                       ELEVATION ANGLE PER DETECTOR LINE (RADIANS)     
      REAL*4 SCNPX(2)                                                   
C                       SCAN ANGLE PER PIXEL (RADIANS)                  
                                                                        
      COMMON /INSTCO/ INCMAX,ELVMAX,SCNMAX,                             
     1   ELVINC,SCNINC,ELVLN,SCNPX                                      
C========================= END DIINSTCO ===========================     
                                                                        
C----------------------------------------------------------------       
                                                                        
      IF (INSTR.EQ.1) THEN                                              
        EVLN = ELVMAX(INSTR) - (RLINE - 4.5) * ELVLN(INSTR)             
      ELSE                                                              
        EVLN = ELVMAX(INSTR) - (RLINE - 2.5) * ELVLN(INSTR)             
      ENDIF                                                             
                                                                        
      RETURN                                                            
      END                                                               
                                                                        
                                                                        
                                                                        
      REAL*8 FUNCTION SCPX(INSTR,PIX)                                     
                                                                        
      IMPLICIT NONE
                                                                        
C CALLING PARAMETERS                                                    
      INTEGER INSTR                                                     
C INSTRUMENT CODE (1-IMAGER, 2-SOUNDER)                                 
      REAL*8 PIX                                                          
C FRACTIONAL PIXEL NUMBER                                               
                                                                        
C     INCLUDE 'INSTCO.INC'
C=========================== DIINSTCO =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIINSTCO = INCLUDE                                                
                                                                        
      INTEGER*4 INCMAX(2)                                               
C                       NUMBER OF INCREMENTS PER CYCLE                  
      REAL*4 ELVMAX(2)                                                  
C                       BOUNDS IN ELEVATION (RADIANS)                   
      REAL*4 SCNMAX(2)                                                  
C                       BOUNDS IN SCAN ANGLE (RADIANS)                  
      REAL*4 ELVINC(2)                                                  
C                       CHANGE IN ELEVATION ANGLE PER INCREMENT (RAD)   
      REAL*4 SCNINC(2)                                                  
C                       CHANGE IN SCAN ANGLE PER INCREMENT (RADIANS)    
      REAL*4 ELVLN(2)                                                   
C                       ELEVATION ANGLE PER DETECTOR LINE (RADIANS)     
      REAL*4 SCNPX(2)                                                   
C                       SCAN ANGLE PER PIXEL (RADIANS)                  
                                                                        
      COMMON /INSTCO/ INCMAX,ELVMAX,SCNMAX,                             
     1   ELVINC,SCNINC,ELVLN,SCNPX                                      
C========================= END DIINSTCO ===========================     
                                                                        
C--------------------------------------------------------------------   
                                                                        
      SCPX = (PIX - 1.) * SCNPX(INSTR) - SCNMAX(INSTR)                  
                                                                        
      RETURN                                                            
      END                                                               
                                                                        
                                                                        
                                                                        
                                                                        
      SUBROUTINE INST2E(R,P,Y,A,AT)                                     
                                                                        
      IMPLICIT NONE
                                                                        
C CALLING PARAMETERS                                                    
      REAL*8 R                                                          
C ROLL ANGLE IN RADIANS                                                 
      REAL*8 P                                                          
C PITCH ANGLE IN RADIANS                                                
      REAL*8 Y                                                          
C YAW ANGLE IN RADIANS                                                  
      REAL*8 A(3,3)                                                     
C SPACECRAFT TO ECEF COORDINATES TRANSFORMATION MATRIX                  
      REAL*8 AT(3,3)                                                    
C INSTRUMENT TO ECEF COORDINATES TRANSFORMATION MATRIX                  
                                                                        
      REAL*8       RPY(3,3)                                             
      INTEGER*4    I     , J                                            
                                                                        
C-----------------------------------------------------------------------
                                                                        
C WE COMPUTE INSTRUMENT TO BODY COORDINATES TRANSFORMATION              
C MATRIX BY USING A SMALL ANGLE APPROXIMATION OF TRIGONOMETRIC          
C FUNKTIONS OF THE ROLL, PITCH AND YAW.                                 
      RPY(1,1) = 1. - 0.5 * (P * P + Y * Y)                             
      RPY(1,2) = -Y                                                     
      RPY(1,3) = P                                                      
      RPY(2,1) = Y + P * R                                              
      RPY(2,2) = 1. - 0.5 * (Y * Y + R * R)                             
      RPY(2,3) = -R                                                     
      RPY(3,1) = -P + R * Y                                             
      RPY(3,2) = R + P * Y                                              
      RPY(3,3) = 1. - 0.5 * (P * P + R * R)                             
                                                                        
C MULTIPLICATION OF MATRICES A AND RPY                                  
      DO 20 I=1,3                                                       
        DO 10 J=1,3                                                     
          AT(I,J) = A(I,1) * RPY(1,J) + A(I,2) * RPY(2,J) + A(I,3)      
     &                                                    * RPY(3,J)    
 10     CONTINUE                                                        
 20   CONTINUE                                                          
                                                                        
      RETURN                                                            
      END                                                               
                                                                        
                                                                        
                                                                        
                                                                        
      SUBROUTINE LPOINT(ALPHA,ZETA,RLAT,RLON,IERR)                      
                                                                        
      IMPLICIT NONE
                                                                        
C CALLING PARAMETERS                                                    
      REAL*8   ALPHA                                                    
C ELEVATION ANGLE (RAD)                                                 
      REAL*8   ZETA                                                     
C SCAN ANGLE (RAD)                                                      
      REAL*8   RLAT                                                     
C LATITUDE IN RADIANS (OUTPUT)                                          
      REAL*8   RLON                                                     
C LONGITUDE IN RADIANS (OUTPUT)                                         
      INTEGER IERR                                                      
C OUTPUT STATUS; 0 - POINT ON THE EARTH FOUND,                          
C                1 - INSTRUMENT POINTS OFF EARTH                        
                                                                        
      REAL*8       AEC   , FERC  , Q1    , Q2    , D     , H     , G1(3)
      REAL*8       SA    , CA    , DA    , DZ    , D1    , CZ    , G(3) 
      REAL*8       U(3)                                                 
      REAL*8       AEBE2C, AEBE3C, AEBE4C                               
                                                                        
C     INCLUDE 'ELCONS.INC'
C=========================== DIELCONS =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCONS = INCLUDE                                                
                                                                        
      REAL*8 PI                                                         
           PARAMETER (PI=3.141592653589793D0)                           
      REAL*8 DEG                                                        
           PARAMETER (DEG=180.D0/PI)                                    
      REAL*8 RAD                                                        
           PARAMETER (RAD=PI/180.D0)                                    
C                    DEGREES TO RADIANS CONVERSION PI/180               
      REAL*8 NOMORB                                                     
           PARAMETER (NOMORB=42164.365D0)                               
C                    NOMINAL RADIAL DISTANCE OF SATELLITE (KM)          
      REAL*8 AE                                                         
           PARAMETER (AE=6378.137D0)                                    
C                    EARTH EQUATORIAL RADIUS (KM)                       
      REAL*8 FER                                                        
           PARAMETER (FER=1.D0-(6356.7533D0/AE))                        
C                    EARTH FLATTENING COEFFICIENT = 1-(BE/AE)           
      REAL*4 AEBE2                                                      
           PARAMETER (AEBE2=1.D0/(1.D0-FER)**2)                         
      REAL*4 AEBE3                                                      
           PARAMETER (AEBE3=AEBE2-1.)                                   
      REAL*4 AEBE4                                                      
           PARAMETER (AEBE4=(1.D0-FER)**4-1.)                           
C========================= END DIELCONS ===========================     
C     INCLUDE 'ELCOMM.INC'
C=========================== DIELCOMM =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCOMM = INCLUDE                                                
                                                                        
C     COMMON VARIABLES                                                  
C                                                                       
      REAL*8 XS(3)                                                      
C                      NORMALIZED S/C POSITION IN ECEF COORDINATES      
      REAL*8 BT(3,3)                                                    
C                      ECEF TO INSTRUMENT COORDINATES TRANSFORMATION    
      REAL*8  Q3                                                        
C                      USED IN SUBRTN LPOINT                        
      REAL*8 PITCH,ROLL,YAW                                             
C                          PITCH,ROLL,YAW ANGLES OF INSTRUMENT (RAD)    
      REAL*4 PMA,RMA                                                    
C                          PITCH,ROLL MISALIGNMENTS OF INSTRUMENT (RAD) 
         COMMON /ELCOMM/ XS,BT,Q3,PITCH,ROLL,YAW,PMA,RMA                
C========================= END DIELCOMM ===========================     
                                                                        
      COMMON /RADCOM/ AEC   , FERC  , AEBE2C, AEBE3C, AEBE4C            
                                                                        
C-----------------------------------------------------------------------
                                                                        
      IERR=1                                                            
                                                                        
C COMPUTES TRIGONOMETRIC FUNKTIONS OF THE SCAN AND ELEVATION            
C ANGLES CORRECTED FOR THE ROLL AND PITCH MISALIGNMENTS                 
      CA = DCOS(ALPHA)                                                   
      SA = DSIN(ALPHA)                                                   
      CZ = DCOS(ZETA)
      DA = ALPHA-PMA*SA*(1.0D0/CZ+DTAN(ZETA))-RMA*(1.0D0-CA/CZ)        
      DZ = ZETA + RMA * SA                                              
                                                                        
C CORRECTED SCAN ANGLE                                                  
      CZ = DCOS(DZ)                                                      
                                                                        
C COMPUTES POINTING VECTOR IN INSTRUMENT COORDINATES                    
      G(1) = DSIN(DZ)                                                    
      G(2) = -CZ * DSIN(DA)                                              
      G(3) = CZ * DCOS(DA)                                               
                                                                        
C TRANSFORMS THE POINTING VECTOR TO EARTH FIXED COORDINATES             
      G1(1) = BT(1,1) * G(1) + BT(1,2) * G(2) + BT(1,3) * G(3)          
      G1(2) = BT(2,1) * G(1) + BT(2,2) * G(2) + BT(2,3) * G(3)          
      G1(3) = BT(3,1) * G(1) + BT(3,2) * G(2) + BT(3,3) * G(3)          
                                                                        
C COMPUTES COEFFICIENTS AND SOLVES A QUADRATIC EQUATION TO              
C FIND THE INTERSECT OF THE POINTING VECTOR WITH THE EARTH              
C SURFACE                                                               
      Q1 = G1(1)**2 + G1(2)**2 + AEBE2C * G1(3)**2                      
      Q2 = XS(1) * G1(1) + XS(2) * G1(2) + AEBE2C * XS(3) * G1(3)       
      D  = Q2 * Q2 - Q1 * Q3                                            
      IF (DABS(D).LT.1.D-9) D=0.                                        
                                                                        
C IF THE DISCRIMINANT OF THE EQUATION, D, IS NEGATIVE, THE              
C INSTRUMENT POINTS OFF THE EARTH                                       
      IF (D.LT.0) THEN                                                  
        RLAT = 999999.                                                  
        RLON = 999999.                                                  
        RETURN                                                          
      ENDIF                                                             
      D = DSQRT(D)                                                      
                                                                        
C SLANT DISTANCE FROM THE SATELLITE TO THE EARTH POINT                  
      H = -(Q2 + D) / Q1                                                
                                                                        
C CARTESIAN COORDINATES OF THE EARTH POINT                              
      U(1) = XS(1) + H * G1(1)                                          
      U(2) = XS(2) + H * G1(2)                                          
      U(3) = XS(3) + H * G1(3)                                          
                                                                        
C SINUS OF GEOCENTRIC LATITUDE                                          
      D1 = U(3) / DSQRT(U(1)**2 + U(2)**2 + U(3)**2)                     
                                                                        
C GEOGRAPHIC (GEODETIC) COORDINATES OF THE POINT                        
      RLAT = DATAN(AEBE2C * D1 / DSQRT(1. - D1 * D1))                     
      RLON = DATAN2(U(2),U(1))                                           
      IERR = 0                                                          
                                                                        
      RETURN                                                            
      END                                                               
                                                                        
                                                                        
                                                                        
                                                                        
      SUBROUTINE GPOINT(RLAT,RLON,ALF,GAM,IERR)                         
                                                                        
      IMPLICIT NONE
                                                                        
C CALLING PARAMETERS                                                    
      REAL*8   RLAT
C GEOGRAPHIC LATITUDE IN RADIANS (INPUT)                                
      REAL*8   RLON
C GEOGRAPHIC LONGITUDE IN RADIANS (INPUT)                               
      REAL*8   ALF
C ELEVATION ANGLE IN RADIANS (OUTPUT)                                   
      REAL*8   GAM
C SCAN ANGLE IN RADIANS (OUTPUT)                                        
      INTEGER IERR                                                      
C OUTPUT STATUS; 0 - SUCCESSFUL COMPLETION,                             
C                1 - POINT WITH GIVEN LAT/LON IS INVISIBLE              
                                                                        
      REAL*8       F(3)  , AEC   , FERC  , AEBE2C, AEBE3C, AEBE4C
      REAL*8       FT(3) , U(3)  , SING  , SLAT  , W1    , W2
                                                                        
C     INCLUDE 'ELCONS.INC'
C=========================== DIELCONS =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCONS = INCLUDE                                                
                                                                        
      REAL*8 PI                                                         
           PARAMETER (PI=3.141592653589793D0)                           
      REAL*8 DEG                                                        
           PARAMETER (DEG=180.D0/PI)                                    
      REAL*8 RAD                                                        
           PARAMETER (RAD=PI/180.D0)                                    
C                    DEGREES TO RADIANS CONVERSION PI/180               
      REAL*8 NOMORB                                                     
           PARAMETER (NOMORB=42164.365D0)                               
C                    NOMINAL RADIAL DISTANCE OF SATELLITE (KM)          
      REAL*8 AE                                                         
           PARAMETER (AE=6378.137D0)                                    
C                    EARTH EQUATORIAL RADIUS (KM)                       
      REAL*8 FER                                                        
           PARAMETER (FER=1.D0-(6356.7533D0/AE))                        
C                    EARTH FLATTENING COEFFICIENT = 1-(BE/AE)           
      REAL*4 AEBE2                                                      
           PARAMETER (AEBE2=1.D0/(1.D0-FER)**2)                         
      REAL*4 AEBE3                                                      
           PARAMETER (AEBE3=AEBE2-1.)                                   
      REAL*4 AEBE4                                                      
           PARAMETER (AEBE4=(1.D0-FER)**4-1.)                           
C========================= END DIELCONS ===========================     
C     INCLUDE 'ELCOMM.INC'
C=========================== DIELCOMM =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCOMM = INCLUDE                                                
                                                                        
C     COMMON VARIABLES                                                  
C                                                                       
      REAL*8 XS(3)                                                      
C                      NORMALIZED S/C POSITION IN ECEF COORDINATES      
      REAL*8 BT(3,3)                                                    
C                      ECEF TO INSTRUMENT COORDINATES TRANSFORMATION    
      REAL*8  Q3                                                        
C                      USED IN SUBRTN LPOINT                        
      REAL*8 PITCH,ROLL,YAW                                             
C                          PITCH,ROLL,YAW ANGLES OF INSTRUMENT (RAD)    
      REAL*4 PMA,RMA                                                    
C                          PITCH,ROLL MISALIGNMENTS OF INSTRUMENT (RAD) 
         COMMON /ELCOMM/ XS,BT,Q3,PITCH,ROLL,YAW,PMA,RMA                
C========================= END DIELCOMM ===========================     
                                                                        
      COMMON /RADCOM/ AEC   , FERC  , AEBE2C, AEBE3C, AEBE4C            
                                                                        
C-----------------------------------------------------------------------
                                                                        
C COMPUTES SINUS OF GEOGRAPHIC (GEODETIC) LATITUDE                      
      SING = DSIN(RLAT)                                                  
      W1   = AEBE4C * SING * SING                                       
                                                                        
C SINUS OF THE GEOCENTRIC LATITUDE                                      
      SLAT = ((0.375 * W1 - 0.5) * W1 + 1.) * SING / AEBE2C             
                                                                        
C COMPUTES LOCAL EARTH RADIUS AT SPECIFIED POINT                        
      W2 = SLAT * SLAT                                                  
      W1 = AEBE3C * W2                                                  
      W1 = (0.375 * W1 - 0.5) * W1 + 1.                                 
                                                                        
C COMPUTES CARTESIAN COORDINATES OF THE POINT                           
      U(3) = SLAT * W1                                                  
      W2   = W1 * DSQRT(1. - W2)                                         
      U(1) = W2 * DCOS(RLON)                                             
      U(2) = W2 * DSIN(RLON)                                             
                                                                        
C POINTING VECTOR FROM SATELLITE TO THE EARTH POINT                     
      F(1) = U(1) - XS(1)                                               
      F(2) = U(2) - XS(2)                                               
      F(3) = U(3) - XS(3)                                               
      W2   = U(1) * SNGL(F(1)) + U(2) * SNGL(F(2))+ U(3) * SNGL(F(3))   
     &                                                   * AEBE2C       
                                                                        
C VERIFIES VISIBILITY OF THE POINT                                      
      IF (W2.GT.0.) THEN                                                
                                                                        
C INVISIBLE POINT ON THE EARTH                                          
        IERR = 1                                                        
        ALF  = 99999.                                                   
        GAM  = 99999.                                                   
        RETURN                                                          
      ENDIF                                                             
                                                                        
C CONVERTS POINTING VECTOR TO INSTRUMENT COORDINATES                    
      FT(1) = BT(1,1) * F(1) + BT(2,1) * F(2) + BT(3,1) * F(3)          
      FT(2) = BT(1,2) * F(1) + BT(2,2) * F(2) + BT(3,2) * F(3)          
      FT(3) = BT(1,3) * F(1) + BT(2,3) * F(2) + BT(3,3) * F(3)          
                                                                        
C CONVERTS POINTING VECTOR TO SCAN AND ELEVATION ANGLES AND             
C CORRECTS FOR THE ROLL AND PITCH MISALIGNMENTS                         
      GAM  = DATAN(FT(1) / DSQRT(FT(2)**2 + FT(3)**2))                    
      ALF  = -DATAN(FT(2) / FT(3))                                       
      W1   = DSIN(ALF)                                                   
      W2   = DCOS(GAM)                                                   
      ALF  = ALF + RMA * (1. - DCOS(ALF) / W2) + PMA * W1 * 
     &                                   (1. / W2 + DTAN(GAM))         
      GAM  = GAM - RMA * W1                                             
      IERR = 0                                                          
                                                                        
      RETURN                                                            
      END
c
c--- an in-line version of gvrang (originally angles.for) must be
c--- included because of the common block, elcomm.
c
c--- this is required because the preprocessor, convdlm, renames
c--- all of the common blocks, appending the nav type.  the common
c--- block in angles.for was not being renamed, and therefor not
c--- being accessed.  one of the big problems with this solution
c--- is that applications cannot currently call angles directly
c--- for goes-8 navigation.  applications should use nvxopt('ANG '...)
c--- instead.
c

      SUBROUTINE GVRANG(JDAY,JTIME,XLAT,XLON,GHA,DEC,SATANG,SUNANG,RELAN
     *G)
C
C   THIS IS SSEC PROPRIETARY SOFTWARE - ITS USE IS RESTRICTED.
C $ SUBROUTINE ANGLES(JDAY,JTIME,XLAT,XLON,GHA,DEC,SATANG,SUNANG,RELANG)
C $ ANGLES - computes zenith angles of sun and satellite and relative
C $   azimuth angle              (DAS)
C $ INPUT:
C $   JDAY = (I) picture day (YYDDD)
C $   JTIME = (I) picture start time
C $   XLAT = (R) latitude of point
C $   XLON = (R) longitude of point
C $   GHA = (R) Greenwich hour angle of sun
C $   DEC = (R) declination of sun
C $ OUTPUT:
C $   SATANG = (R) zenith angle of satellite
C $   SUNANG = (R) zenith angle of sun
C $   RELANG = (R) relative angle
C $$ ANGLES = COMPUTATION, NAVIGATION
C ANGLES MOSHER 1074 WINLIB  ZENITH ANGLES TO SAT,SUN,AND REL AZIMUTH AN
C
C     INCLUDE 'ELCONS.INC'
C=========================== DIELCONS =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCONS = INCLUDE                                                
                                                                        
      REAL*8 PI                                                         
           PARAMETER (PI=3.141592653589793D0)                           
      REAL*8 DEG                                                        
           PARAMETER (DEG=180.D0/PI)                                    
      REAL*8 RAD                                                        
           PARAMETER (RAD=PI/180.D0)                                    
C                    DEGREES TO RADIANS CONVERSION PI/180               
      REAL*8 NOMORB                                                     
           PARAMETER (NOMORB=42164.365D0)                               
C                    NOMINAL RADIAL DISTANCE OF SATELLITE (KM)          
      REAL*8 AE                                                         
           PARAMETER (AE=6378.137D0)                                    
C                    EARTH EQUATORIAL RADIUS (KM)                       
      REAL*8 FER                                                        
           PARAMETER (FER=1.D0-(6356.7533D0/AE))                        
C                    EARTH FLATTENING COEFFICIENT = 1-(BE/AE)           
      REAL*4 AEBE2                                                      
           PARAMETER (AEBE2=1.D0/(1.D0-FER)**2)                         
      REAL*4 AEBE3                                                      
           PARAMETER (AEBE3=AEBE2-1.)                                   
      REAL*4 AEBE4                                                      
           PARAMETER (AEBE4=(1.D0-FER)**4-1.)                           
C========================= END DIELCONS ===========================     
C     INCLUDE 'ELCOMM.INC'
C=========================== DIELCOMM =============================     
                                                                        
C $   (JR)                                                              
C $   THIS INCLUDE FILE IS PART OF THE SUPPLIED GVAR NAV SOFTWARE       
C $                                                                     
C $   DESCRIPTION:                                                      
C $                                                                     
C $                                                                     
C $$  DIELCOMM = INCLUDE                                                
                                                                        
C     COMMON VARIABLES                                                  
C                                                                       
      REAL*8 XS(3)                                                      
C                      NORMALIZED S/C POSITION IN ECEF COORDINATES      
      REAL*8 BT(3,3)                                                    
C                      ECEF TO INSTRUMENT COORDINATES TRANSFORMATION    
      REAL*8  Q3                                                        
C                      USED IN SUBROUTINE LPOINT                        
      REAL*8 PITCH,ROLL,YAW                                             
C                          PITCH,ROLL,YAW ANGLES OF INSTRUMENT (RAD)    
      REAL*4 PMA,RMA                                                    
C                          PITCH,ROLL MISALIGNMENTS OF INSTRUMENT (RAD) 
         COMMON /ELCOMM/ XS,BT,Q3,PITCH,ROLL,YAW,PMA,RMA                
C========================= END DIELCOMM ===========================     
      DATA IDAY/0/
      DATA R/6371.221/
      RDPDG=PI/180.0
      IF(IDAY.EQ.JDAY)GO TO 1
      IDAY=JDAY
      INORB=0
 1    PICTIM=FTIME(JTIME)
C
C   DETERMINE SATELLITE POSITION
C
      XSAT = XS(1) * AE 
      YSAT = XS(2) * AE 
      ZSAT = XS(3) * AE 

      HEIGHT=SQRT(XSAT**2+YSAT**2+ZSAT**2)
      YLAT=RDPDG*XLAT
      YLAT=GEOLAT(YLAT,1)
      YLON=RDPDG*XLON
      SLAT=SIN(YLAT)
      CLAT=COS(YLAT)
      SLON=SIN(YLON)
      CLON=COS(YLON)
      XSAM=R*CLAT*CLON
      YSAM=R*CLAT*SLON
      ZSAM=R*SLAT
C
C   DETERMINE ZENITH ANGLE OF SUN
C
      SNLG=-PICTIM*PI/12.0-RDPDG*GHA
      SNDC=RDPDG*DEC
      COSDEC=COS(SNDC)
      US=COS(SNLG)*COSDEC
      VS=SIN(SNLG)*COSDEC
      WS=SIN(SNDC)
      SUNANG=ACOS((US*XSAM+VS*YSAM+WS*ZSAM)/R)/RDPDG
C
C   DETERMINE ZENITH ANGLE OF SATELLITE
C
      XVEC=XSAT-XSAM
      YVEC=YSAT-YSAM
      ZVEC=ZSAT-ZSAM
      XFACT=SQRT(XVEC**2+YVEC**2+ZVEC**2)
      SATANG=ACOS((XVEC*XSAM+YVEC*YSAM+ZVEC*ZSAM)/(R*XFACT))/RDPDG
C
C   DETERMINE RELATIVE ANGLE
C
      X1=CLAT*CLON
      Y1=CLAT*SLON
      Z1=SLAT
      X2=SLON
      Y2=-CLON
      X3=-SLAT*CLON
      Y3=-SLAT*SLON
      Z3=CLAT
      XC1=US-X1
      YC1=VS-Y1
      ZC1=WS-Z1
      XC2=XSAT/HEIGHT-X1
      YC2=YSAT/HEIGHT-Y1
      ZC2=ZSAT/HEIGHT-Z1
      XAN1=XC1*X3+YC1*Y3+ZC1*Z3
      XAN2=XC2*X3+YC2*Y3+ZC2*Z3
      YAN1=XC1*X2+YC1*Y2
      YAN2=XC2*X2+YC2*Y2
      XAN3=XAN1*XAN2+YAN1*YAN2
      YAN3=-YAN1*XAN2+XAN1*YAN2
      RELANG=ATAN2(YAN3,XAN3)/RDPDG
      RELANG=ABS(RELANG)
      RETURN
      END
