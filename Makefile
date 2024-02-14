.PHONY: hblock

hblock: $(wildcard *.c)
	gcc main.c -o $@
	@echo "Compiling..."

