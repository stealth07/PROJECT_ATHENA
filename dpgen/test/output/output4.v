`timescale 1ns/1ps
//Author: Generated by yaaaay!! dpgen
module test/output4(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, final);
	input 	signed	[7:0]	a;
	input 	signed	[7:0]	b;
	input 	signed	[7:0]	c;
	input 	signed	[7:0]	d;
	input 	signed	[7:0]	e;
	input 	signed	[7:0]	f;
	input 	signed	[7:0]	g;
	input 	signed	[7:0]	h;
	input 	signed	[7:0]	i;
	input 	signed	[7:0]	j;
	input 	signed	[7:0]	k;
	input 	signed	[7:0]	l;
	input 	signed	[7:0]	m;
	input 	signed	[7:0]	n;
	input 	signed	[7:0]	o;
	input 	signed	[7:0]	p;
	output 	signed	[31:0]	final;
	wire 	signed	[31:0]	t1;
	wire 	signed	[31:0]	t2;
	wire 	signed	[31:0]	t3;
	wire 	signed	[31:0]	t4;
	wire 	signed	[31:0]	t5;
	wire 	signed	[31:0]	t6;
	wire 	signed	[31:0]	t7;
	wire 	signed	[31:0]	t8;
	wire 	signed	[31:0]	t9;
	wire 	signed	[31:0]	t10;
	wire 	signed	[31:0]	t11;
	wire 	signed	[31:0]	t12;
	wire 	signed	[31:0]	t13;
	wire 	signed	[31:0]	t14;
	add #(.DATAWIDTH(32))	ADD0(a[7]?{{24{1'd1}},a}:{{24{1'd0}},a}, b[7]?{{24{1'd1}},b}:{{24{1'd0}},b}, t1);
 	add #(.DATAWIDTH(32))	ADD1(t1, c[7]?{{24{1'd1}},c}:{{24{1'd0}},c}, t2);
 	add #(.DATAWIDTH(32))	ADD2(t2, d[7]?{{24{1'd1}},d}:{{24{1'd0}},d}, t3);
 	add #(.DATAWIDTH(32))	ADD3(t3, e[7]?{{24{1'd1}},e}:{{24{1'd0}},e}, t4);
 	add #(.DATAWIDTH(32))	ADD4(t4, f[7]?{{24{1'd1}},f}:{{24{1'd0}},f}, t5);
 	add #(.DATAWIDTH(32))	ADD5(t5, g[7]?{{24{1'd1}},g}:{{24{1'd0}},g}, t6);
 	add #(.DATAWIDTH(32))	ADD6(t6, h[7]?{{24{1'd1}},h}:{{24{1'd0}},h}, t7);
 	add #(.DATAWIDTH(32))	ADD7(t7, i[7]?{{24{1'd1}},i}:{{24{1'd0}},i}, t8);
 	add #(.DATAWIDTH(32))	ADD8(t8, j[7]?{{24{1'd1}},j}:{{24{1'd0}},j}, t9);
 	add #(.DATAWIDTH(32))	ADD9(t9, l[7]?{{24{1'd1}},l}:{{24{1'd0}},l}, t10);
 	add #(.DATAWIDTH(32))	ADD10(t10, m[7]?{{24{1'd1}},m}:{{24{1'd0}},m}, t11);
 	add #(.DATAWIDTH(32))	ADD11(t11, n[7]?{{24{1'd1}},n}:{{24{1'd0}},n}, t12);
 	add #(.DATAWIDTH(32))	ADD12(t12, o[7]?{{24{1'd1}},o}:{{24{1'd0}},o}, t13);
 	add #(.DATAWIDTH(32))	ADD13(t13, p[7]?{{24{1'd1}},p}:{{24{1'd0}},p}, t14);
 	assign final = t14;
endmodule