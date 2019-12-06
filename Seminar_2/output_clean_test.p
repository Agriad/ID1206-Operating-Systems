# set terminal pngcairo  transparent enhanced font "arial,10" fontscale 1.0 size 600, 400 
# set output 'datastrings.5.png'
set boxwidth 0.3 absolute
set style fill   solid 1.00 border lt -1
set grid nopolar
set grid noxtics nomxtics ytics nomytics noztics nomztics nortics nomrtics \
 nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics
set grid layerdefault   lt 0 linecolor 0 linewidth 0.500,  lt 0 linecolor 0 linewidth 0.500
set key fixed right top vertical Right noreverse enhanced autotitle columnhead nobox
set style increment default
set pointsize 2
set xtics border in scale 0,0 mirror norotate  autojustify
set xtics  norangelimit 
set xtics   ()
set ytics border in scale 0,0 mirror norotate  autojustify
set ztics border in scale 0,0 nomirror norotate  autojustify
set cbtics border in scale 0,0 mirror norotate  autojustify
set rtics axis in scale 0,0 nomirror norotate  autojustify
set title "Plot actual y-value as a label" 
set xrange [ -0.500000 : 11.5000 ] noreverse nowriteback
set x2range [ * : * ] noreverse writeback
set ylabel "mm" 
set yrange [ 0.00000 : 180.000 ] noreverse nowriteback
set y2range [ * : * ] noreverse writeback
set zrange [ * : * ] noreverse writeback
set cbrange [ * : * ] noreverse writeback
set rrange [ * : * ] noreverse writeback
set bmargin  3
## Last datafile plotted: "table.dat"
plot 'output_clean.dat' notitle
