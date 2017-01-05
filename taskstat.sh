

for pid in $( pgrep chromium )
do
	echo "$pid: `cat /proc/$pid/status | head -n 1`"
	for tid in $( ls /proc/$pid/task/ )
	do
		cat /proc/$pid/task/$tid/status | head -n 1
	done
done

