`timescale 1ns / 1ps

module RCA2(
    input signed [31:0] Yin,
    input signed [31:0] product,
    output signed [31:0] Yout
    );
    
    assign Yout = Yin + product;
    
endmodule
