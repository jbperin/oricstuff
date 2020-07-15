



EC33 AD C0 02   LDA $02C0       HIRES 
EC36 48         PHA 
EC37 29 02      AND #$02        Error if hires mode cannot be 
EC39 F0 B9      BEQ $EBF4       entered. 
EC3B 68         PLA             Set status to indicate hires 
EC3C 09 01      ORA #$01        mode. 
EC3E 8D C0 02   STA $02C0 
EC41 20 20 F9   JSR $F920       Set screen to hires mode. 
EC44 60         RTS


EDC4 A2 00      LDX #$00        This routine transfers a block
EDC6 A0 00      LDY #$00        of data using #0C as the 
EDC8 C4 10      CPY $10         source pointer and #0E as the 
EDCA D0 04      BNE $EDD0       destination pointer. The 
EDCC E4 11      CPX $11         length of data to be moved is 
EDCE F0 0F      BEQ $EDDF       held in locations #10/#11. 
EDD0 B1 0C      LDA ($0C),Y 
EDD2 91 0E      STA ($0E),Y 
EDD4 C8         INY 
EDD5 D0 F1      BNE $EDC8 
EDD7 E6 0D      INC $0D 
EDD9 E6 0F      INC $0F 
EDDB E8         INX 
EDDC 4C C8 ED   JMP $EDC8 
EDDF 60         RTS



F77C 48         PHA PRINT CHAR TO SCREEN (in X).
F77D 08         PHP
F77E 98         TYA Save all registers on stack.
F77F 48         PHA
F780 8A         TXA
F781 48         PHA Leave a copy of X in A.
F782 D8         CLD
F783 E0 13      CPX #$13 Test for CTRL S, T, and F.
F785 F0 46      BEQ $F7CD If either of them are in X
F787 E0 14      CPX #$14 then go to CTRL CHAR routine.
F789 F0 42      BEQ $F7CD
F78B E0 06      CPX #$06
F78D F0 3E      BEQ $F7CD
F78F AD 6A 02   LDA $026A
F792 29 02      AND #$02
F794 F0 3A      BEQ $F7D0 Screen printing inhibited.
F796 8A         TXA
F797 C9 20      CMP #$20
F799 90 32      BCC $F7CD Control character present.
F79B AD 6A 02   LDA $026A Test and branch if the
F79E 29 10      AND #$10 ESCAPE key was not the last
F7A0 F0 13      BEQ $F7B5 printed.
F7A2 8A         TXA If character after ESCAPE is
F7A3 38         SEC a CTRL character then print a
F7A4 E9 40      SBC #$40 space instead. Otherwise
F7A6 30 09      BMI $F7B1 convert key to attribute code
F7A8 29 1F      AND #$1F and print to screen.
F7AA 20 E4 F7   JSR $F7E4
F7AD A9 1B      LDA #$1B
F7AF D0 1C      BNE $F7CD
F7B1 A9 20      LDA #$20
F7B3 10 F5      BPL $F7AA
F7B5 E0 7F      CPX #$7F Character is DELete.
F7B7 F0 08      BEQ $F7C1
F7B9 68         PLA
F7BA 48         PHA
F7BB 20 E4 F7   JSR $F7E4 Print accumulator on screen.
F7BE 4C D0 F7   JMP $F7D0
F7C1 A9 08      LDA #$08 DEL is done by moving cursor
F7C3 20 02 F6   JSR $F602 back 1 place and printing a
F7C6 A9 20      LDA #$20 space character, and moving
F7C8 20 E4 F7   JSR $F7E4 cursor back again 1 place.
F7CB A9 08      LDA #$08
F7CD 20 02 F6   JSR $F602
F7D0 68         PLA
F7D1 AA         TAX
F7D2 68         PLA Registers not affected at
F7D3 A8         TAY end of the routine.
F7D4 28         PLP
F7D5 68         PLA
F7D6 60         RTS


F8DC 48         PHA             Set up some page 2 variables 
F8DD 8A         TXA             for HIRES. 
F8DE 48         PHA 
F8DF A9 01      LDA #$01 
F8E1 8D 1F 02   STA $021F       Set hires indicator. 
F8E4 A9 BF      LDA #$BF 
F8E6 8D 7B 02   STA $027B       Set the address of the first 
F8E9 8D 79 02   STA $0279       line of text section of screen 
F8EC A9 68      LDA #$68        to $BF68 and that of the 
F8EE 8D 7A 02   STA $027A       second line to $BF90. 
F8F1 A9 90      LDA #$90 
F8F3 8D 78 02   STA $0278 
F8F6 A9 03      LDA #$03        Set the maximum number of 
F8F8 8D 7E 02   STA $027E        rows of text available. 
F8FB A9 00      LDA #$00 
F8FD 8D 7D 02   STA $027D       Set number of characters used 
F900 A9 50      LDA #$50        in screen scrolling to 80 - 
F902 8D 7C 02   STA $027C       two lines worth. 
F905 A2 0C      LDX #$0C
F907 20 38 02   JSR $0238       Clear screen. 
F90A 68         PLA 
F90B AA         TAX 
F90C 68         PLA 
F90D 60         RTS


F920 48         PHA             SET SCREEN TO HIRES 
F921 AD 1F 02   LDA $021F 
F924 D0 05      BNE $F92B 
F926 A2 0B      LDX #$0B 
F928 20 82 F9   JSR $F982 
F92B A9 FE      LDA #$FE        Disable cursor. 
F92D 2D 6A 02   AND $026A 
F930 8D 6A 02   STA $026A 
F933 A9 1E      LDA #$1E        Write 50Hz attribute to last 
F935 8D DF BF   STA $BFDF       location on screen. 
F938 A9 40      LDA #$40 
F93A 8D 00 A0   STA $A000 
F93D A2 17      LDX #$17 
F93F 20 82 F9   JSR $F982 
F942 A9 00      LDA #$00        Set X and Y cursor coordinates 
F944 8D 19 02   STA $0219       to zero. 
F947 8D 1A 02   STA $021A 
F94A 85 10      STA $10         Set cursor address to #A000. 
F94C A9 A0      LDA #$A0 
F94E 85 11      STA $11 
F950 A9 20      LDA #$20        Set cursor position within 
F952 8D 15 02   STA $0215       byte on screen. 
F955 A9 FF      LDA #$FF 
F957 8D 13 02   STA $0213       Set pattern register. 
F95A 20 DC F8   JSR $F8DC       Set up some page 2 variables. 
F95D A9 01      LDA #$01        Re-enable cursor. 
F95F 0D 6A 02   ORA $026A 
F962 8D 6A 02   STA $026A 
F965 68         PLA 
F966 60         RTS



F982 A0 06      LDY #$06        This routine writes addresses 
F984 BD 92 F9   LDA $F992,X     from table below to locations 
F987 99 0B 00   STA $000B,Y     #0C to #11 inclusive. The
F98A CA         DEX             value of X determines which 
F98B 88         DEY             part of the table is copied. 
F98C D0 F6      BNE $F984       The data is then used in a 
F98E 20 C4 ED   JSR $EDC4       block transfer routine. 
F991 60         RTS 

F992 78 FC 00 B5 00 03 00 B4 
F99A 00 98 80 07 00 98 00 B4 
F9A2 80 07 00 A0 01 A0 3F 1F

