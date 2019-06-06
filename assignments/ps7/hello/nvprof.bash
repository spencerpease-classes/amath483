#!/bin/bash

nvprof --metrics achieved_occupancy,branch_efficiency,sysmem_read_throughput,dram_read_throughput,gld_requested_throughput,gst_throughput,gst_efficiency,sm_efficiency,shared_efficiency,gld_efficiency --events branch,divergent_branch --print-gpu-summary ${*}
