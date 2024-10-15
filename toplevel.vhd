library ieee;
use ieee.std_logic_1164.all;

entity toplevel is
	generic (
		ro_length:	positive := 13;
		ro_count:	positive := 16
	);
	port (
		clock:	in	std_logic;	-- global clock input, 50 MHz clock
		reset:	in	std_logic;	-- global asynchronous reset, button
		LEDR0: out std_logic
	);
end entity toplevel;

architecture top of toplevel is
	-- TODO: any signal declarations you may need

	-- Delcarations for Control Unit
	constant challenge_bits: positive := 3;
	constant clock_frequency: positive := 50;	-- in MHz
	constant delay_us: positive := 10;
	signal challenge: std_logic_vector(5 downto 0);
	signal counter_enable: std_logic;
	signal counter_reset : std_logic;
	
	-- Declarations for ro_puf
	signal response: std_logic;
	
	--Declarations for BRAM
	signal data : STD_LOGIC;
	signal wren_sig: std_logic;
	signal q_sig: STD_LOGIC_VECTOR (0 DOWNTO 0);
	signal data_sig: STD_LOGIC_VECTOR (0 DOWNTO 0);
	
	component bram
		PORT (
			address		: IN STD_LOGIC_VECTOR (5 DOWNTO 0);
			clock		: IN STD_LOGIC  := '1';
			data		: IN STD_LOGIC_VECTOR (0 DOWNTO 0);
			wren		: IN STD_LOGIC ;
			q		: OUT STD_LOGIC_VECTOR (0 DOWNTO 0)
		);
	end component;
	
begin

	-- make instance of ro_puf
	puf: entity work.ro_puf
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
			reset => counter_reset,
			enable => counter_enable,
			response => data_sig(0),
			challenge => challenge
		);

	-- TODO: control unit
	-- use control unit entity from blackboard, make entity here
	-- uses the `clock' input and the `reset' input from toplevel
	-- still might need to look at
	
	fsm: entity work.control_unit
		generic map (
			challenge_bits => challenge_bits,
			clock_frequency => clock_frequency,
			delay_us => delay_us		
		)
		port map (
			clock => clock,
			reset => reset,
			enable => '1',
			counter_enable => counter_enable,
			counter_reset => counter_reset,
			challenge => challenge,
			store_response => wren_sig,
			done => LEDR0
		);


	-- TODO: BRAM
	-- create a BRAM using the IP Catalog, instance it here
	-- make sure you enable the In-System Memory Viewer!
	
--	SYN: entity work.BRAM
--		port map (
--			challenge => address,
--			clock => clock,
--			wren => write_mem,
--			response => data
--		);
		
	bram_inst : bram 
		PORT MAP (
			address	 => challenge,
			clock	 => clock,
			data	 => data_sig,
			wren	 => wren_sig,
			q	 => q_sig
	);


end architecture top;
