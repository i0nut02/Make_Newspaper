#!/usr/bin/sh

TOTAL_TESTS=0
PASSED_TESTS=0

run_test() {
    rm -f output.txt
    touch output.txt

    if $7 = true
    then
        printf "Test $1 (par): "
        ./newspaper $1 output.txt $3 $4 $5 $6 -p
    else
        printf "Test $1 (not par): "
        ./makeNewspaper $1 $3 $4 $5 $6
    fi

    sleep .25

    let TOTAL_TESTS++
}

# non-parallel tests
run_test sample_inputs/utf8_prof_in.txt expected_outputs/utf8_prof_in_JOURNAL.txt 3 40 21 10 false
run_test sample_inputs/lorem_long_in.txt expected_outputs/lorem_long_in_JOURNAL.txt 3 40 21 10 false
run_test sample_inputs/lorem_short_in.txt expected_outputs/lorem_short_in_JOURNAL.txt 3 4 5 1 false
run_test sample_inputs/capo_in.txt expected_outputs/capo_in_JOURNAL.txt 3 7 15 1 false
run_test sample_inputs/spacing_in.txt expected_outputs/spacing_in_JOURNAL.txt 1 7 18 1 false
run_test sample_inputs/spacing2_in.txt expected_outputs/spacing2_in_JOURNAL.txt 2 7 18 1 false
run_test sample_inputs/super_short_in.txt expected_outputs/super_short_in_JOURNAL.txt 100 7 21 1 false
run_test sample_inputs/utf8_ending_in.txt expected_outputs/utf8_ending_in_JOURNAL.txt 3 8 4 10 false
run_test sample_inputs/nothing_in.txt expected_outputs/nothing_in_JOURNAL.txt 3 3 4 10 false
run_test sample_inputs/makefile_in.txt expected_outputs/makefile_in_JOURNAL.txt 3 8 16 10 false
run_test sample_inputs/example_in.txt expected_outputs/example_in_JOURNAL.txt 3 5 15 5 false
run_test sample_inputs/example2_in.txt expected_outputs/example2_in_JOURNAL.txt 3 5 15 5 false
run_test sample_inputs/example3_in.txt expected_outputs/example3_in_JOURNAL.txt 3 5 15 5 false
run_test sample_inputs/ionut_in.txt expected_outputs/ionut_in_JOURNAL.txt 3 40 60 10 false
run_test sample_inputs/dante_in.txt expected_outputs/dante_in_JOURNAL.txt 5 40 20 5 false
exit
echo ""

# parallel-tests
run_test sample_inputs/utf8_prof_in.txt expected_outputs/utf8_prof_out.txt 3 40 21 10 true
run_test sample_inputs/lorem_long_in.txt expected_outputs/lorem_long_out.txt 3 40 21 10 true
run_test sample_inputs/lorem_short_in.txt expected_outputs/lorem_short_out.txt 3 4 5 1 true
run_test sample_inputs/capo_in.txt expected_outputs/capo_out.txt 3 7 15 1 true
run_test sample_inputs/spacing_in.txt expected_outputs/spacing_out.txt 1 7 18 1 true
run_test sample_inputs/spacing2_in.txt expected_outputs/spacing2_out.txt 2 7 18 1 true
run_test sample_inputs/super_short_in.txt expected_outputs/super_short_out.txt 100 7 21 1 true
run_test sample_inputs/utf8_ending_in.txt expected_outputs/utf8_ending_out.txt 3 8 4 10 true
run_test sample_inputs/nothing_in.txt expected_outputs/nothing_out.txt 3 3 4 10 true
run_test sample_inputs/makefile_in.txt expected_outputs/makefile_out.txt 3 8 16 10 true
run_test sample_inputs/example_in.txt expected_outputs/example_out.txt 3 5 15 5 true
run_test sample_inputs/example2_in.txt expected_outputs/example2_out.txt 3 5 15 5 true
run_test sample_inputs/example3_in.txt expected_outputs/example3_out.txt 3 5 15 5 true
run_test sample_inputs/ionut_in.txt expected_outputs/ionut_out.txt 3 40 60 10 true
run_test sample_inputs/dante_in.txt expected_outputs/dante_out.txt 5 40 20 5 true

printf "\n[$PASSED_TESTS/$TOTAL_TESTS] tests passed.\n"