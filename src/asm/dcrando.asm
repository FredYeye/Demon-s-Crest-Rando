lorom

{ ;asar functions
    ;general use: calculate distance from label A to B
    function offset(a, b) = b-a
}

{ ;misc changes
    org $8389A0 : lda.b #18    ;reduce somulo  walk timer (0.5s -> 0.3s)
    org $838A3C : lda.b #4     ;reduce somulo hp (7 -> 4)
    org $82EADE : lda.b #54    ;reduce wait time on picking up crest powers (4s -> 0.9s)
    org $BCA044 : lda.w #50*60 ;change trio the pago timer (40s -> 50s)
}

{ ;determine if hp up will exit area/stage
org $82EAC8 : exit_stage:
org $82EAE3 : big_health_exit:

org $82EB22
    cmp #$1F
    beq big_health_exit
    tax
    lda.l exit_values-1,X
    bne exit_stage
    nop

org $BFD500 : exit_values: ;DCRando will fill in these
}

{ ;determine if item will exit area/stage
org $809A5A : update_item_total:
org $80BB58 : exit_area:

org $82E148
    jsl item_exit_check

org $BFD530 : item_exit_check:
    jsl update_item_total
    sep #$30
    lda $03
    and #$BF ;remove "no bounce" property
    lsr
    tax
    lda.l item_exit_list,X
    beq .ret
    jml exit_area
.ret:
    rtl

item_exit_list: ;DCRando will fill in these
}

{ ;determine if crest/firepower will exit area/stage
org $82EAD6 : jsl power_exit_check

org $BFD580 : power_exit_check:
    lda $02 ;catch hp upgrades and always store 2 (like original code)
    cmp #$49
    bne .is_power
    lda #$02
    bra .store

.is_power:
    lda $03
    and #$BF
    lsr
    tax
    lda.l .power_exit_list,X
.store:
    sta $05
    rtl

.power_exit_list: ;DCRando will fill in these
}

{ ;crawler item check
org $849977 : jsl crawler_item_check : nop

org $BFD5B0 : crawler_item_check:
    ldx #$0000 ;crawler offset - set from DCRando
    lda $1E51,X
    and #$0000 ;crawler bit - set from DCRando
    rtl
}

{ ;determine if talisman will exit area/stage
org $82E16A : jsl talisman_check

org $BFD5C0 : talisman_check:
    eor #$06 ;overwritten code from previous function
    sta $00  ;^

    lda $03
    and #$BF
    lsr
    tax
    lda.l talisman_exit_list,X
    beq .ret
    jml exit_area
.ret:
    rtl

talisman_exit_list: ;DCRando will fill in these
}

{ ;change stage reveal requirements
    ;85A1EE: beginning of stage requirement function

    ;todo: document this better!
org $85A1F6
    bne .no_phalanx
    ldy #$03
    lda $1E51
    bit #$0100
    bne .ret

    ldy #$00
    lda $1E56
    and #$001F
    cmp #$001F
    beq .ret

.no_phalanx:
    ldy #$01

.ret:
    sep #$20
    rtl
}

{ ;fix terrible grewon / grewon 2 code (separates demon fire & full health drop id loads)
org $BE9E22
    rep #$20
    lda #$0648 ;demon fire drop. gets replaced in rando
    ldx $03
    beq grewon2

    lda #$8000 : sta $09ED
    lda #$0623 ;full health drop
    nop
grewon2:
}

;----------- gfx related changes, mostly moving code and adjusting call sites

{ ;B19F - B2EC
org $80F00C : adc.w _81B19F,Y

;move this section to make space
org $81FEB4 : _81B19F:
    dw .B1AB, .B1EB, .B26B, .B22B, .B2AB, .B2AB

.B1AB:
    db $00,$00,$02,$02,$04,$04,$06,$06
    db $08,$08,$0A,$0A,$0C,$0C,$0E,$0E
    db $10,$10,$12,$12,$14,$14,$16,$16
    db $18,$18,$1A,$1A,$1C,$1C,$1E,$1E
    db $20,$20,$22,$22,$24,$24,$28,$28
    db $26,$26,$2C,$2A,$2E,$2C,$2A,$90
    db $30,$C8,$1C,$1C,$04,$04,$0C,$0C
    db $1C,$1C,$1A,$1A,$12,$12,$1C,$1C

.B1EB:
    db $00,$2E,$02,$30,$00,$04,$0C,$3A
    db $0C,$3A,$00,$0A,$06,$34,$04,$32
    db $00,$10,$14,$42,$00,$14,$00,$16
    db $1A,$48,$12,$40,$08,$36,$0A,$38
    db $12,$40,$00,$22,$18,$46,$00,$28
    db $00,$2A,$00,$2C,$00,$2E,$1C,$92
    db $1E,$CA,$0E,$3C,$16,$44,$10,$3E
    db $00,$3C,$00,$3C,$00,$42,$00,$6A

.B22B:
    db $02,$6A,$00,$68,$00,$04,$08,$70
    db $08,$70,$12,$7A,$28,$8E,$18,$7E
    db $00,$10,$1C,$82,$22,$88,$00,$16
    db $1A,$80,$04,$6C,$06,$6E,$08,$70
    db $0A,$72,$20,$86,$1E,$84,$0E,$76
    db $10,$78,$24,$8A,$26,$8C,$2A,$94
    db $16,$CE,$08,$70,$08,$70,$26,$8E
    db $04,$6C,$14,$7C,$1C,$82,$0C,$74

.B26B:
    db $06,$54,$00,$4A,$02,$4C,$04,$4E
    db $04,$4E,$00,$0A,$12,$60,$18,$66
    db $00,$10,$14,$62,$00,$14,$00,$16
    db $10,$5E,$0C,$5A,$08,$56,$0A,$58
    db $0E,$5C,$00,$22,$16,$64,$00,$28
    db $00,$2A,$00,$2C,$00,$2E,$1A,$96
    db $1C,$CC,$00,$56,$00,$56,$00,$0C
    db $00,$1A,$00,$1C,$00,$12,$00,$68

.B2AB:
    db $00,$98,$02,$9A,$04,$9C,$06,$9E
    db $08,$A0,$0A,$A2,$0C,$A4,$0E,$A6
    db $10,$A8,$12,$AA,$14,$AC,$16,$AE
    db $18,$B0,$1A,$B2,$1C,$B4,$1E,$B6
    db $20,$B8,$22,$BA,$24,$BC,$28,$C0
    db $26,$BE,$2C,$C2,$2E,$C4,$2A,$C6
    db $30,$D0,$1C,$B4,$04,$9C,$0C,$A4
    db $1C,$B4,$1A,$B2,$12,$AA,$1C,$B4
}

;----------

{
    ;adjust calling locations
    org $80BFB4 : lda.w _BD9C7D,Y
    org $80A72E : ldy.w _BD9F15,X
    org $80C11A : ldx.w _BD9F15,Y
    org $80BECC : ldy.w _BDA04A,X
}

{ ;9C7D - 9F14
org $BD9C7D+$80 : _BD9C7D: ;move this forward to make space for insertions into 9953
    ;dcrando will fill in these
}

{ ;9F15 - A049
org $BD9F15+$100 : _BD9F15: ;move forward to make space for 9C7D insertions
    dw .9FFD, .9FFD, .9FFE, .A011, .A011, .A011, .A011, .A011
    dw .A011, .A011, .A011, .A011, .A011, .A011, .A012, .A02D
    dw .A02D, .9FFD, .A02D, .A02D, .A02D, .A02D, .A02D, .A02D
    dw .A02D, .A02D, .A02D, .A02D, .A02D, .A02D, .A02D, .A02D
    dw .A02D, .A02D, .A02D, .A02D, .A02D, .A02D, .A02D, .A02D
    dw .A02D, .A02D, .A02E, .A049, .A049, .A049, .A049, .A049
    dw .A049, .A049, .A012, .A012, .A049, .A049, .A049, .A049
    dw .A049, .A049, .A049, .A049, .A049, .A049, .A049, .A049
    dw .A049, .A049, .A049, .A049, .A049, .A049, .A049, .A049
    dw .A049, .A049, .A049, .A049, .A049, .A049, .A049, .A049
    dw .9FFD, .9FFD, .9FFD, .9FFD, .9FFD, .9FFD, .9FFD, .9FFD
    dw .9FFD, .9FFD, .9FFD, .9FFD, .9FFD, .9FFD, .9FFD, .9FFD
    dw .9FFD, .9FFD, .9FFD, .9FFD, .9FFD, .9FFD, .9FFD, .9FFD
    dw .9FFD, .9FFD, .9FFD, .9FFD, .9FFD, .9FFD, .9FFD, .9FFD
    dw .9FFD, .9FFD, .9FFD, .9FFD

;-----

;don't know the format here
.9FFD: db $00
.9FFE: db $01, $01 : dw $05C0, $0100, $0080, $0100, $A800 : db $00, $32, $A0, $84, $01, $75, $A0
.A011: db $00
.A012: db $01, $02 : dw $7F00, $7F00, $0080, $0080, $D800 : db $00, $3C, $80, $FC, $00, $47, $A0, $60, $00, $21, $80, $A4, $00, $31, $A0
.A02D: db $00
.A02E: db $01, $02 : dw $7F00, $7F00, $0080, $0080, $A800 : db $01, $7F, $20, $F4, $00, $45, $40, $A4, $01, $7E, $20, $80, $00, $29, $40
.A049: db $00
}

{ ;A04A - A281?
_BDA04A:
    dw .A132, .A137, .A13C, .A13C, .A141, .A146, .A151, .A151
    dw .A151, .A15F, .A16A, .A175, .A16A, .A175, .A180, .A175
    dw .A175, .A137, .A18B, .A196, .A18B, .A1A1, .A18B, .A1A6
    dw .A1B1, .A1BC, .A1BC, .A1C1, .A1C1, .A1D2, .A1D2, .A1D7
    dw .A1D7, .A1D7, .A1D2, .A1D7, .A1D7, .A1DC, .A1DC, .A1DC
    dw .A1DC, .A1DC, .A1E1, .A1FB, .A203, .A203, .A214, .A214
    dw .A214, .A214, .A180, .A180, .A214, .A214, .A214, .A214
    dw .A214, .A214, .A214, .A1C1, .A151, .A21A, .A21A, .A21A
    dw .A15F, .A151, .A196, .A1D7, .A180, .A1BC, .A1C1, .A1D2
    dw .A132, .A137, .A175, .A1E1, .A21A, .A21A, .A21A, .A21A
    dw .A21A, .A21A, .A21F, .A21A, .A227, .A22C, .A231, .A236
    dw .A241, .A246, .A24B, .A253, .A25B, .A263, .A268, .A270
    dw .A268, .A275, .A270, .A268, .A268, .A263, .A270, .A270
    dw .A270, .A270, .A270, .A25B, .A25B, .A25B, .A25B, .A263
    dw .A27D, .A282, .A282, .A282

;-----

.A132: dw $0000 : db $26 : dw $FFFF
.A137: dw $0072 : db $3F : dw $FFFF
.A13C: dw $012F : db $2F : dw $FFFF
.A141: dw $06AB : db $31 : dw $FFFF
.A146: dw $0387 : db $29 : dw $03FF : db $08 : dw $0402 : db $02 : dw $FFFF
.A151: dw $02F4 : db $26 : dw $0366 : db $01 : dw $0369 : db $02 : dw $036F : db $08 : dw $FFFF
.A15F: dw $04A1 : db $1F : dw $062A : db $08 : dw $04FE : db $08 : dw $FFFF
.A16A: dw $0408 : db $2F : dw $0492 : db $02 : dw $02EE : db $02 : dw $FFFF
.A175: dw $0279 : db $28 : dw $02EB : db $09 : dw $02EE : db $02 : dw $FFFF
.A180: dw $0408 : db $0F : dw $0495 : db $04 : dw $0441 : db $2C : dw $FFFF
.A18B: dw $05D3 : db $07 : dw $0087 : db $02 : dw $05E8 : db $1E : dw $FFFF
.A196: dw $086A : db $17 : dw $08AC : db $04 : dw $061E : db $02 : dw $FFFF
.A1A1: dw $073E : db $30 : dw $FFFF
.A1A6: dw $08AF : db $01 : dw $086D : db $0E : dw $08B2 : db $1A : dw $FFFF
.A1B1: dw $0642 : db $23 : dw $06A8 : db $0E : dw $0402 : db $02 : dw $FFFF
.A1BC: dw $0A53 : db $31 : dw $FFFF
.A1C1: dw $0642 : db $03 : dw $0A17 : db $14 : dw $0687 : db $0C : dw $0A50 : db $0E : dw $0402 : db $02 : dw $FFFF
.A1D2: dw $07E9 : db $2B : dw $FFFF
.A1D7: dw $0AE6 : db $2B : dw $FFFF
.A1DC: dw $01BC : db $3F : dw $FFFF
.A1E1: dw $01BC : db $1F : dw $0C21 : db $0B : dw $0C3F : db $01 : dw $023D : db $04 : dw $0249 : db $02 : dw $02EE : db $02 : dw $02F1 : db $02 : dw $0C42 : db $06 : dw $FFFF
.A1FB: dw $01BC : db $34 : dw $0C54 : db $0B : dw $FFFF
.A203: dw $01BC : db $07 : dw $01D1 : db $08 : dw $0BA6 : db $10 : dw $0BD3 : db $12 : dw $02EE : db $02 : dw $FFFF
.A214: dw $0BD6 : db $19 : dw $FF00 : db $FF ;?
.A21A: dw $0516 : db $3F : dw $FFFF
.A21F: dw $0CE7 : db $08 : dw $052E : db $37 : dw $FFFF
.A227: dw $09ED : db $3F : dw $FFFF
.A22C: dw $0A0E : db $3F : dw $FFFF
.A231: dw $098D : db $3F : dw $FFFF
.A236: dw $0900 : db $0D : dw $0924 : db $03 : dw $0924 : db $01 : dw $FFFF
.A241: dw $092A : db $3F : dw $FFFF
.A246: dw $09C0 : db $0F : dw $FFFF
.A24B: dw $0000 : db $0F : dw $0C75 : db $0F : dw $FFFF
.A253: dw $0C93 : db $0F : dw $0543 : db $10 : dw $FFFF
.A25B: dw $07CB : db $01 : dw $07CB : db $0A : dw $FFFF
.A263: dw $0CBA : db $1B : dw $FFFF
.A268: dw $095D : db $10 : dw $0B67 : db $0E : dw $FFFF
.A270: dw $0CFF : db $0B : dw $FFFF
.A275: dw $0CFF : db $01 : dw $0CFF : db $0B : dw $FFFF
.A27D: dw $0D20 : db $13 : dw $FFFF

.A282: ;unused?
}

;----------
