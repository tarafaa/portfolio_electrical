`timescale 1ns / 1ps

module MemoryA(
    input [15:0] Ain,
    input [4:0] addr,
    input WE,
    input clk,
    input rst,
    output reg [15:0] Aout
);

    reg [15:0] A [31:0];

    integer i;
// async rst is included
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            Aout <= 16'd0;

    
            for (i = 0; i < 32; i = i + 1) begin
                A[i] <= 16'd0;
            end
        end
        else begin
            if (WE == 1'b1) begin
                A[addr] <= Ain;
            end
            else begin
                Aout <= A[addr];
            end
        end
    end

endmodule
