/* DO NOT EDIT!
   This file was automatically generated by m68k-palmos-multigen v2.3
   from ../Sections.def  */

	.file	"multiple_code_sections.s"

	.globl	__text__
	.lcomm	__text__,4
	.globl	__text__segment1
	.lcomm	__text__segment1,4
	.globl	__text__segment2
	.lcomm	__text__segment2,4

.text
	.globl	_GccRelocateData
_GccRelocateData:
	bra.w	_GccLoadCodeAndRelocateData

.section ehook
	.long	_GccReleaseCode