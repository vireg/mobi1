for page in $( cat list.txt )
do
	~/chromium/src/build/android/adb_run_chrome_public $page

	adb logcat -c 
	adb logcat chromium:V cr.SomeComponent:V *:S > plt.txt &
	count=0
	LIMIT=10
	while [ "$count" -le "$LIMIT" ]; do
		sleep 1
		start_time=`grep "Vishwesh: NavigatorImpl::NavigateToEntry:" plt.txt | head -n 1 | sed 's/.* \([0-9]*:[0-9]*:[0-9.]*\).*/\1/'`
		stop_time=`grep "Vishwesh: FrameTreeNode::DidStopLoading:" plt.txt | tail -n 1 | sed 's/.* \([0-9]*:[0-9]*:[0-9.]*\).*/\1/'`
		count=$(($count+1))
		echo $count
	done

	start=`date -d "$start_time" +'%s.%3N'`
	stop=`date -d "$stop_time" +'%s.%3N'`
	# load_time=$(($stop-$start))	# Can't do float computations
	load_time=`echo "$stop-$start" | bc`
	# echo "$stop $start" | awk '{print $1-$2}'
	echo "start_time=$start_time, stop_time=$stop_time, load_time=$load_time"
	# Extract PID of logcat from ps
	mp_pid=`ps | grep adb | sed 's/ [a-z].*//'`
	ps
	echo "Killing $mp_pid..."
	kill $mp_pid
	ps
	#rm plt.txt
done
