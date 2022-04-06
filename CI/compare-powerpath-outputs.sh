#!/bin/bash

power_aliveWC=$(cat power_alive | wc -c)
power_aliveDF=$(diff power_alive expected_out/power_alive_out | wc -l)

power_deadWC=$(cat power_dead | wc -c)
power_deadDF=$(diff power_dead expected_out/power_dead_out | wc -l)

if [ $power_aliveWC -eq 0 ]; then
	printf "power path - Adventure program stops producing alive heroes. That's weird."
	exit 100
fi

if [ $power_aliveDF -gt 0 ]; then

	printf "power path - Adventure program isn't producing the alive heroes in the correct order."
	exit 101
fi


if [ $power_deadWC -eq 0 ]; then
	printf "power path - Adventure program stops producing dead heroes. That's weird."
	exit 100
fi

if [ $power_deadDF -gt 0 ]; then
	printf "power path - Adventure program isn't producing the dead heroes in the correct order."
	exit 101
fi

