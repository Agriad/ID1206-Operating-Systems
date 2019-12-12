set terminal png
set output "greenthread vs pthread.png"

set title "Comparison between green thread and pthread"

set key right top

set xlabel "Iterations"

set ylabel "Time in seconds"

set logscale xy

set style data linespoint

plot "greenthread_out.dat" title "green thread" pointtype 7,\
     "pthread_out.dat" title "pthread"