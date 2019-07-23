lorom

;determine if hp up will exit area/stage
{
    org $82EAC8 : exit_stage: ;0x016AC8

    org $82EB22 ;0x016B22
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
        lda.l .power_exit_list
    .store:
        sta $05
        rtl
        
    .power_exit_list: ;DCRando will fill in these
}
