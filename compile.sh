selected=$1
if [ $selected -eq 1 ]
then
    echo "Selected: Cassandra"
    cd cassandra
    make
    mv bloom_filter ..
    cd ..
    echo "Cassandra is compiled."
elif [ $selected -eq 2 ]
then
    echo "Selected: RockDB"
    cd RockDB
    make
    mv bloom_filter ..
    echo "RockDB is compiled."
else
    echo -e "Please select a valid option.\n1. Cassandra\n2. RockDB"
fi