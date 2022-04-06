#!/bin/bash

charm_aliveWC=$(cat charm_alive | wc -c)
charm_aliveDF=$(diff charm_alive expected_out/charm_alive_out | wc -l)

charm_deadWC=$(cat charm_dead | wc -c)
charm_deadDF=$(diff charm_dead expected_out/charm_dead_out | wc -l)

if [ $charm_aliveWC -eq 0 ]; then
	printf "charm path - Adventure program stops producing alive heroes. That's weird."
	exit 100
fi

if [ $charm_aliveDF -gt 0 ]; then

	printf "charm path - Adventure program isn't producing the alive heroes in the correct order."
	exit 101
fi


if [ $charm_deadWC -eq 0 ]; then
	printf "charm path - Adventure program stops producing dead heroes. That's weird."
	exit 100
fi

if [ $charm_deadDF -gt 0 ]; then
	printf "charm path - Adventure program isn't producing the dead heroes in the correct order."
	exit 101
fi
