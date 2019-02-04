# (Used by plot.sh)

# Plot a RuntimeTest CSV file with gnuplot:
#	gnuplot -e "infiles='mytest1.csv mytest2.csv'" -e "outfile='myoutput.jpg'" -e "title='My tests'" -e "xlabel='order'" -c plot_helper.gp

set title title
set grid xtics ytics
set terminal jpeg size 800,800
set output outfile
if (!exists("ratio")) {
	set xlabel xlabel
	set ylabel ylabel
	set style data lines
	plot for [basename in infiles] basename.".csv" using 1:3 "%lf,%lf,%lf" skip 1 title basename
} else {
	set xlabel xlabel
	set ylabel "factor"
	plot "< paste ".infile1.".csv ".infile2.".csv" using 1:($3/$6) "%lf,%lf,%lf %lf,%lf,%lf" skip 1
}
