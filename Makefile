include make/canned_recipes.mk

ifneq (,$(wildcard ./.env))
	include .env
	export
endif

list:
	@$(call _list)

init:
	@$(call _init)

init-dev:
	@$(call _init_dev)

compile:
	@$(CMAKE_COMPILE_COMMAND)
	@$(call _compile)

embed:
	@$(call _embed)

tests: compile
	@cd build/ && ctest --output-on-failure

install:
	@$(call _install)

uninstall:
	@$(call _uninstall)

clean:
	@$(call _clean)

.ONESHELL:
