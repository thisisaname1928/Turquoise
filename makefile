img=turquoise.img
iso=turquoise.iso
imgdir=$(patsubst ./%, %, $(shell cd img && find ./ -type d))
imgfile=$(patsubst ./%, %, $(shell cd img && find ./ -type f))

test: build
	@qemu-system-x86_64 -hda $(img) -m 1G

testefi: build
	@qemu-system-x86_64 -L OVMF-X64-r15214/ -pflash OVMF-X64-r15214/OVMF.fd -hda $(img) -m 1G
	
miso: build
	@cp $(img) iso
	@xorriso -as mkisofs -R -f -e $(img) -no-emul-boot -o $(iso) iso


build: KERNEL
	@rm $(img)
	@printf "Building \e[1;32m$(img)\e[0;37m\n"
	@dd if=/dev/zero of=$(img) bs=1M count=0 seek=64
	@sgdisk $(img) -n 1:2048 -t 1:ef00
	@limine/limine bios-install $(img)
	@mformat -i $(img)@@1M
	@$(foreach dir, $(imgdir), mmd -i $(img)@@1M ::/$(dir);)
	@$(foreach file, $(imgfile), mcopy -i $(img)@@1M img/$(file) ::/$(shell dirname $(file));)

KERNEL:
	@printf "Building \e[1;32mturquoise.kernel\e[0;37m\n"
	@cd kernel && make
	@cp kernel/turquoise.kernel img/boot/

blah:
	echo $(patsubst %/, %, a/b/c/l.cpp)
	