files=$(ls /home/rdennis/data/cleanLevelFiles/*.json);
for file in $files; do
    val=$(echo $file | ./MetaBlocks)
    if [[ "$val" =~ ^-?[0-9]+$ ]]; then
        if (( $val != 1 )); then
            rm $file
        fi
    else
        rm $file
    fi
done