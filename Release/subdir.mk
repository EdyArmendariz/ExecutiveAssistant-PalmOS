################################################################################
# Automatically-generated file.  Do not edit directly; use project properties.
################################################################################


$(MULTISECTION_OBJS): $(ROOT)/Sections.def 
	@echo
	m68k-palmos-multigen -b multiple_code_sections  $(ROOT)/Sections.def 

multiple_code_sections.s: $(ROOT)/Sections.def 
	@echo
	m68k-palmos-multigen -b multiple_code_sections  $(ROOT)/Sections.def 

