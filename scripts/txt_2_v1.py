#!/usr/bin/env python3

import argparse
import csv
import datetime
import sqlite3
import sys


def date_conv(datest):
    m, _, _ = datest.partition(" ")
    m_num = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"].index(m) + 1
    datest = datest.replace(m, str(m_num).zfill(2))

    dt = datetime.datetime.strptime(datest, '%m %d, %Y %H:%M')
    return dt.strftime('%Y-%m-%dT%H:%M%SZ')


def main(db, conv_name, conv_desc, data_file):
    c = sqlite3.connect(db)
    csvr = csv.reader(data_file, delimiter='\t')
    for row in csvr:
        date = date_conv(row[0])
        val = float(row[1])
        c.execute(f"INSERT INTO eurusd VALUES ('{date}', '{val}');")
    c.commit()
    c.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--name', '-n', default='EURUSD')
    parser.add_argument('--desc', '-d', default='euros to dollars')
    parser.add_argument('--db', '-b', default='conv.db')
    parser.add_argument('file', type=argparse.FileType('r'), nargs='?', default=sys.stdin)
    args = parser.parse_args()

    main(args.db, args.name, args.desc, args.file)
