.PHONY: all controller client clean clean-controller clean-client

PIO ?= pio

all: controller client

controller:
	$(PIO) run -e controller

client:
	$(PIO) run -e client

clean: clean-controller clean-client

clean-controller:
	$(PIO) run -t clean -e controller

clean-client:
	$(PIO) run -t clean -e client
