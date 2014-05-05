#!/bin/bash

_gen_id="_$$_$RANDOM"

infile="$1"
hfile="$2"
varsuffix="$3"
prefix="$4"

cat > "$hfile" <<EOF1
/* DO NOT EDIT. THIS FILE IS GENERATED. */

const unsigned char *const${varsuffix} = (unsigned char*)
EOF1

cat "$infile" | grep -v '^#' | grep -v '^$' | sed -e "s/#.*$//" |
awk -v prefix="$prefix" '
{
    c=$1;
    i=2;
    while ($i != ""){
	p=$i;
	if (prefix){
		sub(prefix, "", p);
	} else {
		sub(/^[0-9]/, "_&", p);
	}
	printf("%s %s %02x\n", toupper(p), c, length(p));
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
EOF2

