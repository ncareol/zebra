       INTEGER FUNCTION LWENDW(CFILE)
C *** McIDAS Revision History ***
C 1 LWENDW.FOR 4-Apr-90,15:30:50,`SSEC' PC-McIDAS ver 5.00
C 2 LWENDW.FOR 24-Sep-90,18:20:20,`SMG' First Release into COMmon
C 3 LWENDW.FOR 19-Jul-91,13:43:34,`SUEG' Change IH to I*2 variable
C 4 LWENDW.FOR 19-Jul-91,13:43:34,`USER' Released
C 5 LWENDW.FOR 14-Aug-91,14:48:28,`RJL' Corrected computation of word number to
C      be 0 based
C 6 LWENDW.FOR 14-Aug-91,14:48:28,`USER' Released
C *** McIDAS Revision History ***
C ? INTEGER FUNCTION LWENDW(CFILE)    (TMW)
C ? LWEND - RETURNS THE LAST WORD WRITTEN IN AN LW FILE (0 BASED)
C ? PARAMETERS:  CFILE = NAME OF LW FILE
C ?  IF FUNCTION VALUE IS =-1, FILE DOES NOT EXIST
C ? NOTE:  SEE LWEND FOR LAST PAGE NUMBER
       CHARACTER CFILE*(*)
       INTEGER *2 IH
       L=LEOF(IH(CFILE))
       IF (L.LT.0) THEN
              LWENDW=-1
       ELSE
              LWENDW=L/4-1
       ENDIF
       RETURN
       END