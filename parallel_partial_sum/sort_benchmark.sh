#!/bin/sh
#sorts the benchmark results (best first)

LANG= sort -g --field-separator=',' --key=5 $1

