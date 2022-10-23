//---------------------------------------------------------------------------------------
// uart to internal bus top module 
//
//---------------------------------------------------------------------------------------
/*

 uart2bus_top  #(.ClkFrequency(ClkFrequency),.Baud(Baud))  uart2bus_top 
(
	// global signals 
	.clock( clk ), 
	.reset( rst ),
	// uart serial signals 
	.ser_in( rxd ), 
	.ser_out( txd ),
	// internal bus to register file 
	.int_address( addr), 
	.int_wr_data( wr_dat), 
	.int_write( wr_en),
	.int_rd_data( rd_dat), 
	.int_read( rd_req), 
	.int_req( ), 
	.int_gnt (1'b1) 
);

*/

module uart2bus_top 
(
	// global signals 
	clock, reset,
	// uart serial signals 
	ser_in, ser_out,
	// internal bus to register file 
	int_address, int_wr_data, int_write,
	int_rd_data, int_read, 
	int_req, int_gnt 
);
//---------------------------------------------------------------------------------------
// modules inputs and outputs 
input 			clock;			// global clock input 
input 			reset;			// global reset input 
input			ser_in;			// serial data input 
output			ser_out;		// serial data output 
output	[15:0]	int_address;	// address bus to register file 
output	[7:0]	int_wr_data;	// write data to register file 
output			int_write;		// write control to register file 
output			int_read;		// read control to register file 
input	[7:0]	int_rd_data;	// data read from register file 
output			int_req;		// bus access request signal 
input			int_gnt;		// bus access grant signal 


parameter ClkFrequency = 100000000;	// 100MHz
parameter Baud = 115200;
 
// internal wires 
wire	[7:0]	tx_data;		// data byte to transmit 
wire			new_tx_data;	// asserted to indicate that there is a new data byte for transmission 
wire 			tx_busy;		// signs that transmitter is busy 
wire	[7:0]	rx_data;		// data byte received 
wire 			new_rx_data;	// signs that a new byte was received 


//---------------------------------------------------------------------------------------
// module implementation 
// uart top module instance
 
uart_top uart1
(
	.clock(clock), .reset(reset),
	.ser_in(ser_in), .ser_out(ser_out),
	.rx_data(rx_data), .new_rx_data(new_rx_data), 
	.tx_data(tx_data), .new_tx_data(new_tx_data), .tx_busy(tx_busy)
//,	.baud_freq(baud_freq), .baud_limit(baud_limit),
//	.baud_clk(baud_clk) 
);
 
// uart parser instance 
uart_parser #(16) uart_parser1
(
	.clock(clock), .reset(reset),
	.rx_data(rx_data), .new_rx_data(new_rx_data), 
	.tx_data(tx_data), .new_tx_data(new_tx_data), .tx_busy(tx_busy), 
	.int_address(int_address), .int_wr_data(int_wr_data), .int_write(int_write),
	.int_rd_data(int_rd_data), .int_read(int_read), 
	.int_req(int_req), .int_gnt(int_gnt) 
);

endmodule
//---------------------------------------------------------------------------------------
//						Th.. Th.. Th.. Thats all folks !!!
//---------------------------------------------------------------------------------------

