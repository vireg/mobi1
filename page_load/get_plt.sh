
adb logcat -c 

for page in $( cat list.txt )
do
	~/chromium/src/build/android/adb_run_chrome_public $page
	sleep 10
done

adb logcat -d chromium:V cr.SomeComponent:V *:S > plt.txt
# Following code is required if running logcat in background (without -d option)
# sleep 1 
# Extract PID of logcat from ps
# mp_pid=`ps | grep adb | sed 's/ [a-z].*//'`
# ps
# echo "Killing $mp_pid..."
# kill $mp_pid
# ps

count=2
for page in $( cat list.txt )
do
	page=`echo $page | sed 's/http:\/\///'`
	# next_page=`head -n $count list.txt | tail -n 1 | sed 's/http:\/\///'`
	next_page=`tail -n +$count list.txt | head -n 1 | sed 's/http:\/\///'`

	# https://stackoverflow.com/questions/7103531/how-to-get-the-part-of-file-after-the-line-that-matches-grep-expression-first
	if [ -z "$next_page" ]; then
		cat plt.txt | sed -n -e "/$page/,\$p" > "$page"_log.txt
	else
		cat plt.txt | sed -n -e "/$page/,\$p" | sed -e "/$next_page/,\$d" > "$page"_log.txt
	fi

	start_time=`grep "Vishwesh: NavigatorImpl::NavigateToEntry:" "$page"_log.txt | head -n 1 | sed 's/.* \([0-9]*:[0-9]*:[0-9.]*\).*/\1/'`
	# WebContentsImpl::DidStopLoading & WebContentsObserverProxy::DidStopLoading called only once for the page. FrameTreeNode::DidStopLoading can be called multiple times.
	stop_time=`grep "Vishwesh: WebContentsImpl::DidStopLoading:" "$page"_log.txt | tail -n 1 | sed 's/.* \([0-9]*:[0-9]*:[0-9.]*\).*/\1/'`

	start=`date -d "$start_time" +'%s.%3N'`
	stop=`date -d "$stop_time" +'%s.%3N'`
	# load_time=$(($stop-$start))	# Can't do float computations
	load_time=`echo "$stop-$start" | bc`
	# echo "$stop $start" | awk '{print $1-$2}'
	echo "$page: start_time=$start_time, stop_time=$stop_time, load_time=$load_time"
	count=$(($count+1))
	#rm plt.txt
done
