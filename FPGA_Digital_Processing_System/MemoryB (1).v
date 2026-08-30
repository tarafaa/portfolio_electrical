`timescale 1ns / 1ps

module MemoryB(
    input [15:0] Bin,
    input [4:0] addr,
    input WE,
    input clk,
    input rst,
    output reg [15:0] Bout
);

    reg [15:0] B [31:0];

    integer i;

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            Bout <= 16'd0;

            for (i = 0; i < 32; i = i + 1) begin
                B[i] <= 16'd0;
            end
        end
        else begin
            if (WE == 1'b1) begin
                B[addr] <= Bin;
            end
            else begin
                Bout <= B[addr];
            end
        end
    end

endmodule
