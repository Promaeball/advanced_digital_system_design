library ieee;
use ieee.std_logic_1164.all;

entity toplevel is
	generic (
		ro_length:	positive := 13;
		ro_count:	positive := 16
	);
	port (
		clock:	in	std_logic;	-- global clock input, 50 MHz clock
		reset:	in	std_logic	-- global asynchronous reset, button
	);
end entity toplevel;

architecture top of toplevel is
	-- TODO: any signal declarations you may need
	signal response : std_logic;
	
	
begin

	-- make instance of ro_puf
	puf: ro_puf
		generic map (
			-- add generic information
			-- should come from toplevel's generic list
			ro_length => ro_length,
			ro_count => ro_count	
		)
		port map (
			-- add port information
			-- should use some signals internal to this architecture
			-- should use the `reset' input from toplevel
			reset => reset,
			enable => enable,
			response => response
		);

	-- TODO: control unit
	-- use control unit entity from blackboard, make entity here
	-- uses the `clock' input and the `reset' input from toplevel
	-- still might need to look at
	
	fsm : control_unit
		generic map (
			challenge_bits 		
			clock_frequency:	
			delay_us:			
		);
		port map (
			clock => clock,
			reset => reset,
			enable => enable
		
			counter_enable:	out	std_logic;
			counter_reset:	out	std_logic;
			challenge:		out	std_logic_vector(2*challenge_bits - 1 downto 0);
			store_response:	out	std_logic;
			done:	out	std_logic
		);


	-- TODO: BRAM
	-- create a BRAM using the IP Catalog, instance it here
	-- make sure you enable the In-System Memory Viewer!
	: BRAM
		generic map (
		
		);	
		port map (
            address => challenge,
            clock => clock,
            data => response_vec,
            wren => write_mem
        );

end architecture top;
