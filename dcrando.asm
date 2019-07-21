;code/data to determine if hp up will exit area/stage
{
    org $82EAC8 : exit_stage: ;0x016AC8

    org $82EB22 ;0x016B22
        tax
        lda.l exit_values-1,X
        bne exit_stage
        nop

    org $FFD500 : exit_values: ;0x1FD500 | DCRando will fill in these
}
