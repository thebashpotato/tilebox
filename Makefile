include make/canned_recipes.mk

ifneq (,$(wildcard ./.env))
	include .env
	export
endif

list:
	@$(call _list)

init:
	@$(call _init)

init_dev:
	@$(call _init_dev)

compile:
	@$(CMAKE_COMPILE_COMMAND)
	@$(call _compile)

embed:
	@$(call _embed)

tests: compile
	@cd build/ && ctest

clean:
	@$(call _clean)

.ONESHELL:
