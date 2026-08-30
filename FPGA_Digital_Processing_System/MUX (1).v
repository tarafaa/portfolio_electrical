`timescale 1ns / 1ps

module MUX(
    input [15:0] Btotal,
    input valid,
    output reg [15:0] Bi
);

    always @(*) begin
        if (valid == 1'b1) begin
            Bi = Btotal;
        end
        else begin
            Bi = 16'b0;
        end
    end

endmodule
