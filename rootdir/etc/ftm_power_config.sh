#!/system/bin/sh
echo "ftm_power_config start" >> /dev/kmsg
bootmode=`getprop ro.vendor.factory.mode`
/vendor/bin/sh /vendor/bin/init.qcom.post_boot.sh
sleep 5
echo 0 > /sys/devices/system/cpu/cpu4/online
echo 0 > /sys/devices/system/cpu/cpu5/online
echo 0 > /sys/devices/system/cpu/cpu6/online
echo 0 > /sys/devices/system/cpu/cpu7/online

sleep 10
echo 0 > /sys/devices/system/cpu/cpu3/online
echo 0 > /sys/devices/system/cpu/cpu2/online
echo 518400 > /sys/devices/system/cpu/cpufreq/policy0/scaling_min_freq
echo 883200 > /sys/devices/system/cpu/cpufreq/policy0/scaling_max_freq

echo "ftm_power_config ftrace config start" >> /dev/kmsg
echo 0 > /sys/kernel/debug/tracing/tracing_on
echo "" > /sys/kernel/debug/tracing/set_event
echo "" > /sys/kernel/debug/tracing/trace
echo power:cpu_idle power:cpu_frequency power:cpu_frequency_switch_start msm_low_power:* sched:sched_switch sched:sched_wakeup sched:sched_wakeup_new  sched:sched_enq_deq_task >> /sys/kernel/debug/tracing/set_event
echo power:memlat_dev_update power:memlat_dev_meas msm_bus:bus_update_request msm_bus:* power:bw_hwmon_update power:bw_hwmon_meas >> /sys/kernel/debug/tracing/set_event
echo power:bw_hwmon_meas power:bw_hwmon_update>> /sys/kernel/debug/tracing/set_event
echo clk:clk_set_rate clk:clk_enable clk:clk_disable >> /sys/kernel/debug/tracing/set_event
echo power:clock_set_rate power:clock_enable power:clock_disable msm_bus:bus_update_request >> /sys/kernel/debug/tracing/set_event
echo cpufreq_interactive:cpufreq_interactive_target cpufreq_interactive:cpufreq_interactive_setspeed >> /sys/kernel/debug/tracing/set_event
echo irq:* >> /sys/kernel/debug/tracing/set_event
echo mdss:mdp_mixer_update mdss:mdp_sspp_change mdss:mdp_commit >> /sys/kernel/debug/tracing/set_event
echo workqueue:* >> /sys/kernel/debug/tracing/set_event
echo kgsl:kgsl_pwrlevel kgsl:kgsl_buslevel kgsl:kgsl_pwr_set_state >> /sys/kernel/debug/tracing/set_event
echo regulator:regulator_set_voltage_complete regulator:regulator_disable_complete regulator:regulator_enable_complete >> /sys/kernel/debug/tracing/set_event
echo thermal:* >> /sys/kernel/debug/tracing/set_event
cat /sys/kernel/debug/tracing/set_event
echo 40000 > /sys/kernel/debug/tracing/buffer_size_kb
cat /sys/kernel/debug/tracing/buffer_size_kb

sleep 5
echo "ftm_power_config capture ftrace start" >> /dev/kmsg
echo 0 > /sys/kernel/debug/tracing/tracing_on && cat /sys/kernel/debug/tracing/trace > /data/local/trace.txt && echo 0 > /sys/kernel/debug/tracing/tracing_on && echo "" > /sys/kernel/debug/tracing/trace && sleep 1 && echo 1 > /sys/kernel/debug/tracing/tracing_on && sleep 40 && echo 0 > /sys/kernel/debug/tracing/tracing_on && cat /sys/kernel/debug/tracing/trace > /mnt/vendor/op2/trace.txt&

echo 0 > /sys/module/lpm_levels/parameters/sleep_disabled
baseband=`getprop ro.baseband`
echo "ftm_power_config done baseband=$baseband" >> /dev/kmsg

