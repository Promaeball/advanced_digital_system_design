
set start_x 6


set inverter_y 38


set num_inverters_per_row 13

# Loop through the inverter stages and place them at the desired locations on the FPGA
for {set i 0} {${i} < 16} {incr i} {
    set x [expr ${start_x} + ${i}]  ;# Increment the X-coordinate with each iteration
    
    for {set j 0} {${j} < ${num_inverters_per_row}} {incr j} {    
        # Define the physical location in the FPGA for each inverter (changing N index).
        set inverter_location "LAB_X${x}_Y${inverter_y}_N${j}"
        
        set name_fmt [format ${name} ${i} ${j}]
        
        set_location_assignment ${inverter_location} -to "${name_fmt}"
            }
}
