docker run -it --rm \
  -v /tools/Xilinx:/tools/Xilinx \
  -v $(pwd):/home/ws \
  vitis-2023.2

# > Inside the container
# source /tools/Xilinx/Vitis_HLS/2023.2/settings64.sh
# vitis_hls -version
#
# vitis_hls -f script.tcl
#
