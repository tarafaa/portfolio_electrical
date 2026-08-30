`timescale 1ns / 1ps

module MemoryC(
    input [31:0] Cin,
    input [4:0] addr,
    input WE,
    input clk,
    input rst,
    output reg [31:0] Cout
);

    reg [31:0] C [0:31];

    integer i;

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            Cout <= 32'd0;

            // async included
            for (i = 0; i < 32; i = i + 1) begin
                C[i] <= 32'd0;
            end
        end
        else begin
            if (WE)
                C[addr] <= Cin;
            else
                Cout <= C[addr];
        end
    end

endmodule
