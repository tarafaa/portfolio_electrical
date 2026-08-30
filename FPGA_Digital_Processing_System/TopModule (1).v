`timescale 1ns / 1ps

module TopModule(
    input clk,
    input rst,
    input start,

    input loadA,
    input loadB,
    input [4:0] load_addr,
    input signed [15:0] Ain,
    input signed [15:0] Bin,

    output reg done,
    output reg [5:0] Y_n,
    output reg signed [31:0] Y
);

    wire signed [15:0] Aout;
    wire signed [15:0] Bout;
    wire signed [31:0] product;

    reg [4:0] addrA;
    reg [4:0] addrB;

    reg [5:0] n;
    reg [1:0] k;
    reg signed [31:0] acc;

    reg [2:0] state;

    localparam IDLE = 3'd0;
    localparam READ = 3'd1;
    localparam WAIT = 3'd2;
    localparam ACC  = 3'd3;
    localparam OUT  = 3'd4;

    MemoryA memA (
        .Ain(Ain),
        .addr(loadA ? load_addr : addrA),
        .WE(loadA),
        .clk(clk),
        .rst(rst),
        .Aout(Aout)
    );

    MemoryB memB (
        .Bin(Bin),
        .addr(loadB ? load_addr : addrB),
        .WE(loadB),
        .clk(clk),
        .rst(rst),
        .Bout(Bout)
    );

    Multiplier mult (
        .A(Aout),
        .Bi(Bout),
        .product(product)
    );

    always @(posedge clk or posedge rst) begin
        if (rst) begin
            done <= 1'b0;
            Y <= 32'sd0;
            Y_n <= 6'd0;

            addrA <= 5'd0;
            addrB <= 5'd0;

            n <= 6'd0;
            k <= 2'd0;
            acc <= 32'sd0;
            state <= IDLE;
        end
        else begin
            case (state)

                IDLE: begin
                    done <= 1'b0;

                    if (start) begin
                        n <= 6'd0;
                        k <= 2'd0;
                        acc <= 32'sd0;

                        addrA <= 5'd0;
                        addrB <= 5'd0;

                        state <= READ;
                    end
                end

                READ: begin
                    if ((n >= k) && ((n - k) < 32)) begin
                        addrA <= n - k;
                        addrB <= k;
                    end
                    else begin
                        addrA <= 5'd0;
                        addrB <= 5'd0;
                    end

                    state <= WAIT;
                end

                WAIT: begin
                    state <= ACC;
                end

                ACC: begin
                    if ((n >= k) && ((n - k) < 32)) begin
                        acc <= acc + product;
                    end

                    if (k == 2'd1) begin
                        state <= OUT;
                    end
                    else begin
                        k <= k + 1'b1;
                        state <= READ;
                    end
                end

                OUT: begin
                    Y <= acc;
                    Y_n <= n;

                    acc <= 32'sd0;
                    k <= 2'd0;

                    if (n == 6'd32) begin
                        done <= 1'b1;
                        state <= IDLE;
                    end
                    else begin
                        n <= n + 1'b1;
                        state <= READ;
                    end
                end

            endcase
        end
    end

endmodule
