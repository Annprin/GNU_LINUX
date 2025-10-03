#!/usr/bin/env bash

set -euo pipefail

DELAY=${1:-0.03}

lines=()
maxw=0
while IFS= read -r line || [ -n "$line" ]; do
  lines+=("$line")
  len=${#line}
  if (( len > maxw )); then
    maxw=$len
  fi
done

height=${#lines[@]}

if (( height == 0 )); then
  exit 0
fi

R=()
C=()
CH=()

for ((r=0; r<height; r++)); do
  line="${lines[r]}"
  linelen=${#line}
  for ((c=0; c<maxw; c++)); do
    if (( c < linelen )); then
      ch="${line:c:1}"
    else
      ch=' '
    fi
    R+=($((r+1)))
    C+=($((c+1)))
    CH+=("$ch")
  done
done

N=${#CH[@]}

rand_upto() {
  local k=$1
  if (( k <= 0 )); then
    echo 0
    return
  fi
  local a=$RANDOM
  local b=$RANDOM
  local r=$(( (a << 15) ^ b ))
  echo $(( r % k ))
}

indices=()
for ((i=0; i<N; i++)); do
  indices+=($i)
done

for ((i=N-1; i>0; i--)); do
  j=$(rand_upto $((i+1)))
  tmp=${indices[i]}; indices[i]=${indices[j]}; indices[j]=$tmp
done

printf '\033[?25l'
trap 'printf "\033[?25h\033[?0m\n"; exit' INT TERM EXIT

printf '\033[2J'
printf '\033[H'

for idx in "${indices[@]}"; do
  row=${R[idx]}
  col=${C[idx]}
  ch=${CH[idx]}
  printf '\033[%d;%dH' "$row" "$col"
  printf '%s' "$ch"

  if [ "$DELAY" != "0" ]; then
    sleep "$DELAY"
  fi
done

printf '\033[%d;1H' $((height+1))
printf '\033[?25h'

trap - INT TERM EXIT

