#!/bin/bash

if command primusrun; then
  echo Primusrun Launch.
  primusrun ./bin/coh
elif command optirun; then
  echo Optirun Launch.
	optirun ./bin/coh
else
  echo Normal launch.
	./bin/coh
fi
