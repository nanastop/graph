#!/bin/bash

cd $HOME/trac/tbbsuite/floyd_warshall/
export LD_LIBRARY_PATH=$HOME/tbb/tbb_latest/build/$(cat $HOME/tbb/TBB_LATEST_LIBRARY_VER.txt)

outfile=results_dunnington_24threads.txt
rm $outfile

matsize=4096
for blocksize in 4 8 16 32 64 128 256 512
do
    for xgs in 1 2 4 8 16 32 64 128 256 512
    do
        for ygs in 1 2 4 8 16 32 64 128 256 512
        do
            ./fw_tiled $matsize $blocksize $xgs $ygs 24 >> $outfile
        done
    done
done
