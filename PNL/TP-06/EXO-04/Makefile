.PHONY: all 

ifneq ($(KERNELRELEASE),)

	obj-m := taskmonitor.o

else

  KERNELDIR ?= /tmp/linux-4.2.3/
  PWD := $(shell pwd)

all:
	make -C $(KERNELDIR) M=$$PWD modules

clean:
	make -C $(KERNELDIR) M=$$PWD clean

endif
