#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NUM_CPU 4
#define NUM_TEMP 13

int get_current_now(unsigned long long *);
int get_voltage_now(unsigned long long *);
int get_gpuclk(unsigned long long *);
int get_gpu_utilization(unsigned long long *);
int get_cur_freq(unsigned long long *, int);
int get_cpu_utilization(unsigned long long *, int);
int get_temp(unsigned long long *, int);

int main(int argc, char *argv[])
{		
	unsigned long long i, v, gpuclk, gpu_utilization;
	unsigned long long cur_freq[NUM_CPU], cpu_utilization[NUM_CPU], temp[NUM_TEMP];
	unsigned long long p;
	int cpu,t;
	struct timeval t1, t2;
    double elapsedTime;

	FILE * fp;
	fp = fopen ("power.txt", "w+");
	fclose(fp);
	
	while(1){
		gettimeofday(&t1, NULL);	// start timer
		get_current_now(&i);
		get_voltage_now(&v);
		get_gpuclk(&gpuclk);
		get_gpu_utilization(&gpu_utilization);
		for (cpu=0; cpu<NUM_CPU; cpu++){
			get_cur_freq(&cur_freq[cpu], cpu);	
		}
		for (cpu=0; cpu<NUM_CPU; cpu++){
			get_cpu_utilization(&cpu_utilization[cpu], cpu);	
		}
		for (t=0; t<NUM_TEMP; t++){
			get_temp(&temp[t], t);
		}
		p = i*v;

		fp = fopen ("power.txt", "a+");
		//fseek(fp,0,SEEK_END);
		/*
		printf("i=%llu v=%llu p=%llu gpuclk=%llu gpu_utilization=%llu ",i,v,p,gpuclk,gpu_utilization);
		printf("cpufreq[0]=%llu cpufreq[1]=%llu cpufreq[2]=%llu cpufreq[3]=%llu ",cur_freq[0],cur_freq[1],cur_freq[2],cur_freq[3]); 
		printf("cpu_util[0]=%llu cpu_util[1]=%llu cpu_util[2]=%llu cpu_util[3]=%llu ", cpu_utilization[0],cpu_utilization[1],cpu_utilization[2],cpu_utilization[3]);
		for (t=0; t<NUM_TEMP; t++){
			printf("%llu ", temp[t]);	
		}
		printf("\n");
		*/

		fprintf(fp, "%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,", i,v,p,gpuclk,gpu_utilization,cur_freq[0],cur_freq[1],cur_freq[2],cur_freq[3]);
		fprintf(fp, "%llu,%llu,%llu,%llu,", cpu_utilization[0],cpu_utilization[1],cpu_utilization[2],cpu_utilization[3]);
		for (t=0; t<NUM_TEMP; t++){
			fprintf(fp, "%llu,", temp[t]);	
		}
		fprintf(fp, "\n");

		fclose(fp);

		gettimeofday(&t2, NULL);	// stop timer
		// compute and print the elapsed time in us
    	elapsedTime = (t2.tv_usec - t1.tv_usec);
    	//printf("elapsedTime = %fus, Sleep for %fus\n",elapsedTime,250000-elapsedTime);
		usleep(250000 - elapsedTime);
	}
	return 0;
}

int get_current_now(unsigned long long *current_now)
{
	FILE *fp;
	int ret = 0;

	*current_now = 0;

	fp = fopen("/sys/devices/platform/msm_ssbi.0/pm8921-core/pm8921-charger/power_supply/battery/current_now", "r");
	if (fp == NULL)
		return -1;

	if (fscanf(fp, "%llu\n", current_now) != 1)
		ret = -1;

	fclose(fp);

	return ret;
}

int get_voltage_now(unsigned long long *voltage_now)
{
	FILE *fp;
	int ret = 0;

	*voltage_now = 0;

	fp = fopen("/sys/devices/platform/msm_ssbi.0/pm8921-core/pm8921-charger/power_supply/battery/voltage_now", "r");
	if (fp == NULL)
		return -1;

	if (fscanf(fp, "%llu\n", voltage_now) != 1)
		ret = -1;

	fclose(fp);

	return ret;
}

int get_gpuclk(unsigned long long *gpuclk)
{
	FILE *fp;
	int ret = 0;

	*gpuclk = 0;

	fp = fopen("/sys/class/kgsl/kgsl-3d0/gpuclk", "r");
	if (fp == NULL)
		return -1;

	if (fscanf(fp, "%llu\n", gpuclk) != 1)
		ret = -1;

	fclose(fp);

	return ret;
}

int get_gpu_utilization(unsigned long long *gpubusy)
{
	FILE *fp;
	int ret = 0;

	*gpubusy = 0;

	fp = fopen("/sys/class/kgsl/kgsl-3d0/gpubusy", "r");
	if (fp == NULL)
		return -1;

	if (fscanf(fp, "%llu\n", gpubusy) != 1)
		ret = -1;

	fclose(fp);

	return ret;
}

int get_cur_freq(unsigned long long *cur_freq, int cpu)
{
	FILE *fp;
	int ret = 0;

	*cur_freq = 0;

	switch (cpu){
		case 0: fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", "r");
			break;
		case 1: fp = fopen("/sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq", "r");
			break;
		case 2: fp = fopen("/sys/devices/system/cpu/cpu2/cpufreq/scaling_cur_freq", "r");
			break;
		case 3: fp = fopen("/sys/devices/system/cpu/cpu3/cpufreq/scaling_cur_freq", "r");
			break;
		default: fp = 0;
	}
	if (fp == NULL)
		return -1;

	if (fscanf(fp, "%llu\n", cur_freq) != 1)
		ret = -1;

	fclose(fp);

	return ret;
}

int get_cpu_utilization(unsigned long long *cpu_utilization, int cpu)
{
	FILE *fp;
	int ret = 0;

	*cpu_utilization = 0;

	switch (cpu){
		case 0: fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpu_utilization", "r");
			break;
		case 1: fp = fopen("/sys/devices/system/cpu/cpu1/cpufreq/cpu_utilization", "r");
			break;
		case 2: fp = fopen("/sys/devices/system/cpu/cpu2/cpufreq/cpu_utilization", "r");
			break;
		case 3: fp = fopen("/sys/devices/system/cpu/cpu3/cpufreq/cpu_utilization", "r");
			break;
		default: fp = 0;
	}
	if (fp == NULL)
		return -1;

	if (fscanf(fp, "%llu\n", cpu_utilization) != 1)
		ret = -1;

	fclose(fp);

	return ret;
}

int get_temp(unsigned long long *temp, int zone)
{
	FILE *fp;
	int ret = 0;

	*temp = 0;

	switch (zone){
		case 0: fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
			break;
		case 1: fp = fopen("/sys/class/thermal/thermal_zone1/temp", "r");
			break;
		case 2: fp = fopen("/sys/class/thermal/thermal_zone2/temp", "r");
			break;
		case 3: fp = fopen("/sys/class/thermal/thermal_zone3/temp", "r");
			break;
		case 4: fp = fopen("/sys/class/thermal/thermal_zone4/temp", "r");
			break;
		case 5: fp = fopen("/sys/class/thermal/thermal_zone5/temp", "r");
			break;
		case 6: fp = fopen("/sys/class/thermal/thermal_zone6/temp", "r");
			break;
		case 7: fp = fopen("/sys/class/thermal/thermal_zone7/temp", "r");
			break;
		case 8: fp = fopen("/sys/class/thermal/thermal_zone8/temp", "r");
			break;
		case 9: fp = fopen("/sys/class/thermal/thermal_zone9/temp", "r");
			break;
		case 10: fp = fopen("/sys/class/thermal/thermal_zone10/temp", "r");
			break;
		case 11: fp = fopen("/sys/class/thermal/thermal_zone11/temp", "r");
			break;
		case 12: fp = fopen("/sys/class/thermal/thermal_zone12/temp", "r");
			break;
		default: fp = 0;
	}
	if (fp == NULL)
		return -1;

	if (fscanf(fp, "%llu\n", temp) != 1)
		ret = -1;

	fclose(fp);

	return ret;
}

