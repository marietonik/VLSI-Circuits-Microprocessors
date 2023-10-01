module PanelDisplay (
	input logic clk,
	input logic rst,
	output logic hsync,
	output logic vsync,
	output logic [3:0] red,
	output logic [3:0] green,
	output logic [3:0] blue);
  
  // code 
  logic [10:0] linepxl;
  logic [10:0] colpxl;  
  logic pxlClk;
  
  always_ff @(posedge clk) begin
  if (rst)
  pxlClk <= 1'b1;
  else
  pxlClk <= ~pxlClk;
  end

  always_ff @(posedge clk) begin
  if (rst) begin
  linepxl <= 1'b0;
  colpxl  <= 1'b0;
  end
  
  else begin
    if (pxlClk) begin    
        
        if (colpxl == 1039) begin
        colpxl <= 1'b0;
        
            if (linepxl == 665)
            linepxl <= 1'b0;
            else
            linepxl <= linepxl + 1'b1;
            end
            
        else 
        colpxl <= colpxl + 1'b1;
        
        if (colpxl > 856 && colpxl < 976)
            hsync <= 1'b0;
        else
            hsync <= 1'b1;
                
        if (linepxl > 637 && linepxl < 643)
            vsync <= 1'b0;
        else
            vsync <= 1'b1;            
    end
  end
  end

  always_ff @(posedge clk) begin
    if (rst) begin
        red <= 4'b0000;
        green <= 4'b0000;
        blue <= 4'b0000;
    end
    
    else begin    
        if (colpxl < 400 && linepxl < 300) begin
            red <= 4'b1000;
            green <= 4'b0000;
            blue <= 4'b0000;
        end
        else if (colpxl < 800 && linepxl < 300) begin
            red <= 4'b0000;
            green <= 4'b1000;
            blue <= 4'b0000;
        end
        else if (colpxl < 400 && linepxl < 600) begin
            red <= 4'b0000;
            green <= 4'b0000;
            blue <= 4'b1000;
        end
        else if (colpxl < 800 && linepxl < 600) begin
            red <= 4'b1000;
            green <= 4'b1000;
            blue <= 4'b0000;
        end
        else begin
            red <= 4'b0000;
            green <= 4'b0000;
            blue <= 4'b0000;
        end
    end
  end        
endmodule
