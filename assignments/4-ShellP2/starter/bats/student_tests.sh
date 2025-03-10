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
@test "empty command" {
  run ./dsh <<EOF

EOF

  output="${lines[*]}"
  [ "$status" -eq 0 ]
  [[ "$output" =~ "warning: no commands provided" ]]
}


@test "cd no arguments" {
  run ./dsh <<EOF
pwd
cd
pwd
exit
EOF
  output="${lines[*]}"
  [ "$status" -eq 0 ]
  first_pwd=$(echo "$output" | grep -m1 '^/')
  second_pwd=$(echo "$output" | grep '^/' | tail -1)
  [ "$first_pwd" = "$second_pwd" ]
}

@test "cd invalid directory" {
  run ./dsh <<EOF
cd nosuchdir
exit
EOF
  output="${lines[*]}"
  [ "$status" -eq 0 ]
  [[ "$output" =~ "No such file" ]]||[[ "$output" =~ "nosuchdir" ]]
}

@test "echo multiple args" {
  run ./dsh <<EOF
echo one two three
exit
EOF
  output="${lines[*]}"
  [ "$status" -eq 0 ]
  [[ "$output" =~ "one two three" ]]
}

@test "quoted argument" {
  run ./dsh <<EOF
echo "hello world"
exit
EOF
  output="${lines[*]}"
  [ "$status" -eq 0 ]
  [[ "$output" =~ "hello world" ]]
}

@test "unknown command" {
  run ./dsh <<EOF
someinvalidcommand
exit
EOF
  output="${lines[*]}"
  [ "$status" -eq 0 ]
  [[ "$output" =~ "execvp failed" ]]
}

@test "rc after true" {
  run ./dsh <<EOF
true
rc
exit
EOF
  output="${lines[*]}"
  [ "$status" -eq 0 ]
  [[ "$output" =~ $'0' ]]
}

@test "rc after false" {
  run ./dsh <<EOF
false
rc
exit
EOF
  output="${lines[*]}"
  [ "$status" -eq 0 ]
  [[ "$output" =~ $'1' ]]
}

@test "exit built-in" {
  run ./dsh <<EOF
exit
EOF
  [ "$status" -eq 0 ]
}
