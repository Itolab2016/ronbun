set grid
plot "thrust.txt" u 1:5 w p t "B-motor" , 377.875*x-453.967 w l
set terminal postscript eps  enhanced color
set grid
set key font ',20'
set xlabel "PWM[ms]" font ',20'
set ylabel "thrust[g]" font ',20'
set tics font ',20'
set xrange [1:2]
set yrange [0:300]
set output "thrust-B-kinji.eps"
replot
