#!/bin/bash

strategy_aliveWC=$(cat strategy_alive | wc -c)
strategy_aliveDF=$(diff strategy_alive expected_out/strategy_alive_out | wc -l)

strategy_deadWC=$(cat strategy_dead | wc -c)
strategy_deadDF=$(diff strategy_dead expected_out/strategy_dead_out | wc -l)

if [ $strategy_aliveWC -eq 0 ]; then
	printf "strategy path - Adventure program stops producing alive heroes. That's weird."
	exit 100
fi

if [ $strategy_aliveDF -gt 0 ]; then

	printf "strategy path - Adventure program isn't producing the alive heroes in the correct order."
	exit 101
fi


if [ $strategy_deadWC -eq 0 ]; then
	printf "strategy path - Adventure program stops producing dead heroes. That's weird."
	exit 100
fi

if [ $strategy_deadDF -gt 0 ]; then
	printf "strategy path - Adventure program isn't producing the dead heroes in the correct order."
	exit 101
fi
