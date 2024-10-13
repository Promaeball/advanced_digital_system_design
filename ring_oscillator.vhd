library ieee;
use ieee.std_logic_1164.all;

entity ring_oscillator is
    generic (
        ro_length:    positive    := 13
    );
    port (
        enable:        in    std_logic;
        osc_out:    out    std_logic
    );
end entity ring_oscillator;

architecture gen of ring_oscillator is

    -- declare any signals you may need
    signal inverter : std_logic_vector(ro_length - 1 downto 0);
    attribute keep: boolean;
    attribute keep of inverter: signal is true;

begin
	assert ro_length mod 2 = 1
		report "ro_length must be an odd number"
		severity failure;

    -- place nand gate
    -- one input is enable, the other one the output of the last inverter
    -- output goes into the first inverter in the chain
	 
		inverter(0) <= enable nand inverter(ro_length - 1);

    -- place inverters
    -- for ... generate
    -- end generate

		invert_inputs: for i in 1 to ro_length - 1 generate
			inverter(i) <= not inverter(i-1);

		end generate invert_inputs;


    -- drive osc_out with output of last inverter in the chain
    osc_out <= inverter(ro_length - 1);
end architecture gen;