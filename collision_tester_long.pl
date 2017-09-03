#!/usr/bin/perl

use Time::HiRes qw( time );

# Testing function for time_insert.c and resize_test.py
# Does inserts from 10->10000000

# If you want to see how many items are placed: $tmp_string = "./time_insert $start $start 0" w/o 
# > /dev/null, I did this for cleaner output 

$max = 1000000;

printf("The first timing test to compare my C hash with G_HASH_TABLE.\n");
printf("Running without checks\n");

printf("LARGE HASH\n------------\n");

printf("### Running timing tests for collision_speed.c OPEN_ADDR ###\n");
for ($start = 1000000; $start <= 5000000; $start = $start + 1000000) {
    my $start_time = time();
    $tmp_string = "./collision_speed 1 $start 2";
    system($tmp_string);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}

printf("### Running timing tests for G_HASH_TABLE ###\n");
for ($start = 1000000; $start <= 5000000; $start = $start + 1000000) {
    my $start_time = time();
    $tmp_string = "./test $start";
    system($tmp_string);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}