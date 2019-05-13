ENTRY(_Reset)

C_STACK_SIZE   = 0x1000;
IRQ_STACK_SIZE = 0x1000;
FIQ_STACK_SIZE = 0x1000;
SVC_STACK_SIZE = 0x200;
ABT_STACK_SIZE = 0x200;
UND_STACK_SIZE = 0x200;

SECTIONS {
	. = 0x10000;
	.startup . : { *(.text.startup) }
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss COMMON) }
	. = ALIGN(8);




	.stack : {
		__stack_start__ = . ;

		. += IRQ_STACK_SIZE;
		. = ALIGN (4);
		__irq_stack_top__ = . ;

		. += FIQ_STACK_SIZE;
		. = ALIGN (4);
		__fiq_stack_top__ = . ;

		. += SVC_STACK_SIZE;
		. = ALIGN (4);
		__svc_stack_top__ = . ;

		. += ABT_STACK_SIZE;
		. = ALIGN (4);
		__abt_stack_top__ = . ;

		. += UND_STACK_SIZE;
		. = ALIGN (4);
		__und_stack_top__ = . ;

		. += C_STACK_SIZE;
		. = ALIGN (4);
		__c_stack_top__ = . ;
	}

}
