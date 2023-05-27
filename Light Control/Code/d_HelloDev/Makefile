# Embedded Bone cross-compile module makefile

ifneq ($(KERNELRELEASE),)
    obj-m := HelloDev.o
else
    SUBDIRS := $(shell pwd)

default:
ifeq ($(strip $(KERNELDIR)),)
	$(error "KERNELDIR is undefined!")
else
	$(MAKE) -C $(KERNELDIR) M=$(SUBDIRS) modules 
endif

app:
	arm-linux-gnueabihf-gcc -o Test_HelloDev Test_HelloDev.c

clean:
	rm -rf *~ *.ko *.o *.mod.c modules.order Module.symvers .pwm* .tmp_versions 
	rm Test_HelloDev

endif

