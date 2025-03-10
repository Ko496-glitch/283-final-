#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run "./dsh" <<EOF
ls
EOF
    [ "$status" -eq 0 ]
}

@test "Echo pipe test (two commands)" {
    run "./dsh" <<EOF
echo "Hello World" | grep World
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "Hello World" ]]
}

@test "Three-stage pipeline (simple, no quotes)" {
    run "./dsh" <<EOF
printf "one\\ntwo\\nthree\\n" | grep two | sort
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "one two three" || "$output" =~ "two" ]]
}


@test "Pipeline command limit (exactly 8 commands)" {
    run "./dsh" <<EOF
echo test | cat | cat | cat | cat | cat | cat | cat
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "test" ]]
}

@test "Pipeline exceeds command limit" {
    run "./dsh" <<EOF
echo test | cat | cat | cat | cat | cat | cat | cat | cat
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "error: piping limited to 8 commands" ]]
}

@test "Built-in command in pipeline should fail gracefully" {
    run "./dsh" <<EOF
echo test | cd /tmp | cat
EOF
    [ "$status" -eq 0 ]
    [[ "$output" =~ "failed" || "$output" =~ "error" ]]
}
