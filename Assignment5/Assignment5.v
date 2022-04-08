module Assignment5(
input wire a,
input wire b,
input wire c,
output wire redled,
output wire greenled,
output wire blueled
);

reg kmap_output;
always @(*)
begin
kmap_output = (a|!c)&(!a|!b);
if(kmap_output==1)
begin
redled=1;
end
else
begin
redled=0;
end
end
endmodule
