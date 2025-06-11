pn=$1

for ((i=5; i<=15; i+=5)); do
    for ((j=i; j<=15; j+=5)); do
        for ((b=2; b<=5; b+=1)); do
            for ((l=1; l<=5; l+=1)); do
                filename="/home/rdennis/data/levelFiles/${i}_${j}_${b}_${l}_1000_${pn}.json"
                if [ -f "$filename" ]; then
                    echo "skipping $filename"
                    continue
                fi
                filename="/home/rdennis/data/levelFiles/${i}_${j}_${b}_${l}_250_${pn}.json"
                if [ -f "$filename" ]; then
                    echo "skipping $filename"
                    continue
                fi
                echo "$i,$j,$b,$l,1000,$pn" | ./MetaBlocks
            done
        done
    done
done
