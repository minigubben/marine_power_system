.PHONY: all controller 8-outputs 6-inputs clean clean-controller clean-8-outputs clean-6-inputs

PIO ?= pio

all: controller 8-outputs 6-inputs

controller:
	$(PIO) run -e controller

8-outputs:
	$(PIO) run -e 8-outputs

6-inputs:
	$(PIO) run -e 6-inputs

clean: clean-controller clean-8-outputs clean-6-inputs

clean-controller:
	$(PIO) run -t clean -e controller

clean-8-outputs:
	$(PIO) run -t clean -e 8-outputs

clean-6-inputs:
	$(PIO) run -t clean -e 6-inputs
