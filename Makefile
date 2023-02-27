########################################################################
#
# Copyright (C), 2023, YADRO Development Center, LLC («YADRO»).
# All Rights Reserved.
#
# This software contains the intellectual property of YADRO
# or is licensed to YADRO from third parties. Use of this
# software and the intellectual property contained therein is expressly
# limited to the terms and conditions of the License Agreement under which
# it is provided by YADRO.
#
########################################################################

# ******************************************************************************
# * INCLUDES
# ******************************************************************************

# TODO add includes if needed

# ******************************************************************************
# * FUNCTIONS
# ******************************************************************************

# TODO add functions here

# ******************************************************************************
# * VARIABLES
# ******************************************************************************

# TODO add variables here

# ******************************************************************************
# * TARGETS - MANDATORY
# ******************************************************************************

# Init repository / project
init:
	echo "Not implemented"
	exit 1


# Apply codestyle to all codebase
style_apply:
	echo "Not implemented"
	exit 1


# Check codestyle and return error if ther ere some violence, generate a report into ./artifacts 
style_analyze:
	echo "Not implemented"
	exit 1


# Run unit tests, generate a test report and a gcov report into ./artifacts 
test_unit:
	ceedling test:all
	echo "Reports not implemented"
	exit 1

# Run static analyzers, generate an analyze report into ./artifacts 
static_analyze:
	echo "Not implemented"
	exit 1


# Run security analyzers, generate an analyze report into ./artifacts 
security_analyze:
	echo "Not implemented"
	exit 1


# Build project in debug configuration
build_debug:
	echo "Not implemented"
	exit 1


# Build project in release configuraiton
build_release:
	echo "Not implemented"
	exit 1


# Test on the emulator, generate a test report into ./artifacts 
test_on-emulator:
	echo "Not implemented"
	exit 1


# # Test on the test stand, generate a test report into ./artifacts 
test_on-stand:
	echo "Not implemented"
	exit 1


# Build docs and generate a docs bundle into ./artifacts
docs_build:
	doxygen doxygen_config


# Run debug workflow
all_debug:
	init build_debug


# Run release workflow
all_release:
	init style_analyze test_unit static_analyze build_release test_on-emulator test_on-stand docs_build security_analyze

# ******************************************************************************
# * TARGETS - PROJECT-SPECIFIC
# ******************************************************************************

# TODO add project-specific targets

# ******************************************************************************
# * END OF MAKEFILE
# ******************************************************************************
