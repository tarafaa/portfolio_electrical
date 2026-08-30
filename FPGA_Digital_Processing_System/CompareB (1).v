`timescale 1ns / 1ps

module CompareB(
    input [15:0] Bi,
    input clk,
    input rst,

    output reg [15:0] Bn,
    output [15:0] Btotal,
    output reg done
);

    reg [4:0] k;

    wire [4:0] mem_addr;

    assign mem_addr = Bn[4:0];

    MemoryB u_bm0 (
        .Bin(16'd0),
        .addr(mem_addr),
        .WE(1'b0),
        .clk(clk),
        .rst(rst),
        .Bout(Btotal)
    );

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            k <= 5'd0;
            Bn <= 16'd0;
            done <= 1'b0;
        end
        else begin

            if (Bi >= k) begin
                Bn <= Bi - k;
                k <= k + 1'b1;
                done <= 1'b0;
            end
            else begin
                done <= 1'b1;
            end

        end
    end

endmodule
