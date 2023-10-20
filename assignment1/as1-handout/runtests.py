#!/usr/bin/env python3
# This python script  exercises the complex addition, subtraction,
# multiplication, and division operations for the complex calculator.
# I've attempted to use representative numbers for all four operators
# and every combination involving 0.

# Note that this test program does NOT exercise the error detection
# requirement.

import subprocess
import sys

# Here's the program name:
if len(sys.argv) != 2:
    print("Usage: runtests.py prog_to_test")
    sys.exit(1)

prog = sys.argv[1]  # set the program to test

# Here is the structure of the test tuples:
# ( cmd_string, expected_result_string )
testvectors = (
    # accepts lower case?
    ("a 4 5 6 7", "10 + j 12"),
    ("s 4 5 6 7", "-2 - j 2"),
    ("m 4 5 6 7", "-11 + j 58"),
    ("d 4 5 6 7", "0.694118 + j 0.0235294"),
    
    # accepts upper case?
    ("A -0.47 0.37 -0.43 0.23", "-0.9 + j 0.6"),
    ("S 0.43 0.48 0.17 0.08", "0.26 + j 0.4"),
    ("M 0.13 0.16 0.00 -0.50", "0.08 - j 0.065"),
    ("D 0.40 -0.49 0.04 0.36", "-1.22256 - j 1.24695"),

    # allows leading or trailing whitespace?
    ("    a 4 5 6 7", "10 + j 12"),
    ("\t s 4 5 6 7", "-2 - j 2"),
    ("m 4 5 6 7    ", "-11 + j 58"),
    ("d 4 5 6 7   \t\t", "0.694118 + j 0.0235294"),

    # some fairly intuitive tests
    ("M -0.7071 0.7071 0.7071 0.7071", "-0.999981 + j 0"),
    ("D -0.7071 .7071 .7071 .7071", "0 + j 1"),
    ("M 0 0 0.07303 0.16963", "0 + j 0"),
    ("D 0 0 0.46661 -0.14840", "0 + j 0"),
    ("d 4 0 2 0", "2 + j 0"),
    ("d 0 4 0 2", "2 + j 0"),
    ("A 0 0 0 0", "0 + j 0"),
    ("S 0 0 0 0", "0 + j 0"),
    ("M 0 0 0 0", "0 + j 0"),
    
    # recognizes illegal commands
    ("B 1 2 3 4", "error code: 1: illegal command"),
    # deal with too few arguments
    ("a 1 2 3  ", "error code: 2: missing arguments"),
    # deal with too many arguments
    ("a 1 2 3 4 5 ", "error code: 3: extra arguments"),
    # deal with divide by zero
    ("d 1 2 0 0 ", "error code: 4: divide by zero"),
)

def is_equal(got, expected):
    if got.startswith("error"):
        return got.split(None, 3) == expected.split(None, 3)

    def convert_to_complex(result_string):
        (re, plusminus, jay, im) = result_string.split()
        if plusminus == "+": sign = 1
        else: sign = -1
        complex_result = complex(float(re), sign*float(im))
        return complex_result
        
    got = convert_to_complex(got)
    expected = convert_to_complex(expected)

    TOL = 1e-6

    if abs(expected) > TOL:
        error = (got-expected) / expected
    else:
        error = (got-expected)
    if abs( error ) < TOL:
        return True

    return False

def connect_to_prog(prog):
    pipe = subprocess.Popen(prog, stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    return pipe

tests_passed = 0  # accumulate passes and failures
tests_failed = 0
for n,vec in enumerate(testvectors):
    pipe = connect_to_prog(prog)
    command = bytes(vec[0] + "\nq\n", "utf-8")
    (got_output, eee) = pipe.communicate(command)
    got_output = str(got_output, "utf-8")
    expected_output = vec[1]
    
    try:
        if is_equal(got_output, expected_output):
            tests_passed += 1
            print(".", end="")
        else:
            tests_failed += 1
            print(f"\ntest# {n+1}: Tolerance ERROR")
            print("   input: ", vec[0])
            print("expected: ", vec[1])
            print("     got: ", got_output)
            #sys.exit(2)
    except:
        tests_failed += 1
        print(f"\ntest#{n+1}: Exception: ")
        print("   input: ", vec[0])
        print("expected: ", vec[1])
        print("     got: ", got_output)
        #sys.exit(2)

print(f"\nSummary: passed: {tests_passed}, failed: {tests_failed}")
print("OK")
