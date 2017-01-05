#!/system/bin/sh

for trace in $( ls donkeytamer_traces ); do
        for cpufreq in $( cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies ); do
		for gpufreq in $( cat /sys/class/kgsl/kgsl-3d0/gpu_available_frequencies ); do
		        rm "$trace"_cpufreq_"$cpufreq"_gpufreq_"$gpufreq"/power_"$trace"_cpufreq_"$cpufreq"_gpufreq_"$gpufreq".txt
		        rmdir "$trace"_cpufreq_"$cpufreq"_gpufreq_"$gpufreq"
		done
        done
done

