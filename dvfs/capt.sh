#!/system/bin/sh

cpu_dir=/sys/devices/system/cpu/
gpu_dir=/sys/class/kgsl/kgsl-3d0/

stop mpdecision

for i in $( echo "0 `seq 3`" )
do
   echo 1 > $cpu_dir/cpu$i/online
   echo userspace > $cpu_dir/cpu$i/cpufreq/scaling_governor
done

echo none > /sys/class/kgsl/kgsl-3d0/pwrscale/policy 

for i in $( echo "0 `seq 12`" )
do
   echo enabled > /sys/class/thermal/thermal_zone$i/mode
done

for trace in $( ls donkeytamer_traces ); do
	for cpufreq in $( cat $cpu_dir/cpu0/cpufreq/scaling_available_frequencies ); do
		for i in $( echo "0 `seq 3`" ); do
			echo $cpufreq > $cpu_dir/cpu$i/cpufreq/scaling_setspeed
			cat $cpu_dir/cpu$i/cpufreq/scaling_cur_freq
		done
		for gpufreq in $( cat /sys/class/kgsl/kgsl-3d0/gpu_available_frequencies ); do
			echo $gpufreq > /sys/class/kgsl/kgsl-3d0/gpuclk
			cat /sys/class/kgsl/kgsl-3d0/gpuclk
			echo "Running trace $trace for cpufreq $cpufreq and gpufreq $gpufreq..."
			sleep 2

			./measure_power &
			mp_pid=`top -n 1 | grep measure_power | sed 's/ \([0-9]*\).*/\1/'`
			./monkeytamer donkeytamer_traces/$trace
			kill $mp_pid

			mkdir "$trace"_cpufreq_"$cpufreq"_gpufreq_"$gpufreq"
			mv power.txt "$trace"_cpufreq_"$cpufreq"_gpufreq_"$gpufreq"/power_"$trace"_cpufreq_"$cpufreq"_gpufreq_"$gpufreq".txt
		done
	done
done

