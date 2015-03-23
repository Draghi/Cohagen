#!/bin/bash

if command -v primusrun; then
  primusrun ./bin/Cohagen
elif command -v optirun; then
  optirun ./bin/Cohagen
else
  ./bin/Cohagen
fi

