.section ".text.startup"
.global _Reset



_Reset:

	MSR     CPSR_c, #(0x12 | 0x80 | 0x40)
	LDR     sp,=__irq_stack_top__              /* set the IRQ stack pointer */

	MSR     CPSR_c, #(0x11 | 0x80 | 0x40)
	LDR     sp,=__fiq_stack_top__              /* set the FIQ stack pointer */

	MSR     CPSR_c, #(0x13 | 0x80 | 0x40)
	LDR     sp,=__svc_stack_top__              /* set the SVC stack pointer */

	MSR     CPSR_c, #(0x17 | 0x80 | 0x40)
	LDR     sp,=__abt_stack_top__              /* set the ABT stack pointer */

	MSR     CPSR_c, #(0x1B | 0x80 | 0x40)
	LDR     sp,=__und_stack_top__


	MSR     CPSR_c, #(0x1F | 0x80 | 0x40)
	LDR     sp, =__c_stack_top__
	BL      main

	B .
