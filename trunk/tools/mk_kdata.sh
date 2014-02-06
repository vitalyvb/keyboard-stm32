#!/bin/bash

_gen_id="_$$_$RANDOM"

infile="$1"
hfile="$2"
varsuffix="$3"

cat > "$hfile" <<EOF1
/* DO NOT EDIT. THIS FILE IS GENERATED. */
#ifndef GEN_NAMES_H${_gen_id}
#define GEN_NAMES_H${_gen_id}

const unsigned char *const${varsuffix} = 
EOF1

cat "$infile" | grep -v '^#' | grep -v '^$' | sed -e "s/#.*$//" |
awk '
{
    c=$1;
    i=2;
    while ($i != ""){
	printf("%s %s %02x\n", toupper($i), c, length($i));
	i++;
    }
}
' | sort | \
while read name hex nlen; do
    #echo "// $name $hex"
    if [ -z "$hex" ]; then
	continue
    fi

    if echo "$hex" | grep "^#" &> /dev/null; then
	continue
    fi

    code="\\\\x$hex"
    len="\\\\x$nlen"

    echo -e "\t\"${code}\"\t\"${len}\"\t\"${name}\""

done >> "$hfile"


cat >> "$hfile" <<EOF2
	"\x00";
#endif /* GEN_NAMES_H${_gen_id} */
EOF2

