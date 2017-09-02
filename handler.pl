#!/usr/bin/perl

use Time::HiRes qw( time );

# Testing function for time_insert.c and resize_test.py
# Does inserts from 10->10000000

# If you want to see how many items are placed: $tmp_string = "./time_insert $start $start 0" w/o 
# > /dev/null, I did this for cleaner output 

$max = 1000000;

=pod
printf("The first timing test to compare my C hash with Python's dictionary.\n");
printf("Running without checks\n");
printf("### Running timing tests for time_insert.c COLLISION ###\n");
for ($start = 10; $start <= $max; $start = $start * 10) {
    my $start_time = time();
    $tmp_string = "./time_test $start 0 0 1 1 >/dev/null 2>&1";
    system($tmp_string);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}
=cut

printf("### Running timing tests for time_insert.c OPEN_ADDR DEFAULT ###\n");
for ($start = 10; $start <= $max; $start = $start * 10) {
    my $start_time = time();
    $tmp_string = "./time_test $start 0 0 5 2 >/dev/null 2>&1";
    system($tmp_string);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}

printf("### Running timing tests for time_insert.c OPEN_ADDR STRING VALS ###\n");
for ($start = 10; $start <= $max; $start = $start * 10) {
    my $start_time = time();
    $tmp_string = "./time_test_str $start 0 0 5 2 >/dev/null 2>&1";
    system($tmp_string);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}


=pod
printf("### Running timing tests for time_insert.c COLLISION WITH size initialising ###\n");
for ($start = 10; $start <= $max; $start = $start * 10) {
    my $start_time = time();
    $tmp_string = "./time_test $start 0 0 $start 1 >/dev/null 2>&1";
    system($tmp_string);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}
=cut

printf("### Running timing tests for time_insert.c OPEN_ADDR WITH size initialising ###\n");
for ($start = 10; $start <= $max; $start = $start * 10) {
    my $start_time = time();
    $tmp_string = "./time_test $start 0 0 $start 2 >/dev/null 2>&1";
    system($tmp_string);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}

printf("\n### Running timing tests for time_insert.py ###\n");
for ($start = 10; $start <= $max; $start = $start * 10) {
    my $start_time = time();
    $tmp_string2 = "./time_insert.py $start 0 0 >/dev/null 2>&1";
    system($tmp_string2);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}

printf("Running with checks\n");
printf("### Running timing tests for time_insert.c OPEN_ADDR ###\n");
for ($start = 10; $start <= $max; $start = $start * 10) {
    my $start_time = time();
    $tmp_string = "./time_test $start 1 0 1 2 >/dev/null 2>&1";
    system($tmp_string);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}

=pod
printf("### Running timing tests for time_insert.c COLLISION ###\n");
for ($start = 10; $start <= $max; $start = $start * 10) {
    my $start_time = time();
    $tmp_string = "./time_test $start 1 0 1 1 >/dev/null 2>&1";
    system($tmp_string);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}
=cut

printf("### Running timing tests for time_insert.py ###\n");
for ($start = 10; $start <= $max; $start = $start * 10) {
    my $start_time = time();
    $tmp_string2 = "./time_insert.py $start 1 0 >/dev/null 2>&1";
    system($tmp_string2);
    printf("Time elapsed for $start inserts:%.2f\n", time() - $start_time);
    printf("------------\n");
}

# printf("### Running timing tests for ")
