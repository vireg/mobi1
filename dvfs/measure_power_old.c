#include <stdio.h>
#include <stdlib.h>
//#include<time.h>

int main(int argc, char *argv[])
{		
	unsigned long long i;
	unsigned long long v;
	unsigned long long p;
	//float elap;
	//clock_t start;
	FILE * fp, *inow, *vnow;
	fp = fopen ("power.txt", "w+");
	fclose(fp);
	
	while(1){
		fp = fopen ("power.txt", "a+");
		inow = fopen("/sys/devices/platform/msm_ssbi.0/pm8921-core/pm8921-charger/power_supply/battery/current_now", "r");
		vnow = fopen("/sys/devices/platform/msm_ssbi.0/pm8921-core/pm8921-charger/power_supply/battery/voltage_now", "r");
		//start = time(NULL);
		fscanf(inow, "%llu", &i);//system("cat /sys/devices/platform/msm_ssbi.0/pm8921-core/pm8921-charger/power_supply/battery/current_now");
		fscanf(vnow, "%llu", &v);//system("cat /sys/devices/platform/msm_ssbi.0/pm8921-core/pm8921-charger/power_supply/battery/voltage_now");
		//fseek(inow,0,SEEK_SET);
		//fseek(vnow,0,SEEK_SET);
		fseek(fp,0,SEEK_END);
		p = i*v;
		//printf("i=%lld, v=%lld, p=%lld\n",i,v,p);	
		fprintf(fp, "%llu\n", i);
		//elap = time(NULL) - 0;
		//printf("elap: %f\n", elap);
		fclose(fp);
		fclose(inow);
		fclose(vnow);
		usleep(250000);
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

