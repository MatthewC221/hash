#!/usr/bin/perl

printf("### Running tests for collision_speed.c OPEN_ADDR ###\n");
for ($start = 0; $start < 100; $start++) {
    $tmp_string = "./collision_speed 1 10 2";
    system($tmp_string);
    sleep(1);
}