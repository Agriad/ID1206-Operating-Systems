set terminal png
set output "greenthread amount.png"

set title "Thread amount and completion time"

set key right top

set xlabel "Amount of Threads"

set ylabel "Time in seconds"

set style data linespoint

plot "greenthread_out_thread.dat" notitle pointtype 7