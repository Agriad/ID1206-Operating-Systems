set terminal png
set output "fifth_output.png"

set title "Time for the different algorithms"

set key right top

set xlabel "Algorithms"

set ylabel "Time in milliseconds"

set boxwidth 0.5
set style fill solid

plot "output_clean.dat" using 1:3:xtic(2) with boxes notitle