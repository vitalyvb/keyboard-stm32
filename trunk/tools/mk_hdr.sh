#!/bin/bash

_gen_id="_$$_$RANDOM"

infile="$1"
hfile="$2"

cat > "$hfile" <<EOF1
/* DO NOT EDIT. THIS FILE IS GENERATED. */
#ifndef GEN_NAMES_H${_gen_id}
#define GEN_NAMES_H${_gen_id}

EOF1

cat "$infile" | grep -v '^#' | grep -v '^$' | sed -e "s/#.*$//" |
awk '
{
    c=$1;
    i=2;
    while ($i != ""){
	print toupper($i), c;
	i++;
    }
}
' | sort | \
while read name hex; do
    if [ -z "$hex" ]; then
	continue
    fi

    if echo "$hex" | grep "^#" &> /dev/null; then
	continue
    fi

    echo -e "#define KC_${name}\t(0x${hex})"

done >> "$hfile"


cat >> "$hfile" <<EOF2

#endif /* GEN_NAMES_H${_gen_id} */
EOF2

