.subckt nand2x1 a b o
M1 o b vdd vdd tp L=130n W=130n
M2 o a vdd vdd tp L=130n W=130n
M3 o a n0 vss tn L=130n W=130n
M4 n0 b vss vss tn L=130n W=130n
.ends
