selected=$1
if [ $selected -eq 1 ]
then
    echo "Selected: Cassandra"
    cd cassandra
    make
    mv bloom_filter ..
    cd ..
    echo "Cassandra is compiled."
else
    echo -e "Please select a valid option.\n1. Cassandra\n"
fi