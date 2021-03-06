#!/usr/bin/perl

use Time::HiRes qw( time );

# Testing function for time_insert.c and resize_test.py
# Does inserts from 10->10000000

# If you want to see how many items are placed: $tmp_string = "./time_insert $start $start 0" w/o 
# > /dev/null, I did this for cleaner output 

$max = 1000000;

printf("The first timing test to compare my C hash with G_HASH_TABLE.\n");
printf("Running without checks\n");
=pod
printf("### Running timing tests for collision_speed.c COLLISION ###\n");
for ($start = 10; $start <= $max; $start = $start * 10) {
    my $start_time = time();
    $tmp_string = "./collision_speed 1 $start 1";
    system($tmp_string);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}
=cut

printf("### Running timing tests for collision_speed.c OPEN_ADDR ###\n");
for ($start = 10; $start <= $max; $start = $start * 10) {
    my $start_time = time();
    $tmp_string = "./collision_speed 1 $start 2";
    system($tmp_string);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}

printf("### Running timing tests for G_HASH_TABLE ###\n");
for ($start = 10; $start <= $max; $start = $start * 10) {
    my $start_time = time();
    $tmp_string = "./glib_collision_speed $start";
    system($tmp_string);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}
