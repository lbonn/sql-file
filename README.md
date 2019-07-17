# SQL-file

This is a small example application to illustrate how to manage an application using SQLite as a file format.

Relevant link: [https://www.sqlite.org/appfileformat.html](https://www.sqlite.org/appfileformat.html)

# Demo

`sample_v0.conv` contains conversion rates from euro to usd, in the v0 format
`sample_v1.conv` contains conversion rates from euro to usd and euro to yen, in the v1 format

```
mkdir build
(cd build && cmake .. -GNinja && cmake --build . && ctest -V)

# all of these work
./build/currency_convert_v0 data/sample_v0.conv eurusd 2012-12-12 1
./build/currency_convert_v0 data/sample_v1.conv eurusd 2012-12-12 1
./build/currency_convert_v1 data/sample_v0.conv eurusd 2012-12-12 1
./build/currency_convert_v1 data/sample_v1.conv eurusd 2012-12-12 1
./build/currency_convert_v1 data/sample_v1.conv eurjpy 2012-12-12 1

# these don't
./build/currency_convert_v0 data/sample_v1.conv eurjpy 2012-12-12 1
./build/currency_convert_v1 data/sample_v1.conv eurusd 2020-01-01 1
```

# Slides

Slides for the presentation on the C++ Meetup Berlin are in [slides/](./slides/)

Or browse at [https://lbonn.github.io/sql-file/](https://lbonn.github.io/sql-file/)

# License

This code is licensed under the link:LICENSE[Mozilla Public License 2.0], a copy of which can be found in this repository.

Significant portions are derived from the [aktualizr code base](https://github.com/advancedtelematic/aktualizr), licensed on the same terms and copyright HERE Europe B.V., 2016-2019.
