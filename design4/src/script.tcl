open_project softmax_design4
set_top softmax_top

add_files softmax.cpp
add_files softmax.h
add_files -tb softmax_tb.cc

open_solution -flow_target vitis solution
set_part {xck26-sfvc784-2LV-c}
create_clock -period 250MHz -name default  ;# 250 MHz
config_dataflow -strict_mode warning
config_rtl -deadlock_detection sim
config_interface -m_axi_conservative_mode=1
config_interface -m_axi_auto_max_ports=0
config_export -format xo -ipname softmax_top

# Simulación C
csim_design -clean

# Síntesis
csynth_design

# Co-sim
#cosim_design
close_project
puts "HLS completed successfully"

exit