lorom

;determine if hp up will exit area/stage
{
    org $82EAC8 : exit_stage: ;0x016AC8
    org $82EAE3 : big_health_exit: ;0x016AE3

    org $82EB22 ;0x016B22
        cmp #$1F ;check if this is a big health with exit properties first, since A gets clobbered
        beq big_health_exit
        tax
        lda.l exit_values-1,X
        bne exit_stage
        nop

    org $BFD500 : exit_values: ;0x1FD500 | DCRando will fill in these
}

;determine if item will exit area/stage
{
    org $809A5A : update_item_total: ;0x001A5A
    org $80BB58 : exit_area: ;0x003B58

    org $82E148 ;0x016148
        jsl item_exit_check

    org $BFD530 : item_exit_check: ;0x1FD530
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

;determine if crest/firepower will exit area/stage
{
    org $82EAD6 : jsl power_exit_check

    org $BFD580 : power_exit_check: ;0x1FD580
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

;crawler item check
{
    org $849977 : jsl crawler_item_check : nop ;0x021977

    org $BFD5B0 : crawler_item_check: ;0x1FD5B0
        ldx #$0000 ;crawler offset - set from DCRando
        lda $1E51,X
        and #$0000 ;crawler bit - set from DCRando
        rtl
}

;determine if talisman will exit area/stage
{
    org $82E16A : jsl talisman_check ;0x01616A

    org $BFD5C0 : talisman_check: ;0x1FD5C0
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

;change stage reveal requirements
{
    ;85A1EE: beginning of stage requirement function
    org $85A1F6 ;0x02A1F6
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
