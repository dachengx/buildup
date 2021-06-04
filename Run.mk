SHELL:=bash
thick:= 1 2 3 4
distance:= 12 14 16
collimator:= 0 1
attenuator:= 0 1

erg:=$(shell for i in $(thick); do for j in $(distance); do for k in $(collimator); do for l in $(attenuator); do echo $${i}-$${j}-$${k}-$${l}; done; done; done; done)
# $(warning $(erg))

run:=$(erg:%=result/%.log)

.PHONY : all

all : run

run : $(run)

result/%.log : 
	@mkdir -p $(dir $@)
	./buildup $* run.mac > $@ 2>&1

clean :
	rm -rf result

.DELETE_ON_ERROR: 

.SECONDARY: