set terminal png
set output "test2.png"

set title "TLB benchmark, 4 KiByte pages, 10 Gi operations"

set key right center

set xlabel "number of pages"

set ylabel "time in s"

# use log scale if we use double of number of pages
set logscale x 2

plot "test2.dat" u 1:2 w linespoints title "page refs"