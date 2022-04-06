#!/bin/bash

deusex_aliveWC=$(cat deusex_alive | wc -c)
deusex_aliveDF=$(diff deusex_alive expected_out/deusex_alive_out | wc -l)

deusex_deadWC=$(cat deusex_dead | wc -c)
deusex_deadDF=$(diff deusex_dead expected_out/deusex_dead_out | wc -l)

if [ $deusex_aliveWC -eq 0 ]; then
	printf "Deusex program stops producing alive heroes. That's weird."
	exit 100
fi

if [ $deusex_aliveDF -gt 0 ]; then
	printf "Deusex program isn't producing the alive heroes in the correct order."
	exit 101
fi


if [ $deusex_deadWC -eq 0 ]; then
	printf "Deusex program stops producing dead heroes. That's weird."
	exit 100
fi

if [ $deusex_deadDF -gt 0 ]; then
	printf "Deusex program isn't producing the dead heroes in the correct order."
	exit 101
fi

