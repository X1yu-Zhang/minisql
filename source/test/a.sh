read TMP

g++ $TMP.cpp table.* tuple.* File.* Block.* ./RecordManager/RecordManager.* ./CatalogManager/*  ./IndexManager/BPlusTree.* ./buffermanager/buffermanager.* -o ../$TMP
cd ..
../$TMP > Result.txt