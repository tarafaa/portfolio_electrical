`timescale 1ns / 1ps

module TopModule_TB;

    reg clk = 0;
    reg rst = 0;
    reg start = 0;

    reg loadA = 0;
    reg loadB = 0;
    reg [4:0] load_addr = 0;

    reg signed [15:0] Ain = 0;
    reg signed [15:0] Bin = 0;

    wire done;
    wire [5:0] Y_n;
    wire signed [31:0] Y;

    integer i = 0;

    TopModule DUT (
        .clk(clk),
        .rst(rst),
        .start(start),
        .loadA(loadA),
        .loadB(loadB),
        .load_addr(load_addr),
        .Ain(Ain),
        .Bin(Bin),
        .done(done),
        .Y_n(Y_n),
        .Y(Y)
    );

    always #5 clk = ~clk;

    initial begin
        $display("Starting simulation...");

        rst = 1;
        #20;
        rst = 0;

        // Load Memory A: x = 1, 2, 3, ..., 32
        for (i = 0; i < 32; i = i + 1) begin
            @(negedge clk);
            loadA = 1;
            loadB = 0;
            load_addr = i[4:0];
            Ain = i + 1;
        end

        @(negedge clk);
        loadA = 0;

        // Load Memory B: h = [1, 2, 0, 0, ...]
        for (i = 0; i < 32; i = i + 1) begin
            @(negedge clk);
            loadA = 0;
            loadB = 1;
            load_addr = i[4:0];

            if (i == 0)
                Bin = 16'd1;
            else if (i == 1)
                Bin = 16'd2;
            else
                Bin = 16'd0;
        end

        @(negedge clk);
        loadB = 0;

        repeat(2) @(posedge clk);

        // Start convolution
        @(negedge clk);
        start = 1;

        @(negedge clk);
        start = 0;

        // Display outputs
        $display("Index | Y");
        $display("----------");

        while (!done) begin
            @(posedge clk);
            #1;
            $display("%0d | %0d", Y_n, Y);
        end

        #1;
        $display("%0d | %0d", Y_n, Y);
        $display("Convolution complete.");

        #20;
        $finish;
    end

endmodule
