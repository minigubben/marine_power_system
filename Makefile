.PHONY: all controller 8-outputs-1 8-outputs-2 8-outputs-3 6-inputs-1 6-inputs-2 6-inputs-3 clean clean-controller clean-8-outputs-1 clean-8-outputs-2 clean-8-outputs-3 clean-6-inputs-1 clean-6-inputs-2 clean-6-inputs-3

PIO ?= pio

all: controller 8-outputs-1 8-outputs-2 8-outputs-3 6-inputs-1 6-inputs-2 6-inputs-3

controller:
	$(PIO) run -e controller

8-outputs-1:
	$(PIO) run -e 8-outputs-1

8-outputs-2:
	$(PIO) run -e 8-outputs-2

8-outputs-3:
	$(PIO) run -e 8-outputs-3

6-inputs-1:
	$(PIO) run -e 6-inputs-1

6-inputs-2:
	$(PIO) run -e 6-inputs-2

6-inputs-3:
	$(PIO) run -e 6-inputs-3

clean: clean-controller clean-8-outputs-1 clean-8-outputs-2 clean-8-outputs-3 clean-6-inputs-1 clean-6-inputs-2 clean-6-inputs-3

clean-controller:
	$(PIO) run -t clean -e controller

clean-8-outputs-1:
	$(PIO) run -t clean -e 8-outputs-1

clean-8-outputs-2:
	$(PIO) run -t clean -e 8-outputs-2

clean-8-outputs-3:
	$(PIO) run -t clean -e 8-outputs-3

clean-6-inputs-1:
	$(PIO) run -t clean -e 6-inputs-1

clean-6-inputs-2:
	$(PIO) run -t clean -e 6-inputs-2

clean-6-inputs-3:
	$(PIO) run -t clean -e 6-inputs-3
