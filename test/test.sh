#!/bin/bash

set -ex

BIN_DIR=$1
DATA_DIR=$2

"$BIN_DIR"/currency_convert_v0 "$DATA_DIR"/sample_v0.conv eurusd 2012-12-12 1
"$BIN_DIR"/currency_convert_v0 "$DATA_DIR"/sample_v1.conv eurusd 2012-12-12 1
"$BIN_DIR"/currency_convert_v1 "$DATA_DIR"/sample_v0.conv eurusd 2012-12-12 1
"$BIN_DIR"/currency_convert_v1 "$DATA_DIR"/sample_v1.conv eurusd 2012-12-12 1
"$BIN_DIR"/currency_convert_v1 "$DATA_DIR"/sample_v1.conv eurjpy 2012-12-12 1

# these don't
! "$BIN_DIR"/currency_convert_v0 "$DATA_DIR"/sample_v1.conv eurjpy 2012-12-12 1
! "$BIN_DIR"/currency_convert_v1 "$DATA_DIR"/sample_v1.conv eurusd 2020-01-01 1
