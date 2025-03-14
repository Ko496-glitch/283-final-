#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Check pwd runs without errors" {
    run ./dsh <<EOF
pwd
EOF
    [ "$status" -eq 0 ]
}
@test "Check whoami runs without errors" {
    run ./dsh <<EOF
whoami
EOF
    [ "$status" -eq 0 ]
}

@test "Check echo command" {
    run ./dsh <<EOF
    echo "Hello World"
EOF
    echo "DEBUG: Output is [$output]"
    [ "$status" -eq 0 ]
    [[ "$output" =~ "Hello World" ]] 
}

@test "Check cd changes directory" {
    run ./dsh <<EOF
cd ..
pwd
EOF
    [ "$status" -eq 0 ]
}

@test "Check command piping (ls | grep)" {
    run ./dsh <<EOF
ls | grep dsh
EOF
    [ "$status" -eq 0 ]
}
@test "Check exit command" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}
@test "Check stop-server command" {
    run ./dsh <<EOF
stop-server
EOF
    [ "$status" -eq 0 ]
}
@test "Check sequential commands execution" {
    run ./dsh <<EOF
pwd
ls
whoami
EOF
    [ "$status" -eq 0 ]
}