
#define _ADR_TABLE 1400

    .zero

* = $80
_var_80 .dsb 1
_var_81 .dsb 1
_var_82 .dsb 1
_var_83 .dsb 1
_var_84 .dsb 1
_var_85 .dsb 1
_var_86 .dsb 1
_var_87 .dsb 1
_var_88 .dsb 1
_var_89 .dsb 1
_var_8A .dsb 1
_var_8B .dsb 1
_var_8C .dsb 1
_var_8D .dsb 1
_var_8E .dsb 1
_var_8F .dsb 1

    .text 

_table_139E .byt $20, $10, $08, $04, $02, $01

    .bss
* = $1400
_table_1400 .dsb 256
_table_1500 .dsb 256
_table_1600 .dsb 256
_table_1700 .dsb 256

_spare_table .dsb 96

    .text


_SetUpTables:
.(

    LDX #$00
    LDY #$00
    STX _var_80
label_1206:
    TXA
    ASL    
    ASL 
    ASL
    ASL
    STA _table_1500,Y
    LDA _var_80
    STA _table_1400,Y
    INY
    BEQ label_1222
    INX
    CPX #$06
    BNE label_1206
    INC _var_80
    LDX #$00

    BEQ label_1206
    NOP
label_1222:
    LDA #$A0
    STA _var_81
    LDA #$00
    TAY
    STA _var_80
label_122B:
    LDA _var_80
    STA _table_1600,Y
    LDA _var_81
    STA _table_1700,Y
    LDA _var_80
    CLC
    ADC #$28
    STA _var_80
    BCC label_1240
    INC _var_81
label_1240:
    INY
    BNE label_122B
    RTS
.)

_CreateCharacterTable:
.(
    lda #<(_spare_table)
    ldy #>(_spare_table)
    ldx #65
    jsr create_character_table
.)

;; To call this routine, you must set up
;;   the X register to the appropriate ASCII value, 
;;   A to the low part of a spare #60 byte address, 
;;   and the Y register to the high value of this #60 byte table.
;; The routine uses the normal character set area at #9800, 
;; though you may choose to alter this to the alternate area at #9C00.

create_character_table:
.(
    STA _var_8C
    STY _var_8D
    LDA #$00
    STA _var_80
    LDA #$00
    STA _var_85
    STX _var_84
    ASL _var_84
    ROL _var_85
    ASL _var_84
    ROL _var_85
    ASL _var_84
    ROL _var_85
    CLC
    LDA _var_85
    ADC #$98
    STA _var_85
label_1265:
    LDY #$00
label_1267:
    LDA (_var_84),Y
    STA _var_87
    LDA #$00
    STA _var_88
    LDX _var_80
    BEQ label_127E
label_1273:
    LSR _var_87
    ROR _var_88
    DEX
    BNE label_1273
    LSR _var_88
    LSR _var_88
label_127E:
    LDA _var_87
    STA (_var_8C),Y
    TYA
    PHA
    ORA #$08
    TAY
    LDA _var_88
    STA (_var_8C),Y
    PLA
    TAY
    INY
    CPY #$08
    BNE label_1267
    CLC
    LDA _var_8C
    ADC #$10
    STA _var_8C
    BCC label_129D
    INC _var_8D
label_129D:
    INC _var_80
    LDA _var_80
    CMP #$06
    BNE label_1265
    RTS
.)



calculate_address:
.(
    LDY _var_8B
    LDA _table_1600,Y
    STA _var_82
    LDA _table_1700,Y
    STA _var_83
    LDY _var_8A
    LDA _table_1400,Y
    CLC
    ADC _var_82
    STA _var_82
    BCC label_12C0
    INC _var_83
label_12C0:
    LDA _table_1500,Y
    CLC
    ADC _var_8C
    STA _var_8C
    BCC label_12CC
    INC _var_8D
label_12CC:
    RTS
.)

_testRoutine:
.(
    LDA #45	;; X co-ordinate
    STA   $8A
    LDA #60            ;;  Y co-ordinate
    STA   $8B
    LDA #$40            ;; F40 is address of the table.
    LDY #$0F
    JSR display_character_cell
    RTS
.)


display_character_cell:
.(
    STA _var_8C
    STY _var_8D
    JSR calculate_address
    LDY #$00
label_12D6:
    LDA (_var_82),Y
    EOR (_var_8C),Y
    STA (_var_82),Y
    TYA
    PHA
    ORA #$08
    TAY
    LDA (_var_8C),Y
    TAX
    PLA
    TAY
    INY
    TXA
    EOR (_var_82),Y
    STA (_var_82),Y
    CLC
    LDA _var_82
    ADC #$27
    STA _var_82
    BCC label_12F7
    INC _var_83
label_12F7:
    CPY #$08
    BNE label_12D6
    RTS
.)

FastPixelAddress:
.(
    CLC
    LDA _table_1600,Y
    ADC _table_1400,X
    STA _var_82
    LDA #$00
    ADC _table_1700,Y
    STA _var_83
    LDA _table_1500,X
    LSR
    LSR
    LSR
    LSR
    TAY
    LDA _table_139E,Y
    LDY #$00
    RTS
.)

SetDot:
.(
    JSR FastPixelAddress
    ORA (_var_82),Y
    STA (_var_82),Y
    RTS
.)

ClearDot:
.(
    JSR FastPixelAddress
    EOR #$FF
    AND (_var_82),Y
    STA (_var_82),Y
    RTS
.)

AlterDot:
.(
    JSR FastPixelAddress
    EOR (_var_82),Y
    STA (_var_82),Y
    RTS
.)

TestDot:
.(
    JSR FastPixelAddress
    AND (_var_82),Y
    RTS
.)

ColourCharacter:
.(
    JSR FastPixelAddress
label_118A:
    LDA (_var_82),Y
    PHA
    TYA
    TAX
    ORA #$08
    TAY
    PLA
    STA (_var_8C),Y
    TXA
    TAY
    LDA (_var_82),Y
    CMP #$40
    BNE label_11A1
    LDA (_var_8C),Y
    STA (_var_82),Y
label_11A1:
    CLC
    LDA _var_82
    ADC #$27
    STA _var_82
    BCC label_11AC
    INC _var_83
label_11AC:
    INY
    CPY #$08
    BNE label_118A
    RTS
.)

RemoveColours:
.(
    JSR FastPixelAddress
    LDA _var_8C
    ORA #$08
    STA _var_8C
label_11BB:
    LDA (_var_8C),Y
    STA (_var_82),Y
    CLC
    LDA _var_82
    ADC #$27
    STA _var_82
    BCC label_11CA
    INC _var_83
label_11CA:
    INY
    CPY #$08
    BNE label_11BB
    RTS
.)

DrawLargeShape:
.(
    JSR FastPixelAddress
    LDA _table_1500,X
    LSR
    LSR
    LSR
    LSR
    STA _var_8A
    LDA _var_8F
    STA _var_85
label_1102:
    LDY #$00
    LDA #$00
    STA _var_84
label_1108:
    LDA #$00
    STA _var_86
    LDX _var_8A
    LDA (_var_82),Y
    STA (_var_80),Y
    LDA (_var_8C),Y
    CMP #$40
    BCC label_1131
    AND #$BF
    CPX #$00
    BEQ label_1125
    CLC
label_111F:
    ROR
    ROR _var_86
    DEX
    BNE label_111F
label_1125:
    ORA _var_84
    LSR _var_86
    LSR _var_86
    LDX _var_86
    STX _var_84
    ORA #$40
label_1131:
    STA (_var_82),Y
    INY
    CPY _var_8E
    BNE label_1108
    CLC
    LDA _var_82
    ADC #$28
    STA _var_82
    BCC label_1143
    INC _var_83
label_1143:
    CLC
    LDA _var_80
    ADC $8E
    STA _var_80
    BCC label_114E
    INC _var_81
label_114E:
    CLC
    LDA _var_8C
    ADC _var_8E
    STA _var_8C
    BCC label_1159
    INC _var_8D
label_1159:
    DEC _var_85
    BNE label_1102
    RTS
.)

RemoveLargeShape:
.(
    JSR FastPixelAddress
    LDX _var_8F
label_1163:
    LDY _var_8E
    DEY
label_1166:
    LDA (_var_80),Y
    STA (_var_82),Y
    DEY
    BPL label_1166
    CLC
    LDA _var_80
    ADC _var_8E
    STA _var_80
    BCC label_1178
    INC _var_81
label_1178:
    CLC
    LDA _var_82
    ADC #$28
    STA _var_82
    BCC label_1183
    INC _var_83
label_1183:
    DEX
    BNE label_1163
    RTS
.)


Paint:
.(
    SEI
    LDX #S0F
    LDA _var_80,X
    STA $10E2,X
    DEX
    BPL $1003
    CLD
    JSR $1200
    LDX $00
    LDY $01
    LDA #$00
    STA _var_8C
    LDA #$FF
    PHA
    PHA
    BNE $1021
    PLA
    TAY
    PLA
    TAX
    CPX #SFF
    BEQ $1084
    LDA _var_8C
    BNE $102F
    LDA $#01
    STA _var_8D
    STA _var_8E
    JSR $10A2
    DEY
    JSR $1098
    STA _var_8F
    BNE $1042
    LDA _var_8D
    BEQ $1042
    TXA
    PHA
    TYA
    PHA
    LDA S8F
    STA _var_8D
    INY
    INY
    JSR $1098
    STA S8F
    BNE $1057
    LDA _var_8E
    BEQ $1057
    TXA
    PHA
    TYA
    PHA
    LDA _var_8F
    STA _var_8E
    DEY
    DEX
    JSR $1098
    BNE $106C
    TXA
    PHA
    TYA
    PHA
    LDA #$01
    STA _var_8C
    BNE %1070
    LDA #$00
    STA _var_8C
    INX
    INX
    JSR $1098
    BNE $107F
    TXA
    PHA
    TYA
    PHA
    LDA #$01
    STA _var_8C
    DEX
    LDA #$00
    BEQ $101D
    LDX #$0F
    LDA $10E2,X
    STA _var_80,X
    DEX
    BPL $1086
    CLI
    RTS
    STY _var_80
    JSR $137E
    LDY _var_80
    RTS
    STY _var_80
    JSR $1398
    PHP
    LDY _var_80
    PLP
    RTS
    STX _var_82
    STY _var_83
    TSX
    STX _var_8F
    INX
    INX
    INX
    LDA $0100,X
    CMP #$FF
    BEQ $10D8
    CMP _var_83
    BNE $10D3
    LDA $0101,X
    CMP _var_82
    BNE $10D3
    DEX
    CPX _var_8F
    BEQ $10CC
    LDA $0100,X
    STA $0102,X
    SEC
    BCS $10BE
    INC _var_8F
    INC _var_8F
    SEC
    BCS $10D8
    INX
    INX
    SEC
    BCS $10AC
    LDX _var_8F
    TXS
    LDX _var_82
    LDY _var_83
    JMP $1090
.)
