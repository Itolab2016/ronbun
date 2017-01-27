set grid
plot "moment.txt" u 1:5 w p t "B-motor" , -24.8396*x+50.263
set terminal postscript eps  enhanced color
set grid
set key font ',20'
set xlabel "PWM[ms]" font ',20'
set ylabel "time[s]" font ',20'
set tics font ',20'
set xrange [1:2]
set yrange [0:35]
set output "moment-B-kinji.eps"
replot
