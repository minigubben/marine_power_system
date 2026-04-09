.PHONY: all controller client clean clean-controller clean-client

CONTROLLER_DIR := projects/controller
CLIENT_DIR := projects/client

all: controller client

controller:
	$(MAKE) -C $(CONTROLLER_DIR)

client:
	$(MAKE) -C $(CLIENT_DIR)

clean: clean-controller clean-client

clean-controller:
	$(MAKE) -C $(CONTROLLER_DIR) clean

clean-client:
	$(MAKE) -C $(CLIENT_DIR) clean
