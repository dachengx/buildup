SHELL:=bash
thick:= 1 2 3 4
distance:=$(shell seq -f '%g' 5 1 15)
collimator:= 0 1
attenuator:= 0 1

erg:=$(shell for i in $(thick); do for j in $(distance); do for k in $(collimator); do for l in $(attenuator); do echo $${i}-$${j}-$${k}-$${l}; done; done; done; done)
# $(warning $(erg))

run:=$(erg:%=result/%.log)

.PHONY : all

all : run

run : $(run)

csv : result/result.csv

result/%.log : 
	@mkdir -p $(dir $@)
	@rm -f result/$*.csv
	./buildup $* run.mac > $@ 2>&1

result/result.csv : $(run)
	@rm -f $@
	@mkdir -p $(dir $@)
	python collect.py $(dir $@) -o result/result.csv

clean :
	rm -rf result

.DELETE_ON_ERROR:

.SECONDARY: